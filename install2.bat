cl /I"%MSMPI_INC%\" /I"%MSMPI_INC%\\x64\\" max_mpi_ppv1.c /link "%MSMPI_LIB32%msmpi.lib"
mpiexec -n 5 max_mpi_ppv1.exe input\max.in