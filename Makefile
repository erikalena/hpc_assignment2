CC= gcc
IDIR= ./include
CFLAGS= -Wall -pedantic -I$(IDIR)

OPENMP = -fopenmp

.PHONY : all clean

# source modules
deps = $(IDIR)/quicksort.h $(IDIR)/kdtree.h
src = kdtree.c quicksort.c main.c
objs = kdtree.o quicksort.o main.o #$(patsubst %.c, %.o, $(src))

all: kdtree

%.o: %.c $(deps)
	$(CC) $(OPENMP) -c -o $@ $< $(CFLAGS)
	
kdtree: $(objs) 
	$(CC) -g $(OPENMP) $(CFLAGS) $^ -o $@ 

clean:
	rm kdtree
	rm *.o
