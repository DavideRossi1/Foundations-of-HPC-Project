#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name);

#define MAXVAL 255



void initialize(char* filename, int N, int argc, char * argv[]){
    int myid , numprocs;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    int work=N/numprocs+(N%numprocs>myid);
    unsigned char* matrix=(unsigned char*)malloc(work*N*sizeof(unsigned char));
    for(long i=0;i<work*N;i++){
        //matrix[i]= ((rand()%100) > 95? MAXVAL : 0);
        matrix[i]=0; //debugging
    }
    matrix[0]=MAXVAL;
    matrix[19]=MAXVAL;
    matrix[15]=MAXVAL;
    char * tempfile = (char*)malloc(work*N*sizeof(unsigned char));
    sprintf(tempfile, "partsnap%d",myid);
    
    write_pgm_image(matrix, MAXVAL, N, work, tempfile);
    free(matrix);
    MPI_Barrier(MPI_COMM_WORLD);
    if (myid==0){
        char * file = (char*)malloc(N*N*sizeof(unsigned char));
        sprintf(file, "init");
        write_pgm_image(matrix, MAXVAL, N, N, file); //here is the problem: how to initialize the file?
        
        for (int i=0; i<numprocs;i++ ) {
            char* command=(char*)malloc(100);
            sprintf(command, "cat partsnap%d >> init",i);
            system(command);
        }   
        //system("rm partsnap*");
    }
    MPI_Finalize();
    //free(matrix);
}