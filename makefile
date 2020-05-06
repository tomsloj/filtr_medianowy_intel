CC = gcc
CFLAGS = -Wall -m64 -no-pie
ASMFILE = x86_function
CFILE = main
NAME = x86_function
LIBS = -lallegro -lallegro_dialog -lallegro_image

all: main.o x86_function.o
	$(CC) $(CFLAGS) main.o x86_function.o -o x86_function $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

x86_function.o: x86_function.s
	nasm -f elf64 -g x86_function.s

clean:
	rm -f *.o x86_function
