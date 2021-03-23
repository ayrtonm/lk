#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#include <lib/elf.h>
#include <lib/fs.h>

#include <kernel/thread.h>
#include <kernel/vm.h>

#include <loader.h>

#define ERR(ACTION) \
    do { \
        if (err < 0) { \
            printf("Hit error %d while " #ACTION "\n", err); \
            return 0; \
        } \
    } while (0);

const char *root_dir = "/mydrive/";
const char *elf_list_txt = "/mydrive/elfs.txt";
const char *fs_ty = "ext2";
const char *dev_name = "virtio0";

static filehandle *open_file(const char *path) {
    status_t err;
    filehandle *handle;
    err = fs_open_file(path, &handle);
    printf("%s\n", path);
    ERR(opening file);
    return handle;
}

static uint64_t file_size(filehandle *handle) {
    status_t err;
    struct file_stat stat;
    err = fs_stat_file(handle, &stat);
    ERR(getting ELF size);
    return stat.size;
}

static int run_isolated_ELF(const char *name) {
    status_t err;
    // Arbitrarily limit full path to 50 characters
    char *full_path = malloc(50);
    strcpy(full_path, root_dir);
    strcat(full_path, name);
    filehandle *elf = open_file(full_path);
    uint64_t file_sz = file_size(elf);

    // Create a new aspace
    vmm_aspace_t *aspace;
    err = vmm_create_aspace(&aspace, name, 0);
    ERR(creating aspace);

    // Allocate memory in the new aspace
    void *ptr = (void *)USER_ASPACE_BASE;
    err = vmm_alloc(aspace, "user", ROUNDUP(file_sz, PAGE_SIZE), &ptr, PAGE_SIZE_SHIFT, VMM_FLAG_VALLOC_SPECIFIC, ARCH_MMU_FLAG_PERM_USER);
    ERR(creating user region);

    // Set the aspace as active to load the ELF into it
    vmm_set_active_aspace(aspace);

    // read ELF into the new aspace
    err = fs_read_file(elf, (uint8_t *)USER_ASPACE_BASE, 0, file_sz);
    ERR(reading ELF);

    // Create ELF handle
    elf_handle_t elf_header;
    err = elf_open_handle_memory(&elf_header, (void *)USER_ASPACE_BASE, file_sz);
    ERR(creating ELF handle);

    // Parse the ELF header
    err = elf_load(&elf_header);
    ERR(parsing ELF header);

    // Create a new thread with ELF entry
    int (*entry)(int argc, char *argv[]) = (void *)elf_header.entry;
    thread_t *t = thread_create(name, entry, aspace, DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);

    // Modify thread aspace for `thread_resume` to use
    t->aspace = aspace;

    // Kick off the thread
    thread_detach(t);
    thread_resume(t);

    free(full_path);
    return 0;
}

int run_loader(void) {
    status_t err;
    err = fs_mount(root_dir, fs_ty, dev_name);
    ERR(mounting);
    printf("Opening ELF list...\n");
    filehandle *elf_list = open_file(elf_list_txt);
    uint64_t file_sz = file_size(elf_list);
    char *file_names = malloc(file_sz);
    printf("Reading ELF list...\n");
    err = fs_read_file(elf_list, (void *)file_names, 0, file_sz);
    ERR(reading ELF list);

    printf("Parsing ELF list...\n");
    const char newline = 0x0a;
    const char terminate = 0;
    char *ptr = file_names;
    // Iterate through file names in list
    while ((uint64_t)(ptr - file_names) < file_sz) {
        char *name = ptr;
        ptr = strchr(ptr, newline);
        // Make it a real C-style string
        *ptr = terminate;

        // Run the ELF in its own user address space
        run_isolated_ELF(name);

        ptr++;
    }
    free(file_names);
    return 0;
}
