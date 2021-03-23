// This is easier than fussing with the build system for now...
// Hopefully I'll be able to replace this with the usual header includes eventually
typedef signed long int size_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef long long     off_t;

typedef int status_t;

typedef uintptr_t addr_t;
typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;

typedef unsigned int lk_time_t;

struct list_node {
    struct list_node *prev;
    struct list_node *next;
};

typedef struct arch_aspace {
    /* pointer to the translation table */
    paddr_t tt_phys;
    uint32_t *tt_virt;

    /* range of address space */
    vaddr_t base;
    size_t size;

    /* list of pages allocated for these page tables */
    struct list_node pt_page_list;
} arch_aspace_t;

typedef struct vmm_aspace {
    struct list_node node;
    char name[32];

    uint flags;

    vaddr_t base;
    size_t  size;

    struct list_node region_list;

    arch_aspace_t arch_aspace;
} vmm_aspace_t;

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
    vmm_aspace_t *aspace;
} timer_t;

