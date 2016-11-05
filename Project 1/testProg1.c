#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

void test1();
void test2();

int* status;

int main() {
	setbuf(stdout, NULL);
	int* testNum;
	int isDone = FALSE;
	while(isDone == FALSE) {
		printf("---INPUT test number you wish to run. Or '0' to end. \n");
		scanf("%d",testNum);
		switch(*testNum) {
			case 0: {
				isDone = TRUE;
				break;
			}
			case 1: {
				test1();
				break;
			}
			case 2: {
				test2();
				break;
			}
			default: {
				isDone = TRUE;
				printf("There is no test case %d. \nRefer to the README doc for details on tests.\n",*testNum);
				break;
			}
		}
		
	}
	return 0;
}

//testing get_sv
void test1() { //t1: shared_value can be gotten
	int pid = getpid();
	int st = 1; //preset to failure
	status = &st;
	int shared_value = get_sv(pid, status);
	printf("shared_value: %d\n",shared_value);

	if (*status == 0) {
		printf("test1: shared_value can be gotten: SUCCESS\n");
	}
	else {
		printf("test1: shared_value cannot be gotten: FAILURE\n");
	}
}

void test2() { //t2: set shared_value
	int newVal = 33;
	int shared_value = set_sv(newVal, status);

	shared_value = get_sv(newVal, status);
	printf("shared_value: %d\n", shared_value);
	if(shared_value == newVal) {
		printf("test2: shared_value can be set: SUCCESS\n");
	}
	else {
		printf("test2: shared_value was not set: FAILURE\n");
	}
	
}

