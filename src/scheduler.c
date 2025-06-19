#include "scheduler.h"

#include "cmsis_compiler.h"

#include <stdlib.h>

#define SCHEDULER_FLAG_WORDS ((SCHEDULER_MAX_TASKS - 1)/32 + 1)

void (*scheduler_tasks[SCHEDULER_MAX_TASKS])();
uint16_t scheduler_next_id;
volatile uint32_t scheduler_flags[SCHEDULER_FLAG_WORDS];

SchedulerStatus scheduler_register_task(void (*task)(), uint16_t *task_id) {
    // check there are slots available
    if(scheduler_next_id >= SCHEDULER_MAX_TASKS) {
        return SCHEDULER_TASKS_FULL;
    }

    // register the task
    scheduler_tasks[scheduler_next_id] = task;
    *task_id = scheduler_next_id;
    scheduler_next_id++;
    return SCHEDULER_OK;
}

SchedulerStatus scheduler_set_task(uint16_t task_id) {
    // check the task exists
    if(task_id >= scheduler_next_id) {
        return SCHEDULER_BAD_ID;
    }
    // set the flag (per word: LSB corresponds to lowest ID)
    scheduler_flags[task_id>>5] |= 1<<(task_id&0x1F);
    return SCHEDULER_OK;
}

void scheduler_run_next() {
    size_t word = 0;
    // Search flags atomically
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    // find the next task to run
    for(; word<SCHEDULER_FLAG_WORDS && !scheduler_flags[word]; word++);
    const uint32_t fword = scheduler_flags[word];
    // exit atomic
    __set_PRIMASK(primask);

    // no tasks set
    if(word == SCHEDULER_FLAG_WORDS) {
        return;
    }
    // binary search for the lowest set bit
    uint16_t bit = 0;
    bit += fword&0xFFFFU ? 0 : 16;
    bit += fword&(0xFFU<<bit) ? 0 : 8;
    bit += fword&(0xFU<<bit) ? 0 : 4;
    bit += fword&(0x3U<<bit) ? 0 : 2;
    bit += fword&(0x1U<<bit) ? 0 : 1;
    // construct the task id
    const uint16_t run_id = (word<<5) | bit;

    // run the task
    scheduler_tasks[run_id]();

    // reset the bit atomically
    primask = __get_PRIMASK();
    __disable_irq();
    scheduler_flags[word] &= ~(1U<<bit);
    __set_PRIMASK(primask);
}