
MPICC = mpicc
IDIR= ./include
CFLAGS= -Wall -pedantic -I$(IDIR)

OPENMP = -fopenmp

.PHONY : all clean

# source modules
deps = $(IDIR)/sorting_data.h $(IDIR)/kdtree.h $(IDIR)/utils.h
src = kdtree.c sorting_data.c main.c utils.c
objs = $(patsubst %.c, %.o, $(src))


all: kdtree 

%.o: %.c $(deps)
	$(MPICC) $(OPENMP) -c -o $@ $< $(CFLAGS)
	
kdtree: $(objs)
	$(MPICC) -g $(CFLAGS) $(OPENMP) $^ -o $@ 
	
# compile without openmp
#%.o: %.c $(deps)
#	$(MPICC) -c -o $@ $< $(CFLAGS)
#
#kdtree: $(objs_omp)
#	$(MPICC) -g $(CFLAGS) $^ -o $@ 

	
clean:
	rm kdtree 
	rm *.o
