/**
 * @file client.c
 * This program creates a client that will spam the server I think.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

#define THREAD_COUNT 3


struct hostent *server;
int port;

void error(char *error_message)
{
	perror(error_message);
	exit(0);
}

/**
 * @brief This function handles the client connection.
 *
 * @param sockfd
 */
void handle_input()
{
	char buffer[256];
	int n;
	struct sockaddr_in serv_addr;

	// Create sockfd
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		error("Error opening socket :/\n");
	}

	// Clear server address structure for a clean start
	memset(((char *)&serv_addr), 0, sizeof(serv_addr));

	// Set address family, copy server address to server address structure, set port number
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	// Connect, and start sending and reading message
	if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Error connecting :/\n");
	}

	// Print a random number between 0 and 100 every time, with random seed

	int number = (rand() * getpid()) % 100;
	sprintf(buffer, "%d\0", number);
	printf("Sending %s to server\n", buffer);

	// Input is valid, send to server
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
	{
		error("Error writing to socket :/\n");
	}

	n = read(sockfd, buffer, 255);
	// Get the number part of n because n for some reason has a bunch of weird characters
	buffer[n] = '\0';

	if (n < 0)
	{
		error("Error reading from socket :/\n");
	}
	printf("Server response to number %d: %s\n", number, buffer);
	
	close(sockfd);
}

int main(int argc, char *argv[])
{
	// Initialise and stuff
	int n;
	char buffer[256];
	srand(time(NULL));
	
	if (argc < 3)
	{
		error("Usage: client [hostname] [port]\n");
	}

	// Create socket and port, and initialise the server
	port = atoi(argv[2]);

	server = gethostbyname(argv[1]);

	if (server == NULL)
	{
		error("Error, no such host :/\n");
	}
	
	/*// Create sockfd
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// Clear server address structure for a clean start
	memset(((char *)&serv_addr), 0, sizeof(serv_addr));

	// Set address family, copy server address to server address structure, set port number
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	// Connect, and start sending and reading message
	if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Error connecting :/\n");
	}*/

	// Fork into three child processes to handle user input
	int i;
	for (i = 0; i < THREAD_COUNT; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			handle_input();
			exit(0);
		}

		usleep(100000);
	}

	// Wait for child processes to finish
	for (i = 0; i < THREAD_COUNT; i++)
	{
 		wait(NULL);
	}

	return 0;
}
