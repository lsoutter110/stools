#ifndef STIMER_H
#define STIMER_H

#include <stdint.h>

#define STIMER_MAX_TIMERS 32

enum STimerStatus_Tag {
    STIMER_OK,
    STIMER_TIMERS_FULL,
    STIMER_BAD_ID,
};
typedef enum STimerStatus_Tag STimerStatus;

STimerStatus stimer_create(uint16_t *timer_id, const uint16_t task_id, const uint32_t period, const uint16_t cycles);
STimerStatus stimer_start(const uint16_t timer_id);
STimerStatus stimer_stop(const uint16_t timer_id);
STimerStatus stimer_delete(const uint16_t timer_id);
void stimer_tick();

#endif