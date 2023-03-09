#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXVAL 255

#ifndef RW_PGM
#define RW_PGM
void write_pgm_image( unsigned char *image, int maxval, int xsize, int ysize, const char *image_name);
void read_pgm_image( unsigned char **image, int *maxval, int *xsize, int *ysize, const char *image_name);
#endif

unsigned char checksingle(unsigned char *matrix,  int i, int N){
    int left=N*(i%N==0);
    int right=N*((i+1)%N==0);
    unsigned int square=N*N;
    int up=square*((i-N)<0);
    int down=square*((i+N)>=square);
    unsigned char status=(
             matrix[i-1 + left]  // west el
            +matrix[i+1 - right] // east el
            +matrix[i-N + up] //north el
            +matrix[i+N - down] // south el
            +matrix[i-1-N + left + up] // nort-west el
            +matrix[i+1-N - right + up] // north-east el
            +matrix[i-1+N + left - down] // south-west el
            +matrix[i+1+N - right - down] // south-east el
            )/255; // note: all the elements need a correction to account for the fact that we want a 
            // "pacman effect": left and right apply a correction on the elements of the first and last 
            // column, up and down apply a correction on the elements of the first and last row
    return ((status==2 || status==3)? MAXVAL: 0 );
 }

void updatematr(unsigned char *matrix,unsigned char *newmatrix,int N){
    for(int i=0;i<N*N;i++){
        newmatrix[i]=checksingle(matrix, i, N);
    }
}

void run_static(char* filename, int steps, int dump, int N){
    unsigned char* matrix;
    int maxval=MAXVAL;
    read_pgm_image(&matrix, &maxval, &N, &N,filename);
    unsigned char* tempmatr=(unsigned char*)malloc(N*N*sizeof(unsigned char));
    
    for (int i=0;i<steps;i++){
        if((i%2)==0){
            updatematr(matrix, tempmatr, N); 
        }else{
            updatematr(tempmatr, matrix, N);
        }
        if (i%dump==0) {
            char * filename = (char*)malloc(N*N*sizeof(unsigned char));
            sprintf(filename, "snap/snapshotstat_%03d",i);
            write_pgm_image(matrix, MAXVAL, N, N, filename);
            free(filename);
        }  
    }
    if (dump>steps) {
        char * filename = (char*)malloc(N*N*sizeof(unsigned char));
        sprintf(filename, "snap/snapshotstat_%03d",steps);
        write_pgm_image(matrix, MAXVAL, N, N, filename);
        free(filename);
    }
    free(tempmatr);    
}

void run_order(char* filename, int steps, int dump, int N){
    unsigned char* matrix;
    int maxval=MAXVAL;
    read_pgm_image(&matrix, &maxval, &N, &N,filename);
    for (int i=0;i<steps;i++){
        for(int i=0;i<N*N;i++){
            matrix[i] = checksingle(matrix,i,N);
        }
        if (i%dump==0) {
            char * filename = (char*)malloc(N*N*sizeof(unsigned char));
            sprintf(filename, "snap/snapshotord_%03d",i);
            write_pgm_image(matrix, MAXVAL, N, N, filename);
            free(filename);
        }
    }
    if (dump>steps) {
        char * filename = (char*)malloc(N*N*sizeof(unsigned char));
        sprintf(filename, "snap/snapshotord_%03d",steps);
        write_pgm_image(matrix, MAXVAL, N, N, filename);
        free(filename);
    }
}
