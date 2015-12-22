#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include "xBot.h"
#include <armadillo>
#include <termios.h>
#include <unistd.h>
#include "SDL/SDL.h"
#include "xboxctrl.h"

SDL_Event event;

static int stopsig;
using namespace arma;
static xBot bot;
static arma::vec motion = zeros<vec>(5);

static xboxctrl_t xBoxController;
static pthread_t xBoxControllerThread;

void *updateXboxController(void *args)
{
	while (!stopsig)
	{
		xboxctrl_update(&xBoxController);
	}
	return NULL;
}

void drive(double backLeft, double frontLeft, double backRight, double frontRight)
{

	// rightBack, rightFront, leftFront, leftBack

	motion[0] = -backRight;
	motion[1] = -frontRight;
	motion[2] = -frontLeft;
	motion[3] = -backLeft;
}

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

void stop(int signo)
{
	printf("Exiting yo >>>>\n");
	bot.startStop = true;
	stopsig = 1;
	exit(1);
}

int main(int argc, char *argv[])
{
	bot.startStop = false;
	signal(SIGINT, stop);

	xboxctrl_connect(&xBoxController);
	pthread_create(&xBoxControllerThread, NULL, updateXboxController, NULL);

	if(initSDL() == false)
	{
		return 1;
	}

	bool quit = false;

	while(!quit)
	{
		Uint8 *keystates = SDL_GetKeyState(NULL);

		printf("Xbox left joystick y-axis: %f", xBoxController.LJOY.y);

		double backLeft = 	xBoxController.LJOY.y - xBoxController.LJOY.x + xBoxController.RJOY.x;
		double frontLeft = 	xBoxController.LJOY.y + xBoxController.LJOY.x + xBoxController.RJOY.x;
		double backRight = 	xBoxController.LJOY.y + xBoxController.LJOY.x - xBoxController.RJOY.x;
		double frontRight = xBoxController.LJOY.y - xBoxController.LJOY.x - xBoxController.RJOY.x;

		float t = 0;

		if ((backLeft < t) && (backLeft > -t))
			backLeft = 0;
		if ((frontLeft < t) && (frontLeft > -t))
			frontLeft = 0;
		if ((backRight < t) && (backRight > -t))
			backRight = 0;
		if ((frontRight < t) && (frontRight > -t))
			frontRight = 0;

		drive(backLeft, frontLeft, backRight, frontRight);










		// rightBack, rightFront, leftFront, leftBack

		if (keystates[SDLK_a])
		{
			bot.send(vec({1, 1, -1, -1, 0}));
		}

		else if (keystates[SDLK_s])
		{
			bot.send(vec({-1, -1, 1, 1, 0}));
		}

		else if (keystates[SDLK_UP])
		{
			bot.send(vec({1, 1, 1, 1, 0}));
		}

		else if (keystates[SDLK_DOWN])
		{
			bot.send(vec({-1, -1, -1, -1, 0}));
		}

		else if(keystates[SDLK_LEFT])
		{
			bot.send(vec({1, -1, -1, 1, 0}));
		}

		else if(keystates[SDLK_RIGHT])
		{
			bot.send(vec({-1, 1, 1, -1, 0}));
		}

      /*else if(keystates[SDLK_y])
      {
         bot.send(vec({0, 0, 0, 0, 1}));
      }*/

		// else
		// {
		// 	bot.send(vec({0, 0, 0, 0, 0}));
		// }

		if(keystates[SDLK_q])
		{
			quit = true;
			SDL_Quit();
		}


		bot.send(motion);

		bot.readClear();

	}

	pthread_join(xBoxControllerThread, NULL);
	xboxctrl_disconnect(&xBoxController);

	SDL_Quit();
	return 0;
}