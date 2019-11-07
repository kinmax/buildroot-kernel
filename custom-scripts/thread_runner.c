#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>

char* buffer;

int main(int argc, char **argv)
{
	if(argc < 5)
	{
		printf("usage: ./%s <number_of_threads> <global_buffer_size_in_kilobytes> <scheduling_policy> <priority>\n\n", argv[0]);
		exit(0);
	}
	
	int threads = strtol(argv[1]);
	if(threads < 1)
	{
		printf("The number of specified threads must bean integer greater than 0. Got %d\n\n", threads);
		exit(0);
	}
	
	int buffer_size = strtol(argv[2]);
	if(buffer_size < 1)
	{
		printf("The specified buffer size must be an integer greater than 0 (at least 1 kB). Got %d kB\n\n", buffer_size);
		exit(0)
	}
	
	char* string_policy = argv[3];
	
	int priority = strtol(argv[4]);
	if(priority < 0)
	{
		printf("The specified priority must be at least 0. Got %d\n\n", priority);
	}
	
	int policy;
	
	if(strcmp(string_policy, "SCHED_FIFO") == 0)
	{
		policy = SCHED_FIFO;
	}
	else if(strcmp(string_policy, "SCHED_RR") == 0)
	{
		policy = SCHED_RR;
	}
	else if(strcmp(string_policy, "SCHED_OTHER") == 0)
	{
		policy = SCHED_OTHER;
	}
	else
	{
		printf("Invalid scheduling policy: %s\nPolicy should be one of these: SCHED_FIFO SCHED_RR SCHED OTHER\n\n", string_policy);
		exit(0);
	}
	
}
