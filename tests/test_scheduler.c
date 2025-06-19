#include "framework.h"
#include "scheduler.h"

// tests
int test_task_reg_normal();
int test_task_reg_overflow();
int test_task_run();

// tasks
int task0, task1, task2;
uint16_t task_id0, task_id1, task_id2;
void test_task0() { task0++; }
void test_task1() { task1++; }
void test_task2() { task2++; }

int main() {
    test_register(*test_task_reg_normal);
    test_register(*test_task_reg_overflow);
    test_register(*test_task_run);

    test_run_all();

    return 0;
}

int test_task_reg_normal() {
    tassertf(scheduler_register_task(test_task0, &task_id0)==SCHEDULER_OK, "Task registration 0 failed");
    tassertf(scheduler_register_task(test_task1, &task_id1)==SCHEDULER_OK, "Task registration 1 failed");
    tassertf(scheduler_register_task(test_task2, &task_id2)==SCHEDULER_OK, "Task registration 2 failed");
}

int test_task_reg_overflow() {
    uint16_t temp_id;
    for(int i=3; i<SCHEDULER_MAX_TASKS; i++) {
        tassertf(scheduler_register_task(test_task0, &temp_id)==SCHEDULER_OK, "Task registration failed without full registry");
    }
    tassertf(scheduler_register_task(test_task0, &temp_id)==SCHEDULER_TASKS_FULL, "Task registration succeded with full registry");

    return TEST_SUCCESS;
}

int test_task_run() {
    // Preconditions
    tassertf(task0==0 && task1==0 && task2==0, "Tasks run before set");

    // Run normally
    scheduler_set_task(task_id0);
    scheduler_run_next();
    tassertf(task0==1, "Task 0 did not run");
    scheduler_set_task(task_id1);
    scheduler_run_next();
    tassertf(task1==1, "Task 1 did not run");
    scheduler_set_task(task_id2);
    scheduler_run_next();
    tassertf(task2==1, "Task 2 did not run");

    return TEST_SUCCESS;
}

int test_task_priorities() {
    scheduler_set_task(task_id2);
    scheduler_set_task(task_id1);
    scheduler_set_task(task_id0);
    task0 = 0;
    task1 = 0;
    task2 = 0;
    scheduler_run_next();
    tassertf(task0==1, "Task 0 was not run in order");
    scheduler_run_next();
    tassertf(task1==1, "Task 1 was not run in order");
    scheduler_run_next();
    tassertf(task2==1, "Task 2 was not run in order");

    return TEST_SUCCESS;
}