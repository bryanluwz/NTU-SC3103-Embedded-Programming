#include <stdio.h>
#include "hello.h"

int helloCount(int count) {
	printf("Hello World from funct2!\n");
	count++;
	
	for (int i = 0; i < 0x7ffffff; i++);
	
	return count;
}
