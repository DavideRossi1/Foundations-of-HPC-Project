#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mpi.h>


void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name);
void read_pgm_image( void **image, int *maxval, int *xsize, int *ysize, const char *image_name);
void initialize(char* filename, int N,int argc, char * argv[]);
void run_static(char* filename, int steps, long dump, int N);
void run_order(char* filename, int steps, long dump, int N);



#define MAXVAL 255
//initialize the playground
#define INIT 1
//run the playground
#define RUN  2
//default playground size
#define N_DFLT 100
//default number of iterations
#define N_ITER 50
//default number of iterations between each snap
#define DUMP_DFLT 0
//evolution types
#define ORDERED 0
#define STATIC  1


int main(int argc, char * argv[]){

    int   action = 0;
    int   N      = N_DFLT;

    //number of steps to be calculated
    int   n      = N_ITER;

    //every how many steps a dump of the system is saved on a file
    long   s      = DUMP_DFLT;

    char *optstring = "irk:e:f:n:s:";
    int e = STATIC;
    int c;

    char * filename="init";
    /* 
    MPI_Init(&argc,&argv);
    int numprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

 */


    while ((c = getopt(argc, argv, optstring)) != -1) {
        switch(c) {
      
        case 'i':
            action = INIT; break;
      
        case 'r':
            action = RUN; break;
      
        case 'k':
            N = atoi(optarg); break;

        case 'e':
            e = atoi(optarg); break;

        case 'f':
            filename = (char*)malloc( sizeof(optarg)+1 );
            sprintf(filename, "%s", optarg );
            break;

        case 'n':
            n = atoi(optarg); break;

        case 's':
            s = atoi(optarg); break;

        default :
        printf("argument -%c not known\n", c ); break;
        }
    }

    // if s=0 we want to only print the last iteration, let's set it 
    // to a huge number so that it will be greater than the number of steps for sure
    if(s == 0){
        s = 1000000;
    }
  
    if(action == INIT){
        printf("Initialize matrix with size %d\n",N);
        initialize(filename,N,argc,argv);
    }
  
    if(action == RUN & e == STATIC){
        printf("Run static with size %d, %d iterations, saving a file each %ld iterations\n",N,n,s);
        run_static(filename,n,s,N);
    }


    if(action == RUN & e == ORDERED){
        printf("Run ordered with size %d, %d iterations, saving a file each %ld iterations\n",N,n,s);
        run_order(filename,n,s,N);
    }
  return 0;
} 