#include "SDL/SDL.h"
#include <string>

SDL_Event event;

bool initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	SDL_Surface *screen = SDL_SetVideoMode(200, 200, 32, SDL_SWSURFACE);

	if(screen == NULL)
	{
		return false;
	}

	SDL_WM_SetCaption("xBot", NULL);

	return true;
}

int main(int argc, char* argv[])
{
	bool quit = false;

	if(initSDL() == false)
	{
		return 1;
	}

	while(quit == false)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		Uint8 *keystates = SDL_GetKeyState(NULL);

		if(keystates[SDLK_UP])
		{
			printf("UP PRESSED\n");
		}
		else
		{
			printf("UP NOT PRESSED\n");
		}

		if(keystates[SDLK_DOWN])
		{
			printf("DOWN PRESSED\n");
		}

		if(keystates[SDLK_LEFT])
		{
			printf("LEFT PRESSED\n");
		}

		if(keystates[SDLK_RIGHT])
		{
			printf("RIGHT PRESSED\n");
		}

		if(keystates[SDLK_q])
		{
			quit = true;
		}
	}

	SDL_Quit();
	return 0;
}