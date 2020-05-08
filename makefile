CC = gcc
CFLAGS = -Wall -m64 -no-pie
ASMFILE = filter
CFILE = main
NAME = filter
LIBS = -lallegro -lallegro_dialog -lallegro_image

all: main.o filter.o
	$(CC) $(CFLAGS) main.o filter.o -o filter $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

filter.o: filter.s
	nasm -f elf64 -g filter.s

clean:
	rm -f *.o filter
