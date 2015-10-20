cl /I"%MSMPI_INC%\" /I"%MSMPI_INC%\\x64\\" matmult-mpi-1d.c /link "%MSMPI_LIB32%msmpi.lib"
mpiexec -n 9 matmult-mpi-1d.exe input/mat1 input/mat2 output/matmult-mpi-1d