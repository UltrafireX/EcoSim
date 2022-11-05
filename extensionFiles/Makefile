CC      = gcc
GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
LDLIBS 	= -lm
LDFLAGS = $(GTKLIB) -export-dynamic
CFLAGS  = -fno-omit-frame-pointer -Wall -O3 -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(LDFLAGS)
OBJS = main.o world.o helperFunctions.o fileRead.o creatureHelper.o fox.o rabbit.o creatureOrder.o 

Target = ../EcoSim

.SUFFIXES: .c .o

.PHONY: all clean

all: $(Target)

$(Target): $(OBJS)
	$(CC) -o $(Target) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(wildcard *.o)
	rm -f main

main.o:	main.h world.h helperFunctions.h fileRead.h #creatureHelper.h #fox.h rabbit.h creatureOrder.h
world.o:	world.h helperFunctions.h fox.h rabbit.h creatureOrder.h creatureHelper.h fileRead.h
helperFunctions.o:	helperFunctions.h world.h creatureOrder.h fileRead.h
creatureHelper.o:	world.h helperFunctions.h creatureOrder.h creatureHelper.h fileRead.h
fox.o:	helperFunctions.h creatureHelper.h world.h fox.h creatureOrder.h fileRead.h
rabbit.o:	helperFunctions.h creatureHelper.h world.h rabbit.h creatureOrder.h fileRead.h
creatureOrder.o:	helperFunctions.h world.h creatureHelper.h creatureOrder.h fileRead.h
fileRead.o: fileRead.h helperFunctions.h world.h creatureOrder.h creatureHelper.h