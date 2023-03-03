## Exercise 2

This folder contains 4 directories: 

- `blis` and `myblis` are the compiled libraries needed to run the benchmarks with BLIS;

- `part1` contains all the files used to measure scalability over the size of the matrix at fixed number of cores. It is divided in `single-precision` and `double-precision`, each of them contains the `gemm.c` file, the `Makefile`, set to `DUSE_FLOAT` or `DUSE_DOUBLE` for each case, and the folders `mkl`, `oblas` and `blis`, and each of them contains the executable, the batch files and the output files for that library;

- `part2` contains all the files used to measure scalability over the number of cores at fixed size. The structure is the same of the previous folder

The batch and output files are named so that you can easily understand what benchmark it corresponds to, following the order of the directories: for example, `p1-sp-mkl-cl.sh` is the batch file of part1, single precision, MKL library, with close binding policy.
