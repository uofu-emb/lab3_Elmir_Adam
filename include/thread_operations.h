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

void thread_deadlock_function(void *params);

bool orphaned_lock(int *counter, SemaphoreHandle_t semaphore);

bool not_orphaned_lock(int *counter, SemaphoreHandle_t semaphore);

#endif