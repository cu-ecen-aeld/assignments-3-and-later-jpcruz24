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
	thread_func_args->thread_complete_success = true;
	usleep((thread_func_args->wait_to_obtain_ms)*1000);
	
	int rc = pthread_mutex_lock(thread_func_args->mutex);
	if(rc != 0)
	{
		thread_func_args->thread_complete_success = false;
	}
	
	usleep((thread_func_args->wait_to_release_ms)*1000);
	
	rc = pthread_mutex_unlock(thread_func_args->mutex);
	if(rc != 0)
	{
		thread_func_args->thread_complete_success = false;
	}
	 
	//free(thread_func_args);
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
	struct thread_data *ptr_thread;
	ptr_thread = malloc(sizeof(struct thread_data));

	ptr_thread->wait_to_obtain_ms = wait_to_obtain_ms;
	ptr_thread->wait_to_release_ms = wait_to_release_ms;
	ptr_thread->mutex = mutex;
	
	if ((pthread_create(thread, NULL, threadfunc, (void *)ptr_thread)) != 0)
	{
		success = false;
		free(ptr_thread);
	}
	if((ptr_thread == NULL) && (ptr_thread->thread_complete_success == false))
	{
		success = false;
	}
	else
	{
	
	}
	//free(ptr_thread);
    return success;
}

