#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int main(int argc, char *argv[]) {
	int max_number = INT_MIN;
	int read_number;
	
	FILE *fp = fopen(argv[1], "r");
	while( fscanf(fp, " %d", &read_number) != EOF) {
		if(max_number < read_number) {
			max_number = read_number;
		}
	}
	fclose(fp);
	
	fp = fopen("output/max_serial.out", "w");
	fprintf(fp, "%d\n", max_number);
	fclose(fp);
	
	return 0;
}