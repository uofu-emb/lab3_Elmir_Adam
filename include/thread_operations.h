#ifndef THREAD_OPERATIONS_H
#define THREAD_OPERATIONS_H

#include <FreeRTOS.h>
#include <semphr.h>

bool side_thread_function(int *counter, SemaphoreHandle_t semaphore);

bool main_thread_function(int *counter, int *on, SemaphoreHandle_t semaphore);

#endif