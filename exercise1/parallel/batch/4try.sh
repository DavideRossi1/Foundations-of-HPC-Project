#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=ex1
#SBATCH --nodes=3
#SBATCH --nodelist=epyc[004-006]
#SBATCH --tasks-per-node=2
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load architecture/AMD
module load openMPI/4.1.4/gnu/12.2.1

export OMP_PLACES=cores
export OMP_PROC_BIND=close

mpirun -np 6 ./main.x -i -k20000

echo cores,close,k20k,n50,3nodes >> ompscal4.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
        for j in {1..3}
	mpirun -np 6 --map-by node --bind-to socket ./main.x -r -n50 -s1 -e1 >> ompscal4.dat
	done
done

export OMP_PLACES=socket
export OMP_PROC_BIND=close

echo socket,close,k20k,n50,1node >> ompscal4.dat
for i in {1..64}
        do export OMP_NUM_THREADS=$i
	for j in {1..3}
        mpirun -np 6 --map-by node --bind-to socket ./main.x -r -n50 -s1 -e1 >> ompscal4.dat
	done
done
