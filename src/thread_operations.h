#include <FreeRTOS.h>
#include <semphr.h>

bool side_thread_function(int *counter, SemaphoreHandle_t semaphore);