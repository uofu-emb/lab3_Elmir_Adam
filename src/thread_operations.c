#include <FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>


bool side_thread_function(int *counter, SemaphoreHandle_t semaphore)
{
    bool semaphore_pass = xSemaphoreTake(semaphore, 500);
    if(semaphore_pass)
    {
        *counter += *counter + 1;
        xSemaphoreGive(semaphore);
	    printf("hello world from %s! Count %d\n", "thread", *counter);
    }
   
    return semaphore_pass;
}