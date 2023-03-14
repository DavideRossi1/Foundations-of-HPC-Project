
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

//write function:
void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name);

#define MAXVAL 255

int generate_seed(int omp_rank, int mpi_rank){
  return 2*omp_rank*mpi_rank+omp_rank*omp_rank+mpi_rank*mpi_rank+100;
}


void initialize_serial(const char* filename, unsigned char * world, long size){
    
    //size = number of elements in a row or in a column (a square matrix is used)
    world = (unsigned char *)malloc(size*size*sizeof(unsigned char));

    for(long long i=0; i<size*size; i++){
        
        /* int val = rand()%100;
        if(val>70){
            world[i]=MAXVAL; //white = dead
        }else{
            world[i]=0; //black = alive
        } */
        world[i]=MAXVAL;
    }
    world[25]=world[35]=world[45]=world[55]=world[65]=world[34]=world[36]=world[43]=world[44]=world[46]=world[47]=world[54]=world[56]=0;



    write_pgm_image(world, MAXVAL, size, size, filename);

    free(world);
}


void initialize_parallel(unsigned char * world, int pSize, int pRank, int* rcounts, int* displs){
//for(int i=0; i<pSize; i++){
//printf("rcounts del processo %d contiene gli elementi: %d\n", pRank, rcounts[i]);
//}    
    //for example if we have size=10 and pSize=3, the work (i.e. rows) subdivision would be 4-3-3
    //long smaller_size = size%pSize <= pRank? (long)(size/pSize) : (long)(size/pSize) +1;

    //size = number of columns, 
    //smaller_size = number of rows that each process need to analyze
    //smaller_size+2 = overall number of rows that each process receive -> one extra row above, one below
   unsigned char * process_world = (unsigned char *)malloc(rcounts[pRank]*sizeof(unsigned char));

  #pragma omp parallel
  {
    //set seed
    srand(generate_seed(pRank, omp_get_thread_num()));

    #pragma omp for schedule(static)
    //SUCH AS THE SERIAL VERSION:
    for(long long i=0; i<rcounts[pRank]; i++){
        
        int val = rand()%100;
        if(val>50){
            process_world[i]=0; //black = dead
        }else{
            process_world[i]=MAXVAL; //white = alive
        }
    }
}

    //for(int k=0; k<size*size; k++){
       // printf("%d ", process_world[k]);
       // if(k%size==0){
         //   printf("\n");
        //}
    //}
//printf("\n");

//printf("qua dovrei scrivere %d elementi\n", rcounts[pRank]);
MPI_Gatherv(process_world, rcounts[pRank], MPI_UNSIGNED_CHAR, world, rcounts, displs, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//printf("il processo %d ha inviato %d elementi al processo 0\n", pRank, rcounts[pRank]);
   free(process_world);
}

void choose_initialization(const char * filename, long size, int * argc, char ** argv[]){
//printf("%d", size);
    int pRank, pSize; 
unsigned char* world = (unsigned char*)malloc(size*size*sizeof(char));

    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pRank);
    MPI_Comm_size(MPI_COMM_WORLD, &pSize);
    
//printf("ciao, sono %d\n", pRank);

//here the determination of rcounts and displs:
    int* displs = (int *)malloc(pSize*sizeof(int)); 
    int* rcounts = (int *)malloc(pSize*sizeof(int)); 

int smaller_size;
int cumulative=0;

if(pRank==0){

	for(int i=0; i<pSize; i++){
		//numero di righe che ogni processo dovrà gestirsi
		smaller_size = size%pSize <= i? size/pSize: size/pSize+1;

		//numero di elementi che ogni processo dovrà gestirsi
		rcounts[i] = smaller_size*size;

		//indice del primo elemento che l'i-esimo processo deve inizializzare
		displs[i] = cumulative;
		
		//aggiorno cumulative, che mi dice da quale elemento dovrà partire il processo successivo
		cumulative = cumulative+rcounts[i];
//		printf("prima del broadcast, il processo 0 ha %d, %d\n", rcounts[i], displs[i]);
	}
//printf("%ld %d\n", size*size, cumulative);

}

 MPI_Bcast(rcounts, pSize, MPI_INT,0, MPI_COMM_WORLD);
MPI_Bcast(displs, pSize, MPI_INT, 0, MPI_COMM_WORLD);


//l'idea qua è di fare calcolare a un solo processo tutte ste cose in modo da evitare comunicazioni tra processi
//perché qua pRank è diverso per ogni processo

//quindi tocca che il singolo processo si calcoli tutto!

//int cumul = 0;
//for(int i=0; i < pSize; i++){
    //int smaller_size = size%pSize <= pRank ? size/pSize +1 : size/pSize;
    //cumul += smaller_size*size;
//qua arrivano in vario ordine: 3-2-3-2, per esempio
//for(int i =0; i<pSize; i++){
//int smaller_size;
//	if(i!=pRank){
//		smaller_size= size%pSize < i ? size/pSize : size/ pSize+1;
//	}else{
//		smaller_size=size%pSize <= pRank ? size/pSize : size/pSize+1;
		//printf("%d\n", smaller_size);
//	}
//		rcounts[i] = smaller_size*size;
//		printf("%d, %d\n", i, rcounts[i]);
//}


//}

if(pSize > 1){
//	printf("parallelo\n");
	for(int i=0; i<pSize; i++){
//printf("processo %d ha %d come rcounts\n", pRank, rcounts[i]);
}
        initialize_parallel(world, pSize, pRank, rcounts, displs);
//printf("processo %d è arrivato con %d elementi\n", pRank, rcounts[pRank]);
MPI_Barrier(MPI_COMM_WORLD);
	if(pRank==0){
		write_pgm_image(world, MAXVAL, size, size, filename);
	}
}else{
//	printf("seriale\n");
        initialize_serial(filename, world, size);
}
  MPI_Finalize();
  free(world);
}
