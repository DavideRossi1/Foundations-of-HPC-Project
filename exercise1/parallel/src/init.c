#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name);

#define MAXVAL 255



void initialize(char* filename, int N){
    int myid , numprocs;
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    int work=N/numprocs+(N%numprocs>myid);
    unsigned char* matrix=(unsigned char*)malloc(work*N*sizeof(unsigned char));
    for(long i=0;i<work*N;i++){
        matrix[i]= ((rand()%100) > 95? MAXVAL : 0);
        //matrix[i]=0; //debugging
    }
    /* matrix[1]=MAXVAL;
    matrix[2]=MAXVAL;
    matrix[3]=MAXVAL; */
    char * tempfile = (char*)malloc(work*N*sizeof(unsigned char));
    sprintf(tempfile, "partsnap_%3d",myid);
    write_pgm_image(matrix, MAXVAL, N, N, tempfile);
    free(matrix);
    MPI_Barrier(MPI_COMM_WORLD);
    if (myid==0){
        char * file = (char*)malloc(N*N*sizeof(unsigned char));
        sprintf(file, "init");
        write_pgm_image(matrix, MAXVAL, N, N, file);
        for (int i=0; i<numprocs;i++ ) {
            char* command=(char*)malloc(50);
            sprintf(command, "cat partsnap_%3d > init",i);
            system(command);
        }   
    };
    free(matrix);
}