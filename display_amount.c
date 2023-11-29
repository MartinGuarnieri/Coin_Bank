#include <stdio.h>
#include <unistd.h>
#include "write_display.h"
//#include "read_temp.h"
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {

	if (argc < 2){
		printf("Need Parameter");
		return -1;
	}
	
	double value = atof(argv[1]);
	int fd;
	
	//while(1){
	fd=init_display(); //initialize the display and open file descriptor
	//value = read_weight(); // get weight from sensor
	value = round(value * 100) / 100; //round to 2 decimal place
	write_display(fd,value);	//write temperature to display
	shutdown_display(fd); //close file descriptor
	//}

	return 0;
}

