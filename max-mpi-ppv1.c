#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<mpi.h>

#define MAX_NUM_SIZE 100000
#define MAX_SLAVES 20

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	int myid, numprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	if(myid == 0) {
		int numbers[MAX_NUM_SIZE];
		FILE *fp = fopen(argv[1], "r");
		
		//read the entire file
		int i;
		for(i=0;fscanf(fp, " %d", &numbers[i]) != EOF; i++);
		fclose(fp);
		
		int len = i;
		int num_clients = numprocs - 1;
		int partition_size_base = len/num_clients;
		//first len % numclients processors take partition_size_base + 1 numbers
		int offset = 0;
		for(i = 1; i<len % num_clients + 1; i++) {
			MPI_Send(numbers+offset, partition_size_base + 1, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
			offset += partition_size_base + 1;
		}
		//remaining processors take partition_size_base numbers
		for(i=len % num_clients + 1; i<numprocs; i++) {
			MPI_Send(numbers+offset, partition_size_base, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
			offset += partition_size_base;
		}
		
		//clients calculating local maxs
		int global_max = INT_MIN;
		for(i=1; i<numprocs; i++) {
			int local_max;
			MPI_Recv(&local_max, MAX_SLAVES, MPI_INT, MPI_ANY_SOURCE, 0xACE5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(local_max > global_max) {
				global_max = local_max;
			}
		}
		fp = fopen(argv[2],"w");
		fprintf(fp, "max-mpi-ppv1, number of processors: %d, input file: %s\n", numprocs, argv[1]);
		fprintf(fp, "%d\n", global_max);
		fclose(fp);
	} else {
		MPI_Status status;
		int numbers[MAX_NUM_SIZE/4+1];
		MPI_Recv(numbers, MAX_NUM_SIZE/4+1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &status);
		int len;
		MPI_Get_count(&status, MPI_INT, &len);
		
		int local_max = INT_MIN;
		
		int i;
		for(i=0; i<len; i++) {
			if(local_max < numbers[i]) {
				local_max = numbers[i];
			}
		}
		
		//let us send back local_max to master node
		MPI_Send(&local_max, 1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	
	return 0;
}