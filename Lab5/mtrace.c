#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>

static void* (*OldMalloc)(size_t) = NULL;
static void (*OldFree)(void* ptr) = NULL;

int AmmountOfBlocks = 0;

static void mtrace_malloc_init()
{
    OldMalloc = dlsym(RTLD_NEXT, "malloc");
}

static void mtrace_free_init()
{
    OldFree = dlsym(RTLD_NEXT, "free");
}

void* malloc(size_t size)
{
    if (OldMalloc == NULL) {
        mtrace_malloc_init();
    }
    if (AmmountOfBlocks >= 77) {
        return NULL;
    }
    ++AmmountOfBlocks;
    return OldMalloc(size);
}

void free(void* ptr)
{
    if (OldFree == NULL) {
        mtrace_free_init();
    }
    if (AmmountOfBlocks >= 0) {
        fprintf(stderr, "%d\n", AmmountOfBlocks);
        --AmmountOfBlocks;
        OldFree(ptr);
    }
}
