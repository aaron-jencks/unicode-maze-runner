CC = gcc
export INCLUDE_PATH = include/
export SOURCE_PATH = source/
# export CFLAGS = -g -Ddebug

objects = utf8.o player.o game.o stringbuilder.o display.o gamedefs.o rand_util.o io.o
targets = main

all: $(targets) $(objects);

main: $(objects) main.c
	$(CC) $(CFLAGS) -o main main.c $(objects) -lreadline

utf8.o: utf8.c utf8.h
	$(CC) $(CFLAGS) -o utf8.o -c $<

stringbuilder.o: stringbuilder.c stringbuilder.h utf8.h
	$(CC) $(CFLAGS) -o stringbuilder.o -c $<

player.o: player.c player.h gamedefs.h
	$(CC) $(CFLAGS) -o player.o -c $<

game.o: game.c game.h gamedefs.h display.h rand_util.h
	$(CC) $(CFLAGS) -o game.o -c $<

gamedefs.o: gamedefs.c gamedefs.h
	$(CC) $(CFLAGS) -o gamedefs.o -c $<

display.o: display.c display.h utf8.h gamedefs.h
	$(CC) $(CFLAGS) -o display.o -c $<

rand_util.o: rand_util.c rand_util.h
	$(CC) $(CFLAGS) -o rand_util.o -c $<

io.o: io.c io.h
	$(CC) $(CFLAGS) -o io.o -c $<

.PROXY: clean
clean:
	rm $(targets) $(objects)
