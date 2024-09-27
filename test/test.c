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
#define TEST_THREAD_A_PRIORITY (TEST_RUNNER_PRIORITY - 1UL)
#define TEST_THREAD_B_PRIORITY (TEST_RUNNER_PRIORITY - 1UL)
#define TEST_THREAD_STACK_SIZE (configMINIMAL_STACK_SIZE)

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

void test_deadlock(void)
{
    TaskHandle_t thread_a, thread_b;
    SemaphoreHandle_t semaphore_a, semaphore_b;

    semaphore_a = xSemaphoreCreateCounting(1, 1);
    semaphore_b = xSemaphoreCreateCounting(1, 1);

    struct DeadlockParams deadlock_params_a = {semaphore_a, semaphore_b, 0};
    struct DeadlockParams deadlock_params_b = {semaphore_a, semaphore_b, 0};
    
    printf("Creating threads a and b.\n");
    xTaskCreate(thread_a_function, "ThreadA",
                TEST_THREAD_STACK_SIZE, (void *)&deadlock_params_a, TEST_THREAD_A_PRIORITY, &thread_a);
    printf("Created thread a.\n");
    xTaskCreate(thread_b_function, "ThreadB",
                TEST_THREAD_STACK_SIZE, (void *)&deadlock_params_b, TEST_THREAD_B_PRIORITY, &thread_b);

    printf("Waiting for threads a and b to deadlock. . .\n");
    vTaskDelay(1000);
    //vTaskSuspend(thread_a);
    //vTaskSuspend(thread_b);

    printf("Threads a and b suspended. Testing variables. . .\n");
    TEST_ASSERT_TRUE_MESSAGE(uxSemaphoreGetCount(semaphore_a) == 0, "Semaphore A did not cause a deadlock.");
    TEST_ASSERT_TRUE_MESSAGE(uxSemaphoreGetCount(semaphore_b) == 0, "Semaphore B did not cause a deadlock.");
    TEST_ASSERT_FALSE_MESSAGE(deadlock_params_a.testvar == 0, "Thread a did not acquire semaphore a.");
    TEST_ASSERT_FALSE_MESSAGE(deadlock_params_a.testvar == 2, "Deadlock did not occur; test variable in thread a was incremented twice.");
    TEST_ASSERT_FALSE_MESSAGE(deadlock_params_b.testvar == 0, "Thread b did not acquire semaphore b.");
    TEST_ASSERT_FALSE_MESSAGE(deadlock_params_b.testvar == 2, "Deadlock did not occur; test variable in thread b was incremented twice.");

    printf("Killing threads a and b.\n");
    vTaskDelete(thread_a);
    vTaskDelete(thread_b);
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
    RUN_TEST(test_deadlock);
    sleep_ms(5000);
    return UNITY_END();
}
