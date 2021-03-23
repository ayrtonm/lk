#include "types.h"

extern int printf(const char *fmt, ...);

int main(vmm_aspace_t *aspace) {
    printf("This is the second example!\n");
    return 0;
}
