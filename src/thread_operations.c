#include <FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

#include "thread_operations.h"

bool side_thread_function(int *counter, SemaphoreHandle_t semaphore)
{
    bool semaphore_pass = xSemaphoreTake(semaphore, 500);
    if(semaphore_pass)
    {
        *counter = *counter + 1;
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

void thread_deadlock_function(void *params)
{
    struct DeadlockParams *dl_args = (struct DeadlockParams *)params;

    while(!xSemaphoreTake(dl_args->sem_a, 500));
    dl_args->testvar += 1;
    vTaskDelay(500);
    while(!xSemaphoreTake(dl_args->sem_b, 500));
    dl_args->testvar += 1;
    xSemaphoreGive(dl_args->sem_b);
    xSemaphoreGive(dl_args->sem_a);

}

bool orphaned_lock(int *counter, SemaphoreHandle_t semaphore)
{
    bool semaphore_pass = xSemaphoreTake(semaphore, 500);
    if(semaphore_pass)
    {
        (*counter)++;
        if (*counter % 2) {
            return semaphore_pass;
        }
        printf("Count %d\n", counter);
        xSemaphoreGive(semaphore);
    }
   
    return semaphore_pass;
}

bool not_orphaned_lock(int *counter, SemaphoreHandle_t semaphore)
{
    printf("Inside not_orophaned_lock.");
    bool semaphore_pass = xSemaphoreTake(semaphore, 500);
    if(semaphore_pass)
    {
        printf("Acquired semaphore in not_orphaned_lock.");
        (*counter)++;
        if (!(*counter % 2)) {
            printf("Count %d\n", counter);
        }
        xSemaphoreGive(semaphore);
    }
   
    return semaphore_pass;
}
