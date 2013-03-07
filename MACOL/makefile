#CC=g++
CC=gcc
CFLAGS=-w -Wall -O3 -std=c99 -I $(SRCDIR)/util -I $(SRCDIR)
LDFLAGS=
EXEC=coloring
SRCDIR=./src
#BUILDDIR=./build
#SRC=$(wildcard *.c)#OBJ=$(SRC:.c=.o)

all: $(EXEC)

coloring: main.o graphe.o analyseGraphe.o gfile.o
	$(CC) -o $@ $^ $(LDFLAGS)


%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: $(SRCDIR)/util/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)