#include <FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>
#include "thread_operations.h"

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

bool main_thread_function(int *counter, int *on, SemaphoreHandle_t semaphore)
{
    bool semaphore_pass = xSemaphoreTake(semaphore, 500);
    if (semaphore_pass)
    {
        printf("hello world from %s! Count %d\n", "main", (*counter)++);
        xSemaphoreGive(semaphore);
        *on = !(*on);
    }
    return semaphore_pass;
}

void thread_a_function(void *params)
{
    printf("Entered thread a.\n");
    struct DeadlockParams *dl_args = (struct DeadlockParams *)params;

    
    while(!xSemaphoreTake(dl_args->sem_a, 500));
    printf("Thread a acquired semaphore a.");
    dl_args->testvar += 1;
    vTaskDelay(500);
    while(!xSemaphoreTake(dl_args->sem_b, 500));
    dl_args->testvar += 1;
    xSemaphoreGive(dl_args->sem_b);
    xSemaphoreGive(dl_args->sem_a);

}

void thread_b_function(void *params)
{
    printf("Entered thread b.\n");
    struct DeadlockParams *dl_args = (struct DeadlockParams *)params;

    
    while(!xSemaphoreTake(dl_args->sem_b, 500));
    printf("Thread a acquired semaphore b.");
    dl_args->testvar += 1;
    vTaskDelay(500);
    while(!xSemaphoreTake(dl_args->sem_a, 500));
    dl_args->testvar += 1;
    xSemaphoreGive(dl_args->sem_a);
    xSemaphoreGive(dl_args->sem_b);
}