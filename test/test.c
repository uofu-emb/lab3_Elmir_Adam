#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include "thread_operations.h"

#define TEST_RUNNER_PRIORITY (tskIDLE_PRIORITY + 5UL)

void setUp(void) {}

void tearDown(void) {}

void test_main_thread_function(void)
{
    SemaphoreHandle_t semaphore;
    semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 4;
    int on = 1;
    TEST_ASSERT_TRUE_MESSAGE(main_thread_function(&counter, &on, semaphore) == pdTRUE, "Function failed to acquire semaphore.");
    TEST_ASSERT_TRUE_MESSAGE(counter == 5, "Thread failed to increment counter.");
    TEST_ASSERT_TRUE_MESSAGE(on == 0, "Thread failed to toggle on.");
}

void test_side_thread_function(void)
{
    SemaphoreHandle_t semaphore;
    semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 4;
    TEST_ASSERT_TRUE_MESSAGE(side_thread_function(&counter, semaphore) == pdTRUE, "Function failed to acquire semaphore.");
    TEST_ASSERT_TRUE_MESSAGE(counter == 9, "Thread failed to increment counter.");
}

void test_side_thread_semaphore(void)
{
    SemaphoreHandle_t semaphore;
    semaphore = xSemaphoreCreateCounting(1, 1);
    xSemaphoreTake(semaphore, 500);
    int counter = 4;
    TEST_ASSERT_TRUE_MESSAGE(side_thread_function(&counter, semaphore) == pdFALSE, "Function acquired semaphore when it should not have.");
    TEST_ASSERT_TRUE_MESSAGE(counter == 4, "Thread changed counter when it should not have.");
}

void test_main_thread_semaphore(void)
{
    SemaphoreHandle_t semaphore;
    semaphore = xSemaphoreCreateCounting(1, 1);
    xSemaphoreTake(semaphore, 500);
    int counter = 4;
    int on = 1;
    TEST_ASSERT_TRUE_MESSAGE(main_thread_function(&counter, &on, semaphore) == pdFALSE, "Function acquired semaphore when it should not have.");
    TEST_ASSERT_TRUE_MESSAGE(counter == 4, "Thread changed counter when it should not have.");
    TEST_ASSERT_TRUE_MESSAGE(on == 1, "Thread toggled on when it should not have.");
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_main_thread_function);
    RUN_TEST(test_side_thread_function);
    RUN_TEST(test_main_thread_semaphore);
    RUN_TEST(test_side_thread_semaphore);
    sleep_ms(5000);
    return UNITY_END();
}
