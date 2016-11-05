
#include <lib.h>
#include <unistd.h>
#include <stdio.h>

//implementing lib functions
semaphore create_semaphore(int type, int initial_value) {
	//printf("it makes it into create_semaphore.\n");

	//error checking
	if(type == BINARY) {
		if(initial_value > 1 || initial_value < 0) {
			return NULL;
		}
	}
	else if(type == MULTIVALUED) {
		if(initial_value < 0) {
			return NULL;
		}
	}
	else return NULL; //not accurate type

	//printf("finishes checking the inputs.\n");

	message m;
	m.m1_i1 = type;
	m.m1_i2 = initial_value;

	int status = _syscall(PM_PROC_NR, CREAT_SEM, &m);
	//printf("Returns from syscall.\n");
	
	if(status == -1) { //if fails, return NULL
		return NULL;
	}

	semaphore s = m.m1_i1;
	printf("The semaphore index is: %d\n",s);
	return s;
}

int down(semaphore s) { //take
	message m;
	m.m1_i1 = s;

	int status = _syscall(PM_PROC_NR, DOWN_SEM, &m);
	
	if(status == -1) { //if call fails
		return NULL;
	}

	return -1; //call succeeded
}

int up(semaphore s) { //give
	message m;
	m.m1_i1 = s;

	int status = _syscall(PM_PROC_NR, UP_SEM, &m);
	//printf("EINVAL in lib is: %d\n", EINVAL);

	if(status == -1) { //if call fails
		return NULL;
	}

	return -1; //call succeeded
}

int delete_semaphore(semaphore s) {
	//obvious error checking
	if(s < 0) {
		return NULL;
	}
	message m;
	m.m1_i1 = s;

	int status = _syscall(PM_PROC_NR, DELETE_SEM, &m);
	//printf("It left syscall. status: %d\n", status);
	if(status == -1) { //if call fails
		return NULL;
	}

	return -1; //call succeeded

}

//EINVAL = 22
