#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define SCHEDULER_MAX_TASKS 256

enum SchedulerStatus_Tag {
    SCHEDULER_OK,
    SCHEDULER_TASKS_FULL,
    SCHEDULER_BAD_ID,
};
typedef enum SchedulerStatus_Tag SchedulerStatus;

SchedulerStatus scheduler_register_task(void (*task)(), uint16_t *task_id);
SchedulerStatus scheduler_set_task(uint16_t task_id);
void scheduler_run_next();

#endif
