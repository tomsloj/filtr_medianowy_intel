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
		printf("Not enought arguments");
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
    FILE *inputFile, *inputFile2;
    inputFile=fopen(pText,"r");
	if(!inputFile)
	{
		fprintf( stderr, "Cannot open file!\n" );
        return - 1;
	}
    int BM, size, offset;
    fread(&BM,2,1,inputFile);
    fread(&size,4,1,inputFile);
    fread(&offset,4,1,inputFile);
    fread(&offset,4,1,inputFile);
    fclose(inputFile);

	printf("%d\n", offset);

	unsigned char * header = malloc(offset + 1);
	unsigned char * table = malloc(size - offset + 1);
	
	
	inputFile2=fopen(pText,"r");
	if(!inputFile2)
	{
		fprintf( stderr, "Cannot open file!\n" );
        return - 1;
	}
	fread(&header,1, 10,inputFile2);
	//fread(&table, 1, size-offset, inputFile);
	fclose(inputFile);
	
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
	//al_set_window_title(display1, "in");
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

	/*
	for(int i = 0; i < height; ++i)
	{
		for( int j = 0; j < width; ++j )
		{
			printf("%d ",table[i*width + j]);
		}
		printf("\n");
	}
	*/

	unsigned char * table = malloc(sizeof(unsigned char) * al_get_bitmap_width(bitmap) * al_get_bitmap_height(bitmap) + 1);
	unsigned char * table2 = malloc(sizeof(unsigned char) * al_get_bitmap_width(bitmap) * al_get_bitmap_height(bitmap)*10 + 1);
	table[al_get_bitmap_width(bitmap) * al_get_bitmap_height(bitmap)] = '\0';

	ALLEGRO_LOCKED_REGION *lr = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_SINGLE_CHANNEL_8, ALLEGRO_LOCK_READWRITE);
	for (int i = 0; i < al_get_bitmap_height(bitmap); i++)
	{
		uint8_t *ptr = (uint8_t *)lr->data + i * lr->pitch;
		for (int j = 0; j < al_get_bitmap_width(bitmap);j++)
		{
			unsigned char pixel = *(ptr+0);
			printf("%d ", pixel);
			ptr += 1;
			table[i * al_get_bitmap_width(bitmap) + j] = pixel;
		}
		printf("\n");
 	}

	al_unlock_bitmap(bitmap);

	printf("\n");
	
	//al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGB_888);
	
	//al_draw_bitmap(bitmap, 0, 0, 0);
	//al_flip_display();
	unsigned char* tab25 = malloc(25);

	for( int i = 0; i < 5; ++i )
		{
			for( int j = 0; j < 5; ++j )
			{
				tab25[5*i+j]=11;
			}
		}


	x86_function(table, width, height, tab25, table2);			// call assembler procedure

	printf("\n");

	for( int i = 0; i < 5; ++i )
		{
			for( int j = 0; j < 5; ++j )
			{
				printf("%d ", tab25[5*i+j]);
			}
			printf("\n");
		}
	
	printf("\n");
	for(int i = 0; i < height; ++i)
	{
		for( int j = 0; j < width; ++j )
		{
			printf("%d ",table[i*width + j]);
		}
		printf("\n");
	}
	printf("\n");
	
	ALLEGRO_LOCKED_REGION *lr2 = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_SINGLE_CHANNEL_8, ALLEGRO_LOCK_READWRITE);
	for (int i = 0; i < al_get_bitmap_height(bitmap); i++)
	{
		uint8_t *ptr = (uint8_t *)lr->data + i * lr->pitch;
		for (int j = 0; j < al_get_bitmap_width(bitmap);j++)
		{
			*(ptr+0) = table2[i * al_get_bitmap_width(bitmap) + j];
			printf("%d ", table2[i * al_get_bitmap_width(bitmap) + j]);
			ptr += 1;
		}
		printf("\n");
 	}

	al_unlock_bitmap(bitmap);

	
	al_draw_bitmap(bitmap, al_get_bitmap_width(bitmap), 0, 0);
	al_flip_display();
	
	if(!al_save_bitmap("out.bmp", bitmap))
	{
		printf("Error with saving bitmap\n");
		exit(-1);
	}

	/*
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
	*/
	
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

