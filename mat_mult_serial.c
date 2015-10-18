#include<stdio.h>
#include<stdlib.h>

int multiply_vec(int size, int *vec1, int *vec2) {
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
	int dimension;
	int *mat1, *mat2, *mat3, *temp_column, *temp_row;
	int i, j;
	
	FILE *fp = fopen(argv[1], "r");
	fscanf(fp, " %d", &dimension);
	
	mat1 = (int *)malloc(sizeof(int) * dimension * dimension);
	mat2 = (int *)malloc(sizeof(int) * dimension * dimension);
	mat3 = (int *)malloc(sizeof(int) * dimension * dimension);
	temp_row = (int *)malloc(sizeof(int) * dimension);
	temp_column = (int *)malloc(sizeof(int) * dimension);
	
	for(i=0; i<dimension*dimension; i++) {
		fscanf(fp, " %d", &mat1[i]);
	}
	fclose(fp);
	fp = fopen(argv[2], "r");
	
	//dimension is guaranteed to be the same with the previous one. this is empty reading
	fscanf(fp, " %d", &dimension);
	for(i=0; i<dimension*dimension; i++) {
		fscanf(fp, " %d", &mat2[i]);
	}
	fclose(fp);

	for(i=0; i<dimension; i++) {
		get_row(dimension, i, mat1, temp_row);
		for(j=0; j<dimension; j++) {
			get_column(dimension, j, mat2, temp_column);
			mat3[i*dimension + j] = multiply_vec(dimension, temp_row, temp_column);
		}
	}
	
	fp = fopen(argv[3], "w");
	for(i=0; i<dimension*dimension; i++) {
		fprintf(fp, "%d ", mat3[i]);
		if(i % dimension + 1 == dimension)
			fprintf(fp, "\n");
	}
	
	return 0;
}