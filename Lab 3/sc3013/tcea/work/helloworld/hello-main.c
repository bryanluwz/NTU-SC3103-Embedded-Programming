#include <stdio.h>
#include "hello.h"

int main() {
	int count = 0;
	printf("Hello world from main");
	while (1) {
		char c = getchar();
		helloprint();
		count = helloCount(count);
		
		if (c == 'q')
			break;
	}
	printf("BYE");
}
