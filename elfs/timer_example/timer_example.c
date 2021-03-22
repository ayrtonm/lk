// This is easier than fussing with the build system for now...
typedef unsigned int lk_time_t;

struct list_node {
    struct list_node *prev;
    struct list_node *next;
};

enum handler_return {
    INT_NO_RESCHEDULE = 0,
    INT_RESCHEDULE,
};

struct timer;
typedef enum handler_return (*timer_callback)(struct timer *, lk_time_t now, void *arg);

typedef struct timer {
    int magic;
    struct list_node node;

    lk_time_t scheduled_time;
    lk_time_t periodic_time;

    timer_callback callback;
    void *arg;
} timer_t;

extern int printf(const char *fmt, ...);
extern void timer_initialize(timer_t *);
extern void timer_set_oneshot(timer_t *, lk_time_t delay, timer_callback, void *arg);
extern void timer_set_periodic(timer_t *, lk_time_t period, timer_callback, void *arg);
extern void timer_cancel(timer_t *);
// Hopefully I'll be able to replace this with the usual header includes eventually

static timer_t clk;

static enum handler_return tick(struct timer *t, lk_time_t now, void *arg) {
    printf("tick\n");
    return INT_RESCHEDULE;
}

int main(int argc, char *argv[]) {
    timer_initialize(&clk);
    timer_set_oneshot(&clk, 1000, tick, 0);
    return 0;
}
