#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#define I2C_ADDR 0x64

int main(int argc, char **argv) {

	int fd;

	//char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[4];
	int result;
	int i;
	/* Open i2c device */

	fd = open("/dev/i2c-1", O_RDWR);
	if (fd < 0) {
		printf("Error opening i2c dev");
		exit(0);
	}
	
	/* Set slave address */

	result=ioctl(fd, I2C_SLAVE, I2C_ADDR);
	if (result < 0)
	{
		printf("Error settig I2C address");
		close(fd);
		exit(0);
	}	

	while(1){
		read(fd,buffer,sizeof(buffer));
		//	printf("Error Reading from I2C device");
		//	close(fd);
		//	exit(0);
		
		for( i=0; i<4; i++)
			printf("0x%x\r\n",buffer[i]);
		printf("--------------\r\n");
		sleep(1);
	}

	/* Close device */
	close(fd);

	return 0;
}
