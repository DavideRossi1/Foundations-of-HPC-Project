#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

#define MAXVAL 255

#ifndef RW_PGM
#define RW_PGM
void write_pgm_image( unsigned char *image, int maxval, int xsize, int ysize, const char *image_name);
void read_pgm_image( unsigned char **image, int *maxval, int *xsize, int *ysize, const char *image_name);
#endif

/* //######################## just debugging ###################
 void printmatrix(unsigned char* matrix, int N){
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            printf("%d ",matrix[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
} 
//#####################################
 */

unsigned char checksingle(unsigned char *matrix,  long i, long N,long M){
    int left=N*(i%N==0);
    int right=N*((i+1)%N==0);
    long square=N*M;
    long up=square*((i-M)<0);
    long down=square*((i+M)>=square);
    unsigned char status=(
             matrix[i-1 + left]  // west el
            +matrix[i+1 - right] // east el
            +matrix[i-M + up] //north el
            +matrix[i+M - down] // south el
            +matrix[i-1-M + left + up] // nort-west el
            +matrix[i+1-M - right + up] // north-east el
            +matrix[i-1+M + left - down] // south-west el
            +matrix[i+1+M - right - down] // south-east el
            )/255; // note: all the elements need a correction to account for the fact that we want a 
            // "pacman effect": left and right apply a correction on the elements of the first and last 
            // column, up and down apply a correction on the elements of the first and last row
    return ((status==5 || status==6)? 0: MAXVAL );
 }

void updatematr(unsigned char *matrix,unsigned char *newmatrix,long N,long M){ //generalized for single pieces
        printf("\n new matrix \n");
        for(long i=0;i<N*M;i++){
        newmatrix[i]=checksingle(matrix, i, N,M);
        printf("%d ",newmatrix[i]);
    }
        printf("\n end new matrix \n");
}


/* unsigned char* enlargematrix(char* matrix, int N){
    unsigned char* newmatrix=(unsigned char*)malloc((N+2)*(N+2)*sizeof(unsigned char));
    for(int i=0;i<(N+2)*(N+2);i++){
        newmatrix[i]=matrix[i+];
    }

} */



//########################################################################

void run_static(char* filename, int steps, long dump, int N,int argc, char * argv[]){
    unsigned char* matrix;
    int maxval=MAXVAL;
    read_pgm_image(&matrix, &maxval, &N, &N,filename);
    unsigned char* tempmatr=(unsigned char*)malloc(N*N*sizeof(unsigned char));
    
    for (int i=0;i<steps;i++){
        if((i%2)==0){
            updatematr(matrix, tempmatr, N,N); 
            if (i%dump==0 && dump<steps) {
                char * filename = (char*)malloc(N*N*sizeof(unsigned char));
                sprintf(filename, "snap/snapshotstat_%03d",i);
                write_pgm_image(tempmatr, MAXVAL, N, N, filename);
                free(filename);
            }  
        }else{
            updatematr(tempmatr, matrix, N,N);
            if (i%dump==0 && dump<steps) {
                char * filename = (char*)malloc(N*N*sizeof(unsigned char));
                sprintf(filename, "snap/snapshotstat_%03d",i);
                write_pgm_image(matrix, MAXVAL, N, N, filename);
                free(filename);
            }  
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



//#######################################################################

void run_order(char* filename, int steps, long dump, int N){
    unsigned char* matrix;
    int maxval=MAXVAL;
    read_pgm_image(&matrix, &maxval, &N, &N,filename);
    for (int i=0;i<steps;i++){
        for(long j=0;j<N*N;j++){
            matrix[j] = checksingle(matrix,j,N,N);
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
