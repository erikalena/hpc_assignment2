
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


all: create kdtree 

create:
ifeq ("$(wildcard obj)","")
	-mkdir obj
endif


obj/%.o: $(SRC)/%.c $(deps)
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
	rm -r obj
