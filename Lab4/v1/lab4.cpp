#include <iostream>
#include <malloc.h>
#include <cstring>

uint8_t *heap = nullptr;
uint64_t heap_size;
uint64_t free_heap_size;
const uint64_t block_size = 1997;
const uint64_t max_block_size = 1997*9;

static void *(*old_malloc_hook)(size_t, const void *);

static void (*old_free_hook)(void *, const void *);

static void my_init_hook();

static void *my_malloc_hook(size_t, const void *);

static void my_free_hook(void *, const void *);

void (*__malloc_initialize_hook)() = my_init_hook;

static void my_init_hook() {
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
}

static void *my_malloc_hook(size_t size, const void *caller) {
    void *result = nullptr;
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
    if (size > max_block_size) {
        result = nullptr;
    }
    else {
        uint8_t *finder = heap;
        while (finder < heap+heap_size-size){
            uint32_t *range = (uint32_t*)finder;
            bool check = (*range & (1<<31));
            if (check == 1 /*добавить условие что в range в первых битах размер свободного блока больше size*/){
				free_heap_size -= size; 
				*range = size;
                result = range+1;
                finder += size+4;
                range = (uint32_t *) finder;
                *range = free_heap_size;
                break;
            }
            finder += *range+4;
        }
    }
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
    return result;
}

static void my_free_hook(void *ptr, const void *caller) {
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
    uint32_t *range = reinterpret_cast <uint32_t*>(ptr);
    range -= 1;
    memset(ptr, 0, *range);
    *range |= (1 << 31); 

    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
}

int main(int argc, char **argv) {
    heap_size = std::stoull(std::string(argv[1]));
    heap = reinterpret_cast<uint8_t *>(malloc(heap_size));;
    uint32_t *firstblock = (uint32_t *)heap;
    memset(heap, 0, sizeof(heap));
    *firstblock = heap_size;
    *firstblock |= (1 << 31);
    bool tmp = (*firstblock | (1 << 31));
    //std::cout << tmp << std::endl;
    my_init_hook();
    for (uint64_t i = 1; i < 10; ++i) {
        void *ptr = malloc(i * block_size);
        std::cout << ptr << std::endl;
        void *ptr2 = malloc(i * block_size);
        std::cout << ptr2 << std::endl;
        free(ptr);
        free(ptr2);
    }
    void *ptr = malloc(block_size*10);
    if (ptr == nullptr) {
        std::cout << "It was NULL" << std::endl;
    }
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
    return 0;
}
