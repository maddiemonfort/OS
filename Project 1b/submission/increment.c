#include <stdio.h>
#include <stdlib.h>

void increment(int N, char* filename);

int main(int argc, char *argv[]) {
	//check that two arguments
	if(argc < 3) {
		//error message
		printf("ERROR: not enough arguments.\nInput should be of the form:\n./increment # filename.txt\n");
		return 0;
	}
	//if first argument is int
	if(atoi(argv[1])) {
		//call increment
		int N = atoi(argv[1]);
		char* file = argv[2];
		increment(N, file);
	}

	return 0;
}

void increment(int N, char* filename) {
	FILE *fp;
	for (int i = 0; i < N; ++i) {
		//open file
		fp = fopen(filename,"r");
		//read file
		int final= 0;
		int temp = 0; 
		while(fscanf(fp,"%d\n",&temp)!= EOF) {
			final = temp;
		}
		fclose(fp);
		//increment
		final++;
		//write incremented line
		fp = fopen(filename,"a+");
		fprintf(fp, "%d\n", final);
		//close file
		fclose(fp);
	}
}