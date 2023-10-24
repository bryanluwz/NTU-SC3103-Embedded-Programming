/**
 * @file pthread1.c
 * This program creates two threads that will each print out a message onto the screen.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message(void *ptr)
{
	char *text;
	text = (char *)ptr;
	printf("%s \n", text);
	return NULL;
}

int main()
{
	pthread_t thread1, thread2;
	char *str1 = "I am thread 1!";
	char *str2 = "I am thread 2!";

	int T1ret, T2ret;

	// Create two threads
	T1ret = pthread_create(&thread1, NULL, print_message, (void *)str1);
	T2ret = pthread_create(&thread2, NULL, print_message, (void *)str2);

	// Main waits for both thread to finish
//	pthread_join(thread1, NULL);
//	pthread_join(thread2, NULL);
	
	printf("Thread 1 and 2 returns: %d, %d \n", T1ret, T2ret);
	sleep(1);
	return 0;
}
