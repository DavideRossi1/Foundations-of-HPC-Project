#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
//#include <mpi.h>
//#include <omp.h>

void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name);

#define MAXVAL 255

void initialize(char* filename, long N){
    unsigned char* matrix=(unsigned char*)malloc(N*N*sizeof(unsigned char));
    for(long i=0;i<N*N;i++){
        int val=rand()%100;
        matrix[i]= (val>95? MAXVAL : 0);
        //matrix[i]=0; debugging
    }
    write_pgm_image(matrix, MAXVAL, N, N, filename);
    printf("\n");
    free(matrix);
}