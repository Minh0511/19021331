#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

SDL_Surface* image = NULL;


bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	image = SDL_LoadBMP( "cat.bmp" );
	if( image == NULL )
	{
		cout<<"error"<<endl;
		success = false;
	}

	return success;
}


#endif // BACKGROUND_h
