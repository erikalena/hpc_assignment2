MPICC = mpicc
IDIR= ./include
SRC= ./src
CFLAGS= -Wall -pedantic -I$(IDIR)

OPENMP = -fopenmp

.PHONY : all clean

# source modules
deps = $(IDIR)/sorting_data.h $(IDIR)/kdtree.h $(IDIR)/utils.h
src = kdtree.c sorting_data.c main.c utils.c
objs = $(patsubst %.c, obj/%.o, $(src))
objs_omp = $(patsubst %.c, obj/omp_%.o, $(src))

all: create kdtree omp_kdtree

create:
ifeq ("$(wildcard obj)","")
	-mkdir obj
endif


obj/omp_%.o: $(SRC)/%.c $(deps)
	$(MPICC) $(OPENMP) -c -o $@ $< $(CFLAGS)
	
omp_kdtree: $(objs_omp)
	$(MPICC) $(CFLAGS) $(OPENMP) $^ -o $@ 
	
# compile without openmp
obj/%.o: $(SRC)/%.c $(deps)
	$(MPICC) -c -o $@ $< $(CFLAGS)

kdtree: $(objs)
	$(MPICC) $(CFLAGS)  $^ -o $@ 
	
clean:
	rm kdtree omp_kdtree
	rm -r obj
