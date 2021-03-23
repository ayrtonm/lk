#include "types.h"

extern void thread_yield(void);
extern void thread_sleep(lk_time_t delay);
extern int printf(const char *fmt, ...);

int main(vmm_aspace_t *aspace) {
    printf("Hello from user space!\n");
    thread_sleep(5000);
    printf("This is also from the hello_world ELF\n");
    return 0;
}
