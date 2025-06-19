#include "framework.h"

#include "scheduler.h"
#include "stimer.h"

// Tests
int test_stimer_add();
int test_stimer_period();
int test_stimer_pause();
int test_stimer_cycles();
int test_stimer_delete();

// Timers and tasks
volatile int task0_ticks, task1_ticks;
void task0() { task0_ticks++; }
void task1() { task1_ticks++; }
uint16_t task0_id;
uint16_t task1_id;
uint16_t timer0_id;
uint16_t timer1_id;

int main() {
    test_register(&test_stimer_add);
    test_register(&test_stimer_period);
    test_register(&test_stimer_pause);
    test_register(&test_stimer_cycles);
    test_register(&test_stimer_delete);

    test_run_all();

    return 0;
}

int test_stimer_add() {
    scheduler_register_task(&task0, &task0_id);
    STimerStatus status = stimer_create(&timer0_id, task0_id, 4, 0);
    tassertf(status==STIMER_OK, "Failed to create timer0");

    scheduler_register_task(&task1, &task1_id);
    status = stimer_create(&timer1_id, task1_id, 10, 4);
    tassertf(status==STIMER_OK, "Failed to create timer1");

    tassertf(timer0_id!=timer1_id, "Timers assigned same ID");

    stimer_start(timer0_id);

    return 0;
}

int test_stimer_period() {
    task0_ticks = 0;
    for(int i=0; i<3; i++) {
        stimer_tick();
        scheduler_run_next();
    }
    tassertf(task0_ticks==0, "Timer ticked too early");
    stimer_tick();
    scheduler_run_next();
    tassertf(task0_ticks==1, "Timer ticked too late");

    for(int i=0; i<20; i++) {
        stimer_tick();
        scheduler_run_next();
    }
    tassertf(task0_ticks==6, "Timer period incorrect");

    return 0;
}

int test_stimer_pause() {
    task0_ticks = 0;

    stimer_stop(timer0_id);
    for(int i=0; i<20; i++) {
        stimer_tick();
        scheduler_run_next();
    }
    tassertf(task0_ticks==0, "Timer did not stop");

    stimer_start(timer0_id);
    for(int i=0; i<20; i++) {
        stimer_tick();
        scheduler_run_next();
    }
    tassertf(task0_ticks==5, "Timer did not start");

    return 0;
}

int test_stimer_cycles() {
    stimer_start(timer1_id);
    stimer_stop(timer0_id);
    task1_ticks = 0;

    for(int i=0; i<100; i++) {
        stimer_tick();
        scheduler_run_next();
    }
    tassertf(task1_ticks==4, "Timer did not run for the correct number of cycles (%d instead of 4)", task1_ticks);

    return 0;
}

int test_stimer_delete() {
    stimer_start(timer0_id);
    stimer_delete(timer0_id);
    task0_ticks = 0;

    for(int i=0; i<100; i++) {
        stimer_tick();
        scheduler_run_next();
    }
    tassertf(task0_ticks==0, "Deleted timer still ticking");

    STimerStatus status = stimer_start(timer0_id);
    tassertf(status==STIMER_BAD_ID, "Deleted timer successfully starts");

    status = stimer_delete(timer0_id);
    tassertf(status==STIMER_BAD_ID, "Deleted timer twice");

    return 0;
}
