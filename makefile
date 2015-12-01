CC = gcc
CFLAGS = -pedantic -Wall -Werror -std=gnu99 -O3 -o
COMPILEC = $(CC) $(CFLAGS)

CPPC = g++
CPPFLAGS = -pedantic -Wall -std=c++11 -O3 -o
COMPILECPP = $(CPPC) $(CPPFLAGS)
LIBS = -larmadillo -lSDL -pthread

OBJECTS = xBot.o serial.o xBotRunner.o

all: $(OBJECTS) xBot

serial.o: serial.c serial.h
	$(COMPILEC) $@ -c $<

xBot.o: xBot.cpp xBot.h
	$(COMPILECPP) $@ -c $<

xBotRunner.o: xBotRunner.cpp
	$(COMPILECPP) $@ -c $<

xBot: $(OBJECTS)
	$(COMPILECPP) $@ $^ $(LIBS)

clean:
	rm -rfv *.o xBot