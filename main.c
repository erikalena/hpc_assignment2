#include "kdtree.h"



int countlines(char* file) {
    FILE* fp = fopen(file, "r");
    int count = 0,c;
	if(fp != NULL) {
        while ((c = getc(fp)) != EOF) 
              if (c == '\n') // Increment count if this character is newline 
                 count++; 
    }
    fclose(fp);
    return count;
}    

int main(int argc, char** argv) {
    
    int npoints;
    data_t *data;
    
    //load dataset
    
    if(argc > 1) {
        // read data if given
		npoints = countlines(argv[1]);
		data = (data_t*)malloc(npoints*sizeof(data_t));
		
		int i = 0;
		FILE* fp = fopen(argv[1], "r");
		float x,y;
		while(fscanf(fp,"%f,%f", &x, &y) ==2) {
			data[i].data[0] = x;
			data[i].data[1] = y;
			i++;
		}
		fclose(fp);
	}
	else {
	    // otherwise generate points randomly
	    npoints = NPOINTS;
	    data = (data_t*)malloc(npoints*sizeof(data_t));
        
        for ( int i = 0; i < npoints; i++ )
        for (int j = 0; j < NDIM; j++)
            data[i].data[j] = drand48()*MAX_VALUE;
	}
	    
	struct knode *root = NULL;
	
	//build kdtree on the given dataset
	#if defined(_OPENMP)

    #pragma omp parallel
    {
        #pragma omp single
	    root = build_kdtree(data, npoints, -1, 0);
    }
    #else 
         root = build_kdtree(data, npoints, -1, 0);
    #endif
    
    print_kdtree(root, 0);   
	return 0;
}
