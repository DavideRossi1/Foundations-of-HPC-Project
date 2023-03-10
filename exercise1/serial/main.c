#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>



void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name);
void read_pgm_image( void **image, int *maxval, int *xsize, int *ysize, const char *image_name);
//void init(unsigned char *matrix);
//void update(unsigned char *matrix, int rep);
void initialize(char* filename, long N);
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

/*
########################### first attempt ##################
 void init(unsigned char *matrix){

    for(int i=0;i<N*N;i++){
        int val=rand()%100;
        matrix[i]= val>95? 255 : 0;
        //matrix[i]=0; debugging
    }
    char * filename = "init";
    write_pgm_image(matrix, MAXVAL, N, N, filename);
    printf("\n");
    //free(matrix);
}
 
####################### First trials: they work, need to add the read ####################
 void update(unsigned char *matrix, int rep){
    unsigned char* newmatr=(unsigned char*)malloc(N*N*sizeof(unsigned char));
    for(int i=0;i<N*N;i++){
        newmatr[i] = checkstatus(matrix,i);
    }
    for(int i=0;i<N*N;i++){
        matrix[i]=newmatr[i];
    }
    char * filename = "init2";
    write_pgm_image(matrix, MAXVAL, N, N, filename);
    printf("\n");
    free(newmatr);
}
void update_order(unsigned char *matrix){
    for(int i=0;i<N*N;i++){
        matrix[i] = checkstatus(matrix,i);
    }
    char * filename2 = "init3";
    write_pgm_image(matrix, MAXVAL, N, N, filename2);

    printf("\n");
}
#############################################


void run_order(char* filename, int steps, int dump){

}

############ OLD MAIN ####################
int main(int argc, char **argv){

    int action = 0;
    int c;

    char *optstring = "i:rk:e:f:n:s:";
    while ((c = getopt(argc, argv, optstring)) != -1) {
        switch(c) {
            case 'i':
                N=atoi(optarg);
                action=INIT; break;
        }
    }
    if (action==INIT) {
        printf("test");
        unsigned char* matrix = (unsigned char *)malloc(N*N*sizeof(unsigned char));
        init(N,matrix);
        matrix[0]=255;
        matrix[1]=255;
        matrix[2]=255;
        matrix[4]=255;
        matrix[21]=255;
        //matrix[93]=255;
        printmatrix(N, matrix);
        update_order(N,matrix);
        printmatrix(N, matrix);
        free(matrix);
        return 0;
    }
} 
*/
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
        initialize(filename,N);
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