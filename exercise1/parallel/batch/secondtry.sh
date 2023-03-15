#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=ex1
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load architecture/AMD
module load openMPI/4.1.4/gnu/12.2.1

export OMP_PLACES=cores
export OMP_PROC_BIND=close

mpirun -np 1 ./main.x -i -f init2 -k20000

echo cores,close,k20k,n50,1node >> ompscal2.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
        mpirun -np 1 --map-by socket ./main.x -r -f init2 -n50 -s1 -e1 >> ompscal2.dat

done

export OMP_PLACES=socket
export OMP_PROC_BIND=close

echo socket,close,k20k,n50,1node >> ompscal2.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
        mpirun -np 1 --map-by socket ./main.x -r -n50 -s1 -e1 -f init2 >> ompscal2.dat
done
