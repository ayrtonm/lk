#include "types.h"

extern int printf(const char *fmt, ...);
extern void timer_initialize(timer_t *);
extern void timer_set_oneshot(timer_t *, lk_time_t delay, timer_callback, void *arg);
extern void timer_set_periodic(timer_t *, lk_time_t period, timer_callback, void *arg);
extern void timer_cancel(timer_t *);

static enum handler_return tick(struct timer *t, lk_time_t now, void *arg) {
    printf("tick\n");
    return INT_RESCHEDULE;
}

timer_t clk;

int main(vmm_aspace_t *aspace) {
    timer_initialize(&clk);
    timer_set_oneshot(&clk, 1000, tick, 0);
    clk.aspace = aspace;
    return 0;
}
