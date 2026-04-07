#ifndef TEXTURE_H
#define TEXTURE_H
 
#include "SDL.h"
#include "defs.h"

typedef struct Texture Texture;
struct Texture
{
	char name[MAX_FILENAME_LENGTH];
	SDL_Texture* texture;
	Texture* next;
};


#endif // !TEXTURE_H

