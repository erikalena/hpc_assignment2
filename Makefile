CC= gcc
MPICC = mpicc
IDIR= ./include
CFLAGS= -Wall -pedantic -I$(IDIR)

OPENMP = -fopenmp

.PHONY : all clean

# source modules
deps = $(IDIR)/quicksort.h $(IDIR)/kdtree.h $(IDIR)/utils.h
src = kdtree.c quicksort.c main.c utils.c
objs = $(patsubst %.c, %.o, $(src))

all: kdtree

%.o: %.c $(deps)
	$(MPICC) -c -o $@ $< $(CFLAGS)
	
kdtree: $(objs) 
	$(MPICC) -g $(CFLAGS) $^ -o $@ 

clean:
	rm kdtree
	rm *.o
