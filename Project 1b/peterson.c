#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

void safe_increment(int n, char* filename, int c_pid, int o_pid);
void enter_region(int current_pid, int other_pid);
void exit_region(int current_pid);

int main(int argc, char *argv[]) {
	//check that two arguments
	if(argc < 5) {
		//error message
		printf("ERROR: not enough arguments.\n");
		return 0;
	}
	//if first argument is int
	if(atoi(argv[1])) {

		int n = atoi(argv[1]);
		char* file = argv[2];
		int id = atoi(argv[3]);
		char* pids_file = argv[4];

		int other_pid;
		int current_pid;

		//suspend process
		sleep(1); //determine process' pid and make pids_file

		FILE* conf = fopen(pids_file, "r");
		int x, y;
		fscanf(conf, "%d %d", &x, &y);

		if(id == 0) {
		  other_pid = y;
		  current_pid = x;

		} else {
		  other_pid = x;
		  current_pid = y;
		}

		safe_increment(n, file, current_pid, other_pid);
	}

	return 0;
}

void enter_region(int current_pid, int other_pid) {
	int tA = get_sv(current_pid, &status);
	int tB = get_sv(other_pid, &status);
	
	int tAturn = tA >> 1;
	int tBturn = tB >> 1;

	//set XOR of turns == global turn
	int global_turn = tAturn ^ tBturn;
	if(global_turn == 0) {
		tBturn = tBturn^1; //set it not to be tBturn's turn 
	}

	tA = tAturn << 1;
	tA = tA | 1;
	set_sv(tA,&status);
	
	//enter busy waiting while loop
	while( ( (get_sv(other_pid,&status)>>1) ^ (get_sv(current_pid,&status)>>1) ) == global_turn &&  (get_sv(other_pid,&status)&1) == 1); 
}

void exit_region(int current_pid) {
	//set sv = no longer interested/using
	set_sv(0,&status);
}

void safe_increment(int n, char* filename, int c_pid, int o_pid) {
	FILE *fp;
	int status;
	setsv(0,&status); //set all sv to 0 or false, nothing has tried to access the crit section
	for (int i = 0; i < n; ++i) {
		//Peterson check!
		enter_region(c_pid, o_pid);

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

		exit_region(c_pid);
	}
}