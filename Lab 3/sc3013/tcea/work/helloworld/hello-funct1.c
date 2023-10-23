#include <stdio.h>
#include "hello.h"

void helloprint() {
	printf("Hello World from funct1!");
	for (int i = 0; i < 0x6ffffff; i++);
}
