
//under semaphore.h
typedef int semaphore;

#define NULL 0
#define BINARY 0
#define MULTIVALUED 1
#define SEMAPHORE_MAX 100
#define SEMAPHORE_UNUSED -1


//semaphore table
struct sem_arr {
	int value = SEMAPHORE_UNUSED;
	int type = NULL;
	int end = 0;
	int head = 0;
	pid_t Q[20];
	pid_t Ref[40];
	//hopefully those arrays have been initialized with NULL (or 0)
} sem_arr[SEMAPHORE_MAX];
//cant use semaphore 0 when returning values... (semaphore = index+1)



//referencing proc.h & proc.c from (kernel) AND mproc.h

/*----------------------------------------SYSCALLS--------------------------------------------*/
#include "pm.h"
#include "mproc.h"
#include "semaphore.h"
#include "param.h"
#include <stdio.h>

//----------------HELPER FUNCTIONS------------

void awake(register struct mproc* rmp, int val) {
	rmp->mp_reply.reply_res = val; //indicate success or failure
	rmp->mp_flags |= REPLY; //mark process as having a message
}

void delete_ref_sem(register struct mproc* rmp) {

}

void add_ref_sem(register struct mproc* rmp) {
	
}
/*----------------------------------SYSCALL FUNCTIONS--------------------------------*/

int do_create_semaphore() {
	//printf("It makes it into the syscall.\n");
	int s_type = m_in.m1_i1;
	int initial_value = m_in.m1_i2;
	//scan through table to find open entry

	static unsigned int next_semaphore = 0;
	int n = 0;
	
	//printf("beginnging to scan for an open semaphore in the table.");
  	if (sema_in_use == SEMAPHORE_MAX) {
  		printf("PM: warning, semaphore table is full!\n");
  		return EINVAL;
  	}

	/* Find a slot in 'sem_arr' for the semaphore.  A slot must exist. */
	do {
	    next_semaphore = (next_semaphore+1) % SEMAPHORE_MAX;
		n++;
	} while((sem_arr[next_semaphore].value != SEMAPHORE_UNUSED) && n <= SEMAPHORE_MAX); //while semaphore is in use & less than max
	if(n > SEMAPHORE_MAX) {
		printf("do_create_semaphore can't find semaphore slot");
		return EINVAL;
	}
	if(next_semaphore >= SEMAPHORE_MAX || (sem_arr[next_semaphore].value != SEMAPHORE_UNUSED)) {//if sem is placed in bad spot
		printf("do_create_semaphore finds wrong semaphore slot: %d", next_semaphore);
		return EINVAL;
	}

	//printf("got past the checks to find an open slot for the new semaphore.");

	sema_in_use++;
	//set the index value = intial_value
	sem_arr[next_semaphore].value = initial_value;
	// and type = type
	sem_arr[next_semaphore].type = s_type;
	// and add current pid (mp) to empty spot in ref[] - should be first spot because semaphore just created
	sem_arr[next_semaphore].Ref[0] = mp->mp_pid;

	//return index of first open entry (+1 -> can't send 0)
	int index = next_semaphore + 1;
	mp->mp_reply.m1_i1 = index;

	return OK;
}


int do_down() { //take semaphore
	semaphore s = m_in.m1_i1;

	if(sem_arr[s-1].value > 0) {
		sem_arr[s-1].value--;
		//mp->mp_reply.m1_i1 = sem_arr[s-1].value;
		return OK;
	}

	int i = sem_arr[s-1].end; //starts at end and loops around to head
	int end = i;
		 
	while(1) { //while(true)
		//prevent infinite loop
		if(i == sem_arr[s-1].head) {
			//weren't able to set a place for the  value on queue... not sure how to handle
			printf("PM: Warning, semaphore Queue is full, process not able to take semaphore.\n");
			//maybe return EINVAL?
			return EINVAL;
		}

		//push the process to the queue
		if(sem_arr[s-1].Q[i] == NULL) { //if empty slot
			sem_arr[s-1].Q[i] = mp->mp_pid;
			end = i; //new end of queue
			break;
		}

		i++;
		//account for looping around array
		if(i > 20) {
			i = 0;
		}
	}
	//set new end of queue
	sem_arr[s-1].end = end;

	//suspend process
	return SUSPEND;
}

int do_up() { //give semaphore
	semaphore s = m_in.m1_i1;

	//if queue is not empty
	int head = sem_arr[s-1].head;
	if(sem_arr[s-1].Q[head] != NULL) {
		pid_t next = sem_arr[s-1].Q[head]; //pop the next process from the queue
		
		//register the pop from the queue
		sem_arr[s-1].Q[head] = NULL;
		head++;
		if (head > 21) {
			head = 0;
		}

		//unsuspend the process--awake next
		register struct mproc *rmp = find_proc(next);
		if (rmp == NULL) panic("process no longer exists."); //this could make an ERROR
		awake(rmp, OK);

		return OK;
	}

	else {
		sem_arr[s-1].value++;
		//mp->mp_reply.m1_i1 = sem_arr[s-1].value;
		return OK;
	}
}

int do_delete_semaphore() {
	//find index s of table
	semaphore s = m_in.m1_i1;
	printf("semaphore index: %d\n", s);
	//check if it is valid
	if (s > SEMAPHORE_MAX) {
		return EINVAL;
	}
	int value = sem_arr[s-1].value;
	//printf("semaphore value: %d\n", value);
	if(sem_arr[s-1].value == SEMAPHORE_UNUSED) {
		return EINVAL;
	}

	//printf("It passed the checks for deleting semaphores.\n");

	//set it equal to unused
	sem_arr[s-1].value = SEMAPHORE_UNUSED;

	//NOTIFY ALL PROCESSES IN QUEUE
	int start = sem_arr[s-1].head;
	int stop = sem_arr[s-1].end;
	do { //iterate through queue
		pid_t pid_to_unblock = sem_arr[s-1].Q[start];
		if( pid_to_unblock != NULL) { //if there's something there
			register struct mproc *rmp = find_proc(pid_to_unblock);
			awake(rmp, EINVAL);
		}

		if(start == stop) { //this was the last value
			break;
		}
		//otherwise increment start
		start++;
		if(start > 20) {
			start = 0;
		}
		
	} while(1);

	sema_in_use--;
	
	//handle references

	return OK;
}
//EINVAL = -22

