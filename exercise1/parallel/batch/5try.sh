#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=ex1
#SBATCH --nodes=2
#SBATCH --tasks-per-node=2
#SBATCH --exclusive
#SBATCH --time=02:00:00


export OMP_PLACES=socket
export OMP_PROC_BIND=close

echo socket,close,k20k,n50,4socket >> ompscal5.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
        for i in {1..5}
        mpirun -np 4 --map-by node --bind-to socket ./main.x -r -n50 -s1 -e1 -f init3 >> ompscal5.dat
        done
done
