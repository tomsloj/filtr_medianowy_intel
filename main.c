#include <stdio.h> 		// angle brackets include looks for file 'stdio.h' 
#include <string.h>		//	in all the possible directories (including 
#include <stdlib.h>		//	this file directory or system directories)
#include<allegro5/allegro.h>
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_native_dialog.h> 


#include "filter.h" 	// quotation include looks for 'x86_function.h' ONLY in the  
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

	//write bitmap
    FILE *inputFile;
    inputFile=fopen(pText,"r");
	if(!inputFile)
	{
		fprintf( stderr, "Cannot open file!\n" );
        return - 1;
	}
    int BM, size, offset, headerSize, width, height;
    fread(&BM,2,1,inputFile);
    fread(&size,4,1,inputFile);
    fread(&offset,4,1,inputFile);
    fread(&offset,4,1,inputFile);
	fread(&headerSize,4,1,inputFile);
	fread(&width,4,1,inputFile);
	fread(&height,4,1,inputFile);
    fclose(inputFile);

	unsigned char header[offset];
	unsigned char tab[size - offset];
	
	inputFile=fopen(pText,"r");
	if(!inputFile)
	{
		fprintf( stderr, "Cannot open file!\n" );
        return - 1;
	}
	fread(&header,offset, 1,inputFile);
	fread(&tab, size - offset, 1, inputFile);
	fclose(inputFile);

	unsigned char * table = malloc(sizeof(unsigned char) * width * height);
	unsigned char * table2 = malloc(sizeof(unsigned char) * width * height);

	//padding
	int padding = (4 - ( width % 4 ))%4;

	int index = 0;
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			table[i * width + j] = tab[index];
			++index;
		}
		index += padding;
	}

	unsigned char* tab25 = malloc(25);

	filter(table, width, height, tab25, table2);			// call assembler procedure

	//padding
	index = 0;
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			tab[index] = table2[i * width + j];
			++index;
		}
		for( int j = 0; j < padding; ++j)
		{
			tab[index] = 0;
			++index;
		}
	}

	//save to file
	FILE *outputFile;
    outputFile=fopen("out.bmp","w");
	if(!outputFile)
	{
		fprintf( stderr, "Cannot open output file!\n" );
        return - 1;
	}
	fwrite(&header,offset, 1, outputFile);
	for(int i = 0; i < size - offset; ++i )
		fwrite(&tab[i], 1, 1, outputFile);
	fclose(outputFile);


	//display bitmaps
	al_init_image_addon();

	ALLEGRO_DISPLAY *display1 = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP * bitmap = al_load_bitmap(pText);
	ALLEGRO_BITMAP * bitmap2 = al_load_bitmap("out.bmp");

	if(!bitmap)
	{
		printf("Error with loading bitmap\n");
		exit(-1);
	}
	if(!bitmap2)
	{
		printf("Error with loading bitmap\n");
		exit(-1);
	}
	display1 = al_create_display(width * 2, height);
	if(!display1)
	{
        printf("Error with display\n");
		exit(-1);
    }
	
	al_draw_bitmap(bitmap, 0, 0, 0);
	al_draw_bitmap(bitmap2, width, 0, 0);
	al_flip_display();

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		printf("Error with event queue\n");
		exit(-1);
	}

	al_register_event_source(event_queue, al_get_display_event_source(display1));

	while(true)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			exit(0);
	}

	free(pText);				// free allocated memory
	free(table);
	free(table2);
	free(tab25);

	return 0;
	
}

