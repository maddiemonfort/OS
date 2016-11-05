#include <lib.h>
#include <uninstd.h>
//include statements

int set_sv(int newVal, int* status) {
	//changes the value of shared_value to newVal
	//status is the status of the call -> always success (0)
	message m;
	int rw = 1;
	m.m1_i1 = rw;
	m.m1_i2 = newVal;

	int shared_val = _syscall(PM_PROC_NR, SVEDIT, &m);
	//*status = m.m1_i1;
	return shared_val;
	
}

int get_sv(int pid, int* status) {
	//pid == ID of process who's shared_value to be read
	//status == status of syscall
	message m;
	int rw = 0;
	m.m1_i1 = rw;
	m.m1_i2 = pid;

	int shared_val = _syscall(PM_PROC_NR, SVEDIT, &m);
	//*status = m.m1_i1;
	return shared_val;
}

//when calling the syscall: _syscall(PM_PROC_NR, SVEDIT, &m)
//m is a message