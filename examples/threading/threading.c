#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
	
	sleep(thread_func_args->wait_to_obtain_ms);
	
	
	printf("Thread %d: %s\n", thread_func_args->wait_to_obtain_ms, thread_func_args->wait_to_release_ms);
	
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
	bool success = true;
	thread_data *ptr_thread;
	ptr_thread = malloc(sizeof(thread_data));
	
	pthread_t thread_1;
	
	
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        success = false;
    }
    
    // Access shared resource here
	int rc = pthread_mutex_lock(&mutex);
	if(rc != 0)
	{
		printf("Mutex lockout failed ---------> \n");
		success = false;
	}
	else
	{
		ptr_thread->wait_to_obtain_ms = wait_to_obtain_ms;
		ptr_thread->wait_to_release_ms = wait_to_release_ms;
	}

	rc = pthread_mutex_unlock(&mutex);
	if(rc != 0)
	{
		printf("Mutex lockout failed ---------> \n");
		success = false;
	}
	if(success)
	{
		if ((pthread_create(&thread_1, NULL, threadfunc, (void *)thread_data)) != 0)
		{
			success = false;
		}
		else
		{
			thread = &thread_1;
		}
	}

    return success;
}

