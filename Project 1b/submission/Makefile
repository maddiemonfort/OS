all: increment consecutive safe_increment

increment:
	cc -g -o increment increment.c

consecutive:
	cc -g -o consecutive consecutive.c

safe_increment: 
	cc -g -o safe_increment peterson.c

clean:
	rm -rf safe_increment consecutive increment