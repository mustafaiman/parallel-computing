cl /I"%MSMPI_INC%\" /I"%MSMPI_INC%\\x64\\" max-mpi-ppv2.c /link "%MSMPI_LIB32%msmpi.lib"
mpiexec -n 5 max-mpi-ppv2.exe input\max.in