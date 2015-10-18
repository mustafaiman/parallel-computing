cl /I"%MSMPI_INC%\" /I"%MSMPI_INC%\\x64\\" mat_mult_parallel.c /link "%MSMPI_LIB32%msmpi.lib"
mpiexec -n 9 mat_mult_parallel.exe input/mat1 input/mat2