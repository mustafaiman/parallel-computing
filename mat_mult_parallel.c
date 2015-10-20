#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>

#define MAX_LEN 100
int myid;
int dot_product(int size, int *vec1, int *vec2) {
	int sum = 0;
	int i;
	for(i=0; i<size; i++) {
		sum += vec1[i]*vec2[i];
	}
	return sum;
}

void get_column(int size, int column_number, int *mat, int *column) {
	int i;
	for(i=0; i<size; i++) {
		column[i] = mat[column_number+i*size];
	}
}

void get_row(int size, int row_number, int *mat, int *row) {
	int i;
	for(i=0; i<size; i++) {
		row[i] = mat[i + row_number*size];
	}
}

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	int numprocs, root_numprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	root_numprocs = (int)sqrt((double)numprocs);
	
	int dimension;
	int i, j;
	int *mat_r1, *mat_r2, *mat_result;
	int len;
	
	mat_r1 = (int *)malloc(sizeof(int) * MAX_LEN);
	mat_r2 = (int *)malloc(sizeof(int) * MAX_LEN);
	
	int group;
	
	if(myid == 0) {
		int *mat1, *mat2;
		
		FILE *fp = fopen(argv[1], "r");
		fscanf(fp, " %d", &dimension);
		
		mat1 = (int *)malloc(sizeof(int) * dimension * dimension);
		mat2 = (int *)malloc(sizeof(int) * dimension * dimension);
		mat_result = (int *)malloc(sizeof(int) * dimension * dimension);
		
		for(i=0; i<dimension*dimension; i++) {
			fscanf(fp, " %d", &mat1[i]);
		}
		fclose(fp);
		fp = fopen(argv[2], "r");
		
		//dimension is guaranteed to be the same with the previous one. this is empty reading
		fscanf(fp, " %d", &dimension);
		//read matrix column-wise
		for(i=0; i<dimension*dimension; i++) {
			fscanf(fp, " %d", &mat2[dimension * (i%dimension) + i/dimension]);
		}
		fclose(fp);

		group = dimension/root_numprocs;
		for(i=1; i<numprocs; i++) {
			MPI_Send(mat1+(i/root_numprocs)*(dimension*group), dimension*group, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
		}
		for(i=1; i<numprocs; i++) {
			MPI_Send(mat2+(i%root_numprocs)*(dimension*group), dimension*group, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
		}
		for(i=0; i<group*dimension; i++)
			mat_r1[i] = mat1[i];
		for(i=0; i<group*dimension; i++)
			mat_r2[i] = mat2[i];
		len = dimension*group;
	
	} else {
		MPI_Status status1, status2;
		MPI_Recv(mat_r1, MAX_LEN, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &status1);
		MPI_Get_count(&status1, MPI_INT, &len);
		
		MPI_Recv(mat_r2, MAX_LEN, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &status2);
		MPI_Get_count(&status2, MPI_INT, &len);
		
	}
	MPI_Bcast(&group, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	int *temp_c = (int *)malloc(sizeof(int) * group * group);
	for(i = 0; i<group; i++) {
		for(j = 0; j<group; j++) {
			temp_c[i*group + j] = dot_product(len/group, mat_r1+i*len/group, mat_r2+j*len/group);
		}
	}
	
	MPI_Gather(temp_c, group*group, MPI_INT, mat_result, group*group, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	if(myid == 0 ) {
		for(i=0; i< dimension; i++) {
			for(j=0; j< dimension; j++) {
				printf("%d ", mat_result[i/group * (dimension * group) + ((int)(j/group))*group*group + (i%group) * (group) + j%group]);
			}
			printf("\n");
		}
	}
		
	MPI_Finalize();
	
	
	return 0;
}