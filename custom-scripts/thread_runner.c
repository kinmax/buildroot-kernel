#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>

volatile char* buffer;
int buffer_size, number_of_threads, priority, policy, *call_counter;
volatile int current_size;
sem_t mutex;

void *run(void *character)
{
	char my_char = (char)(long int)character;
	while(1)
	{
	    sem_wait(&mutex);
        if(current_size >= buffer_size)
	    {
	        sem_post(&mutex);
	        break;
	    }
	    buffer[current_size] = my_char;
	    current_size++;
	    call_counter[my_char-65]++;
	    sem_post(&mutex);
	}
	return(NULL);
}

void setpriority(pthread_t *thr, int newpolicy, int newpriority){
	int policy, ret;
    struct sched_param param;
    pthread_t t;
    if(newpriority > sched_get_priority_max(newpolicy) ||
	   newpriority < sched_get_priority_min(newpolicy)){
		printf("Invalid priority: MIN: %d, MAX: %d", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));
	}

    pthread_getschedparam(*thr, &policy, &param);
    
    param.sched_priority = newpriority;
    
    ret = pthread_setschedparam(*thr, newpolicy, &param);

    if (ret != 0){
		perror("perror(): ");
	}
}

int main(int argc, char **argv)
{
	if(argc < 5)
	{
		printf("usage: ./%s <number_of_threads> <global_buffer_size_in_kilobytes> <scheduling_policy> <priority>\n\n", argv[0]);
		exit(0);
	}
	
	number_of_threads = atoi(argv[1]);
	if(number_of_threads < 1)
	{
		printf("The number of specified threads must be an integer greater than 0. Got %d\n\n", number_of_threads);
		exit(0);
	}
	
	buffer_size = atoi(argv[2]);
	if(buffer_size < 1)
	{
		printf("The specified buffer size must be an integer greater than 0 (at least 1 kB). Got %d kB\n\n", buffer_size);
		exit(0);
	}
	buffer_size = buffer_size * 1024;
	
	char* string_policy = argv[3];
	
	priority = atoi(argv[4]);
	if(priority < 0)
	{
		printf("The specified priority must be at least 0. Got %d\n\n", priority);
	}
	
	
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
		printf("Invalid scheduling policy: %s\nPolicy should be one of these: SCHED_FIFO SCHED_RR SCHED_OTHER\n\n", string_policy);
		exit(0);
	}
	
	long int character = 65;
	int i;
	pthread_t threads[number_of_threads];
	current_size = 0;
	sem_init(&mutex, 0, 1);
	call_counter = malloc(number_of_threads * sizeof(int));
	buffer = malloc(buffer_size * sizeof(char));
	for(i = 0; i < number_of_threads; i++)
	{
	    call_counter[i] = 0;
	    pthread_create(&threads[i], NULL, run, (void *)character);		
	    setpriority(&threads[i], policy, priority);
	    character++;
	}
	for(i = 0; i < number_of_threads; i++)
	{
	    pthread_join(threads[i], NULL);
	}
	sem_destroy(&mutex);
    printf("%s\n\n", buffer);
    
    for(i = 0; i < buffer_size; i++)
    {
        if(i == 0)
        {
            printf("%c", buffer[i]);
        }
        else
        {
            if(buffer[i] != buffer[i-1])
            {
                printf("%c", buffer[i]);
            }
        }
    }
    printf("\n\n");
    for(i = 0; i < number_of_threads; i++)
    {
        printf("%c - %d\n", i+65, call_counter[i]);
    }
    
	return(0);	
}

