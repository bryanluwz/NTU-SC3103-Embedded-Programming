/**
 * @file pthread2.c
 * This program creates two threads that will each print out the global variable after incrementing it using mutex.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Mutexes
pthread_mutex_t mutexA;

// Global variable
int g_var1 = 0;

void *inc_gv()
{
	int i, j;
	for (i = 0; i < 10; i++)
	{
		pthread_mutex_lock(&mutexA);
		
		g_var1++; // increment the global variable
		for (j = 0; j < 5000000; j++)
			; // delay loop
		printf("%d ", g_var1);
		fflush(stdout);
		
		pthread_mutex_unlock(&mutexA);
		
		for (j = 0; j < 5000000; j++)
			; // delay loop
	}
}

int main()
{
	pthread_t thread1, thread2;
	pthread_mutex_init(&mutexA, NULL);

	int T1ret, T2ret;

	// Create two threads
	T1ret = pthread_create(&thread1, NULL, inc_gv, NULL);
	T2ret = pthread_create(&thread2, NULL, inc_gv, NULL);

	// Main waits for both thread to finnish
	pthread_join(thread1, NULL);
	printf("Thread 1 done! \n");
	pthread_join(thread2, NULL);
	printf("Thread 2 done! \n");

	printf("Thread 1 and 2 returns: %d, %d \n", T1ret, T2ret);
	return 0;
}
