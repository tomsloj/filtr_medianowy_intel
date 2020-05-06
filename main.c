#include <stdio.h> 		// angle brackets include looks for file 'stdio.h' 
#include <string.h>		//	in all the possible directories (including 
#include <stdlib.h>		//	this file directory or system directories)
#include<allegro5/allegro.h>
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_native_dialog.h> 


#include "x86_function.h" 	// quotation include looks for 'x86_function.h' ONLY in the  
				//	working directory and relative 
				//	directories (for example "../another_file.h")


const int WINDOW_WIDTH = 200;
const int WINDOW_HEIGHT = 200;

int main(int argc, char *argv[])
{
	char *pText = (char *) 0;
	if (argc < 2)				// check if text is provided
	{
		printf("NOt enought arguments");
		return -1;
	}
	
	pText = malloc(strlen(argv[1]) + 1);	// alloc mamory for copy of our text
	if (pText == NULL)
	{
		printf("Memory allocation failed.");
		return -1;
	}
	
	strcpy(pText, argv[1]);			// copy text

	if( !al_init() )
	{
        fprintf( stderr, "failed to initialize allegro!\n" );
        return - 1;
    }

	/*
	FILE *inputImage;
	unsigned char *inputImageArray;


	if ((inputImage = fopen(argv[1], "r+")) == NULL)
    {
        printf ("Cannot open file %s \n", argv[1]);
    }
	else
        {

            fseek(inputImage, 0, SEEK_END);
            int fileLength = ftell(inputImage);
            rewind(inputImage);

            inputImageArray = (unsigned char *)malloc((fileLength+1)*sizeof(unsigned char));
            fread(inputImageArray, fileLength, 1, inputImage);

			long offset = inputImageArray[10]<<3 + inputImageArray[11]<<2 + inputImageArray[12]<<1 + inputImageArray[13];
			long offset1 = inputImageArray[10] + inputImageArray[11]<<1 + inputImageArray[12]<<2 + inputImageArray[13]<<3;

			printf("%ld\n%d\n", offset, offset1);
		}
	*/
	





	al_init_image_addon();

	//al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_SINGLE_CHANNEL_8);
	ALLEGRO_DISPLAY *display1 = NULL;
	ALLEGRO_DISPLAY *display2 = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP * bitmap = al_load_bitmap(pText);
	//ALLEGRO_BITMAP * bitmap = al_load_bitmap_flags(pText,ALLEGRO_KEEP_INDEX);
	if(!bitmap)
	{
		printf("Error with loading bitmap\n");
		exit(-1);
	}
	
	display1 = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
	al_draw_bitmap(bitmap, 0, 0, 0);
	al_flip_display();
	al_set_window_title(display1, "in");
	if(!display1)
	{
        printf("Error with display\n");
		exit(-1);
    }

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		printf("Error with event queue\n");
		exit(-1);
	}
	
	int width = al_get_bitmap_width(bitmap);
	int height = al_get_bitmap_height(bitmap);

	unsigned char * table = malloc(sizeof(unsigned char) * al_get_bitmap_width(bitmap) * al_get_bitmap_height(bitmap) + 1);
	table[al_get_bitmap_width(bitmap) * al_get_bitmap_height(bitmap)] = '\0';

	ALLEGRO_LOCKED_REGION *lr = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_SINGLE_CHANNEL_8, ALLEGRO_LOCK_READWRITE);
	for (int i = 0; i < al_get_bitmap_height(bitmap); i++)
	{
		uint8_t *ptr = (uint8_t *)lr->data + i * lr->pitch;
		for (int j = 0; j < al_get_bitmap_width(bitmap);j++)
		{
			unsigned char pixel = *(ptr+0);
			//printf("%d ", pixel);
			ptr += 1;
			table[i * al_get_bitmap_width(bitmap) + j] = pixel;
		}
		printf("\n");
 	}

	al_unlock_bitmap(bitmap);
	//al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGB_888);
	
	//al_draw_bitmap(bitmap, 0, 0, 0);
	//al_flip_display();

	x86_function(table, width, height);			// call assembler procedure

	al_draw_bitmap(bitmap, al_get_bitmap_width(bitmap), 0, 0);
	al_flip_display();
	
	while(true)
	;
	if(!al_save_bitmap("out.bmp", bitmap))
	{
		printf("Error with saving bitmap\n");
		exit(-1);
	}
	display2 = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
	if(!display2)
	{
        printf("Error with display\n");
		exit(-1);
    }
	
	/*
	al_register_event_source(event_queue, al_get_display_event_source(display1));
	al_register_event_source(event_queue, al_get_display_event_source(display2));

	while(true)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			//al_save_bitmap("./bitmap.bmp", bitmap);
			exit(0);
	}
	*/


	free(pText);				// free allocated memory
	
	return 0;
	
}

