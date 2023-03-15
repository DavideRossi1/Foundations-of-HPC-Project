#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=ex1
#SBATCH --nodes=2
#SBATCH --tasks-per-node=2
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load architecture/AMD
module load openMPI/4.1.4/gnu/12.2.1

export OMP_PLACES=cores
export OMP_PROC_BIND=close

mpirun -np 4 ./main.x -i -f init3 -k20000

echo cores,close,k20k,n50,4socket >> ompscal3.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
	for i in {1..5}
        do mpirun -np 4 --map-by node --bind-to socket ./main.x -r -f init3 -n50 -s1 -e1 >> ompscal3.dat
	done
done

export OMP_PLACES=socket
export OMP_PROC_BIND=close

echo socket,close,k20k,n50,4socket >> ompscal3.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
	for i in {1..5}
        mpirun -np 4 --map-by node --bind-to socket ./main.x -r -n50 -s1 -e1 -f init3 >> ompscal3.dat
	done
done
