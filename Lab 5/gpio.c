#include <linux/gpio.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd0 = open("/dev/gpiochip0", O_RDWR); // open the file descriptor
	struct gpiochip_info cinfo;
	ioctl(fd0, GPIO_GET_CHIPINFO_IOCTL, &cinfo); // get the chip information
	fprintf(stdout, "GPIO chip 0: %s, \"%s\", %u lines\n", cinfo.name, cinfo.label, cinfo.lines);

	struct gpiohandle_request req_GYR, req_PB; // Green and Yellow
	struct gpiohandle_data data_GYR, data_PB;  // for data bit

	req_GYR.lines = 3;						   // 3 pins in this LED handler
	req_GYR.lineoffsets[0] = 4;				   // pin 4 - Green LED
	req_GYR.lineoffsets[1] = 17;			   // pin 17 - Yellow LED
	req_GYR.lineoffsets[2] = 27;			   // pin 27 - Red LED
	req_GYR.flags = GPIOHANDLE_REQUEST_OUTPUT; // set them to be output

	data_GYR.values[0] = 1;							 // set initial value of Green LED to High (ON)
	data_GYR.values[1] = 0;							 // set initial value of Yellow LED to Low (OFF)
	data_GYR.values[2] = 0;							 // set initial value of Red LED to Low (OFF)
	data_GYR.values[3] = 0;							 // set initial value of Push button to Low (OFF)
	ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_GYR); // now get the line handler req_GY

	req_PB.lines = 1;
	req_PB.lineoffsets[0] = 11;				 // pin 11 - Push button
	req_PB.flags = GPIOHANDLE_REQUEST_INPUT; // set it to be input

	data_PB.values[0] = 0;										  // set initial value of Push button to Low (OFF)
	ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_PB);				  // now get the line handler req_PB
	ioctl(req_PB.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data_PB); // read the value of the push button

	for (int i = 0; i < 10; ++i)
	{
		ioctl(req_GYR.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_GYR); // output data bits
		data_GYR.values[0] = !data_GYR.values[0];						// toggle
		data_GYR.value[1] = !data_GYR.values[1];

		// Get the push button value
		ioctl(req_PB.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data_PB); // read the value of the push button
		if (data_PB.values[0] == 1)
		{
			data_GYR.values[2] = !data_GYR.values[2]; // toggle the red LED
		}
		else
		{
			data_GYR.values[2] = 0; // turn off the red LED
		}

		usleep(1000000); // sleep for 1 second
	}

	close(req_GYR.fd); // release line
	close(req_PB.fd);  // release line
	close(fd0);		   // close the file
	exit(EXIT_SUCCESS);
}