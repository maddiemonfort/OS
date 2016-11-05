#include "pm.h"
#include "mproc.h"
#include "param.h"
#include <stdio.h>

int do_svedit(int rw) {
	//shared_val may be read by ANY process
	//shared_val may be written only by owner (process who's process table entry holds shared_val)
	//mp = current process pointer
	//who_p for caller's process number
	//rw = whether read or write
	if(rw == 0) { //rw=0 means read
		//check if process
		return 0;
	}
	else if(rw == 1) {
		return 0;
	}
	else {
		printf("System call cannot interpret rw. Default is success");
		return 0;
	}
	//return: success == 0, failure == 1
}