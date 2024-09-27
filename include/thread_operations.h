#ifndef THREAD_OPERATIONS_H
#define THREAD_OPERATIONS_H

#include <FreeRTOS.h>
#include <semphr.h>

struct DeadlockParams {
    SemaphoreHandle_t sem_a;
    SemaphoreHandle_t sem_b;
    int testvar;
};

bool side_thread_function(int *counter, SemaphoreHandle_t semaphore);

bool main_thread_function(int *counter, int *on, SemaphoreHandle_t semaphore);

void thread_a_function(void *params);

void thread_b_function(void *params);

#endif