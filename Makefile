CC= gcc
MPICC = mpicc
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
	$(MPICC) -c -o $@ $< $(CFLAGS)
	
kdtree: $(objs) 
	$(MPICC) -g $(CFLAGS) $^ -o $@ 

clean:
	rm kdtree
	rm *.o
