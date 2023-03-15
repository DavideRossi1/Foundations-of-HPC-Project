#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=ex1
#SBATCH --nodelist=epyc[006]
#SBATCH --tasks-per-node=1
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load architecture/AMD
module load openMPI/4.1.4/gnu/12.2.1

export OMP_PLACES=cores
export OMP_PROC_BIND=close

mpirun -np 1 ./main.x -i -f init4 -k10000

echo cores,close,k10k,n20,1node >> compmeth.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
        mpirun -np 1 --map-by socket ./main.x -r -n20 -s1 -e1 -f init4 >> compmeth.dat

done

echo cores,close,k10k,n20,1node >> compmeth.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
        mpirun -np 1 --map-by socket ./main.x -r -n20 -s1 -e1 -f init4 >> compmeth.dat
done
