//includes
#include <stdio.h>

int main(int argc, char *argv[]) {
	//read from stdin: FILE* stdin
	int num = 0;
	//get first line and set the temp value
	int temp;
	fscanf(stdin,"%d\n",&temp);
	//read the rest of the file line by line
	while(fscanf(stdin,"%d\n",&num)!= EOF ) {
		if (num != (temp+1)) {
			//then it is not consecutive
			fprintf(stdout, "%d %d\n", temp, num);
		}
		temp = num;
	}
	return 0;
}
