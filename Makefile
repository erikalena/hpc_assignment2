
MPICC = mpicc
IDIR= ./include
CFLAGS= -Wall -pedantic -I$(IDIR)

OPENMP = -fopenmp

.PHONY : all clean

# source modules
deps = $(IDIR)/sorting_data.h $(IDIR)/kdtree.h $(IDIR)/utils.h
src = kdtree.c sorting_data.c main.c utils.c
objs = $(patsubst %.c, %.o, $(src))
objs_omp = $(patsubst %.c, %omp.o, $(src))

#all: kdtree omp_kdtree

%.o: %.c $(deps)
	$(MPICC) -c -o $@ $< $(CFLAGS)

%omp.o: %.c $(deps)
	$(MPICC) $(OPENMP) -c -o $@ $< $(CFLAGS)
	
kdtree: $(objs)
	$(MPICC) -g $(CFLAGS) $^ -o $@ 
	
# second executable using both mpi and openmp
omp_kdtree: $(objs_omp)
	$(MPICC) -g $(CFLAGS) $(OPENMP) $^ -o $@ 

	
clean:
	rm kdtree omp_kdtree
	rm *.o
