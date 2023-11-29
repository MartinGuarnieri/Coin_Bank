#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>
#include "write_display.h"

/* Initialize Disdplay */
int init_display(void){

	int fd;
	unsigned char buffer[17]; // buffer to write to display
	int result;
	/* Open i2c device */
	fd = open("/dev/i2c-1", O_RDWR);

	/* Set slave address */
	result=ioctl(fd, I2C_SLAVE, 0x70);
	if (result < 0) //error and exit if result < 0
	{
		printf("ioctl error");
		exit(0);
	}
	
	/* Turn on oscillator */
	buffer[0]=(0x2<<4)|(0x1);
	result=write(fd, buffer, 1);
	
	/* Turn on Display, No Blink */
	buffer[0]=(0x8<<4)|0x1;
	result=write(fd, buffer, 1);
	
	/* Set Brightness */
	buffer[0]=(0xE<<4)|0xA;
	result=write(fd, buffer, 1);
	
	return fd;
}

int write_display(int fd,double value){
	/* Array of digits 0-9  */
	int digits[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
	int decimal = 0x80; //To turn on decimal point, bit wise and (&) with this
	int error[3] = {0x79,0x31,0x31}; //Err to display invalid temperatures

	unsigned char buffer[17]; //char buffer to write to display
	int val = value * 100;    //multiply value by 10 to use integer math
	int first; //first 7-segment digit
	int second; //second 7-segment digit
	int third; //third 7-segment digit
	int fourth; //fourth 7-segment digit

	buffer[0]=0x00; // offset pointer

	if (0 <= value && value <= 9.99){ //0 to 9.9 
		fourth = val % 10;
		val /= 10;
		third = val % 10; //get the decimal place
		val/=10;          
		second = val % 10; //get the ones place
		
		buffer[1] = 0x00; // clear first digit to prevent leading zeros
		buffer[3] = digits[second] | decimal;//update  ones digit with decimal point
		buffer[7] = digits[third]; //update decimal digit
		buffer[9] = digits[fourth];
	}
	else if (9.9 < value && value <= 99.99){ //9.9 to 99.9
		fourth = val % 10;
		val /= 10;
		third = val % 10; //get the decimal place
		val/=10;
		second = val % 10; //get the ones place
		val /= 10;
		first = val % 10; //get the tens place
		
		buffer[1] = digits[first]; // update the tens place
		buffer[3] = digits[second] | decimal;//update the ones place with decimal point
		buffer[7] = digits[third]; //update the decimal place 
		buffer[9] = digits[fourth];
	}
	else{ //Otherwise an invalid value
		buffer[1] = error[0]; // E
		buffer[3] = error[1]; // r
		buffer[7] = error[2]; // r
		buffer[9] = 0;
	}
	
	write(fd,buffer,17); //write buffer to device

	return 0;
}

int shutdown_display(int fd){// shutdown display
	close(fd);	//close file descriptor
	return 0;
}
