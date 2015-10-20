cl /I"%MSMPI_INC%\" /I"%MSMPI_INC%\\x64\\" max-mpi-ppv1.c /link "%MSMPI_LIB32%msmpi.lib"
mpiexec -n 5 max-mpi-ppv1.exe input\max.in