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

static int stopsig;
using namespace arma;
static xBot bot;

int myGetch(void)
{
	struct termios oldt,
	newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

vec forward(double a)
{
	if (abs(a) > 1)
	{
		printf("whatchu doin boi\n");
		return vec({0, 0, 0, 0, 0});
	}
	return vec({a, a, a, a, a});
}

vec back(double a)
{
	if (abs(a) > 1)
	{
		printf("whatchu doin boi\n");
		return vec({0, 0, 0, 0, 0});
	}
	return vec({a, a, a, a, a});
}

vec left(double a)
{
	if (abs(a) > 1)
	{
		printf("whatchu doin boi\n");
		return vec({0, 0, 0, 0, 0});
	}
	return vec({a, a, a, a, a});
}

vec right(double a)
{
	if (abs(a) > 1)
	{
		printf("whatchu doin boi\n");
		return vec({0, 0, 0, 0, 0});
	}
	return vec({a, a, a, a, a});
}

void stop(int signo)
{
	printf("yo\n");
	stopsig = 1;
	exit(1);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, stop);

	while (true)
	{
		int numberOfSends = 0;
		char * data;
		char x;
		vec values = vec({0, 0, 0, 0, 0});
		vec oldValues = vec({0, 0, 0, 1, 0});
		x = myGetch();
		printf("getch: %c\n", x);


		if (x == 'A')
			values = vec({0, 1, 0, 0, 0});
		else if (x == 'B')
			values = vec({0, -1, 0, 0, 0});
		else if ((x == 'C') || (x == 'D'))
			values = vec({0, 0, 0, 0, 0});

		if (!(values(0) == oldValues(0) &&
			  values(1) == oldValues(1) &&
			  values(2) == oldValues(2) &&
			  values(3) == oldValues(3)))
		{
			numberOfSends++;
		 	bot.send(values);
		 	printf("SEND");
		 	oldValues = values;
		}



		if (numberOfSends > 20)
		{
			data = bot.read(1);
			printf("Data sent back to xBotRunner: %s\n", data);
			numberOfSends = 0;
		}
	}
	return 0;
}
