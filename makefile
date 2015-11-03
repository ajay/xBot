CC = g++
CFLAGS = -pedantic -Wall -std=c++11 -O3
LIBS = -larmadillo -lSDL -pthread

OBJECTS = xBot.o serial.o xBotRunner.o

all: $(OBJECTS) xBot

serial.o: serial.c serial.h
	gcc -pedantic -Wall -Werror -std=gnu99 -O3 -o $@ -c $<

xBot.o: xBot.cpp xBot.h
	$(CC) $(CFLAGS) -o $@ -c $<

xBotRunner.o: xBotRunner.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

xBot: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rfv *.o xBot
