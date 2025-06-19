#include "stimer.h"
#include "scheduler.h"
#include "util.h"

#include <stdlib.h>

#define STIMER_MAX_WORDS ((STIMER_MAX_TIMERS - 1)/32 + 1)

volatile struct {
    uint32_t init[STIMER_MAX_WORDS];
    uint32_t active[STIMER_MAX_WORDS];
    uint16_t task[STIMER_MAX_TIMERS];
    uint16_t cycles[STIMER_MAX_TIMERS];
    uint32_t period[STIMER_MAX_TIMERS];
    uint32_t countdown[STIMER_MAX_TIMERS];
} timers;

STimerStatus stimer_create(uint16_t *timer_id, const uint16_t task_id, const uint32_t period, const uint16_t cycles) {
    // find uninitialised timer
    size_t word = 0;
    for(; word<STIMER_MAX_WORDS && !~timers.init[word]; word++);
    // no free timers
    if(word == STIMER_MAX_WORDS) {
        return STIMER_TIMERS_FULL;
    }
    // find free bit
    uint8_t bit = binsearch32(~timers.init[word]);
    uint16_t id = (word<<5) | bit;
    *timer_id = id;

    // setup timer
    timers.init[word] |= 1U<<bit;
    timers.active[word] &= ~1U<<bit;
    timers.task[id] = task_id;
    timers.period[id] = period;
    timers.countdown[id] = period;
    timers.cycles[id] = cycles;

    return STIMER_OK;
}

STimerStatus stimer_start(const uint16_t timer_id) {
    if(timer_id >= STIMER_MAX_TIMERS) {
        return STIMER_BAD_ID;
    }
    if(!(timers.init[timer_id>>5] & 1U<<(timer_id&0x1F))) {
        return STIMER_BAD_ID;
    }
    // start timer
    timers.active[timer_id>>5] |= 1U<<(timer_id&0x1F);
}

STimerStatus stimer_stop(const uint16_t timer_id) {
    if(timer_id >= STIMER_MAX_TIMERS) {
        return STIMER_BAD_ID;
    }
    if(!(timers.init[timer_id>>5] & 1U<<(timer_id&0x1F))) {
        return STIMER_BAD_ID;
    }
    // stop timer
    timers.active[timer_id>>5] &= ~(1U<<(timer_id&0x1F));
}

STimerStatus stimer_delete(const uint16_t timer_id) {
    if(timer_id >= STIMER_MAX_TIMERS) {
        return STIMER_BAD_ID;
    }
    if(!(timers.init[timer_id>>5] & 1U<<(timer_id&0x1F))) {
        return STIMER_BAD_ID;
    }
    // delete timer
    timers.init[timer_id>>5] &= ~(1U<<(timer_id&0x1F));
    timers.active[timer_id>>5] &= ~(1U<<(timer_id&0x1F));
}

void stimer_tick() {
    // iterate over the active timers
    uint16_t id = 0;
    for(size_t wordi=0; wordi<STIMER_MAX_WORDS; wordi++) {
        const uint32_t word = timers.active[wordi];
        for(uint32_t mask=1UL; mask; mask<<=1) {
            if(mask&word) {
                // timer active, tick
                timers.countdown[id]--;
                if(timers.countdown[id] == 0) {
                    // timer done, set task
                    scheduler_set_task(timers.task[id]);
                    // if cycles not up then reset, else delete
                    if(timers.cycles[id] != 1) {
                        // decrement if cycles is used
                        if(timers.cycles[id] > 0) {
                            timers.cycles[id]--;
                        }
                        // reset timer
                        timers.countdown[id] = timers.period[id];
                    } else {
                        // cycles finished, delete timer
                        stimer_delete(id);
                    }
                }
            }
            id++;
        }
    }
}