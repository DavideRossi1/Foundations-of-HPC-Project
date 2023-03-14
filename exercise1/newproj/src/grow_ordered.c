#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <mpi.h>
#include <string.h>
//#include <omp.h>

#ifndef RW_PGM
#define RW_PGM
void read_pgm_image( unsigned char  **image, int *maxval, int *xsize, int *ysize, const char *image_name);
void write_pgm_image( unsigned char *image, int maxval, int xsize, int ysize, const char *image_name);
#endif



#define MAXVAL 255

#define CPU_TIME (clock_gettime( CLOCK_MONOTONIC, &ts ), (double)ts.tv_sec +	\
		  (double)ts.tv_nsec * 1e-9)

void update_world(unsigned char * world, long size){

    //COPY of the first row in the last row
    //since we defined a size*(size+1) array as the world, the last row will be 
    //composed by world[size*size], world[size*size+1], ..., world[size*size+(size-1)]
    //while the first by world[0], world[1], ..., world[size-1]
    //for(long i=0; i<size; i++){
    //    world[size*size+i] = world[i];

        //printf("%d", world[i]);
    //}

    //now i want to update the matrix in a ordered way:
    //starting from the first element, neighbours will be considered in order to establish whether the cell must be remain alive or not.
    //then i will proceed in row-major direction, just as an old writing machine
    int SUM;

    for(long k=0; k<size*size; k++){

      long row = k/size;
      long col = k%size;

      SUM = 0;

        
        SUM = world[(size+row-1)%size*size + (size+col-1)%size] 
            + world[(size+row+0)%size*size + (size+col-1)%size] 
            + world[(size+row+1)%size*size + (size+col-1)%size] 
            + world[(size+row-1)%size*size + (size+col+0)%size] 
            + world[(size+row+1)%size*size + (size+col+0)%size] 
            + world[(size+row-1)%size*size + (size+col+1)%size] 
            + world[(size+row+0)%size*size + (size+col+1)%size] 
            + world[(size+row+1)%size*size + (size+col+1)%size];
            

        SUM = SUM/MAXVAL; //number of (dead) elements that surround the selected cell


        //dead if 1 or 0 cells alive in its neighbours (due to undercrowding) or if 
        //more than 3 cells alive in its neighbours (due to overcrowding)
        world[k] = MAXVAL;

        //alive (encoded as 255) if 2 or 3 cells in its neighbours (made of 8 cells) are alive
        if(SUM==5 || SUM==6){
           world[k] = 0; 
        }


        //only for the first line, we want to copy the updated version in the last line
        //in this way, in evaluating the TRUE last line, we consider the correct next row (i.e. the first updated row)
        //if(i<size){
        //    world[size*size+i] = world[i];
        //}
    }

}

//the program asks as input the number of steps that we need to perform the game of life: here "times"
void grw_serial(unsigned char* world, long size, int times, int snap){

  for(int i=1; i<=times; i++){
    //printf("qua ci arrivo");
    //at each iteration, we update the world
    update_world(world, size);

    //snaps = every how many steps a dump of the system is saved on a file (0 meaning only at the end)
    if(i%snap==0){
      //the number of iterations to save in fname could not exceed times
      char * fname = (char*)malloc(60);
      sprintf(fname, "snap/image_ORDERED_%03d",i+1);
      //we print ONLY these images
      write_pgm_image(world, MAXVAL, size, size, fname);
      free(fname);
    }
  }
}


void run_ordered(char * filename, int times, int dump){
  

  unsigned char * world;
  int size = 0;
  int maxval = 0; 
  
  //first of all, i need to read the previous world state
  /* printf("tutto bene \n");
  printf("controllino: %p, %p, %p, %d, %s \n", &world, &maxval, &size, size, filename);
   */
  read_pgm_image(&world, &maxval, &size, &size, filename);

  /* printf("ancora tutto bene \n");

  printf("controllino #2: %p, %d, %d, %d\n", &world, size, times, dump);
   */
  grw_serial(world, size, times, dump);

  /* for(int i=0; i<size*size; i++){
   printf("%d\n",  world[i]);
  }
  
  printf("\neccoci \n");
 */


  //free(world);
}



