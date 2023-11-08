#include <linux/gpio.h>
#include <stdio.h>
#include <stdlib.h>
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

	struct gpiohandle_request req_GYR, req_PB; // Green, Yellow and Red, and Push Button
	struct gpiohandle_data data_GYR, data_PB;  // for data bit
	
	int isButtonPressed = 0;

	// Init GYR LEDs
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

	// Init Push Button
	req_PB.lines = 1;
	req_PB.lineoffsets[0] = 11;				 // pin 11 - Push button
	req_PB.flags = GPIOHANDLE_REQUEST_INPUT; // set it to be input

	data_PB.values[0] = 0;										  // set initial value of Push button to Low (OFF)
	ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_PB);				  // now get the line handler req_PB
	ioctl(req_PB.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data_PB); // read the value of the push button

	int i = 0;
	int initial_red_toggle_i = i;
	int max_button_push_count = 2;
	int current_button_push_count = 0;
	
	while (1)
	{
		ioctl(req_GYR.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_GYR); // output data bits
		data_GYR.values[0] = !data_GYR.values[0];						// toggle
		data_GYR.values[1] = !data_GYR.values[1];

		// Get the push button value
		ioctl(req_PB.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data_PB); // read the value of the push button
		if (data_PB.values[0] == 1 && !isButtonPressed)
		{	
			current_button_push_count++;
			if (current_button_push_count > max_button_push_count)
				break;
			isButtonPressed = 1;
			initial_red_toggle_i = i;
			data_GYR.values[2] = 0;
		}
		
		if (isButtonPressed) {
			data_GYR.values[2] = !data_GYR.values[2];
			if (i > initial_red_toggle_i + 10) {  // Toggle for 10 cycles
				isButtonPressed = 0;
			}
		}

		usleep(500000); // sleep for 0.5 second
		i++;
	}

	close(req_GYR.fd); // release line
	close(req_PB.fd);  // release line
	close(fd0);		   // close the file
	exit(0);
}
