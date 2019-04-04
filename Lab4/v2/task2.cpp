#include <iostream>
#include <list>
#include <stdio.h>
#include <malloc.h>
#include <iomanip>

using namespace std;
typedef unsigned char Byte;

Byte* heap = nullptr;

const int block_size = 1998;

struct block {
    Byte* ptr;
    int size;
    bool flag;
};

list<block> all_blocks;

static void* (*old_malloc_hook)(size_t, const void*);

static void (*old_free_hook)(void*, const void*);

static void my_init_hook();

static void* my_malloc_hook(size_t, const void*);

static void my_free_hook(void*, const void*);

void (*__malloc_initialize_hook)() = my_init_hook;

static void my_restore_hook() {
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
}

static void my_init_hook()
{
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
}

int check_block(int size)
{
    if (size > (7 * block_size) || size == 0)
        return 0;

    int new_size = block_size;
    while (size > new_size)
        new_size += block_size;
    return new_size;
}

static void* my_malloc_hook(size_t size, const void* caller)
{
    void* result;
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
    int temp = 0;
    int new_size = 0, step = 0;
    for (list<block>::iterator it = all_blocks.begin(); it != all_blocks.end(); it++, step++) {
        new_size = check_block(size);
        if (new_size == 0)
            return nullptr;
        if (it->size >= new_size && it->flag == 0) {
            block new_b;
            new_b.ptr = it->ptr + new_size;
            new_b.size = it->size - new_size;
            new_b.flag = 0;
            it->size = new_size;
            it->flag = 1;
            result = it->ptr;
            if (new_b.size >= 1)
                all_blocks.push_back(new_b);
            break;
        }
    }
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
    return result;
}

static void my_free_hook(void* ptr, const void* caller)
{
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
    list<block>::iterator it;
    list<block>::iterator pred;

    for (it = all_blocks.begin(); it != all_blocks.end(); it++) {
        if (it->ptr == (Byte*)ptr) {
            it->flag = 0;
            break;
        }
    }
    it = all_blocks.begin();
    it++;

    for (it, pred = all_blocks.begin(); it != all_blocks.end(); it++, pred++) {
        if (it->flag == 0 && pred->flag == 0) {
            pred->size += it->size;
            all_blocks.erase(it);
            it--;
            pred--;
        }
    }   
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
}

int main(int argc, char** argv)
{
    const int heap_size = stoull(string(argv[1]));
    heap = (Byte*)(malloc(heap_size));
    block head;
    head.ptr = heap;
    head.size = heap_size;
    head.flag = 0;
    all_blocks.push_back(head);
    
    list<block>::iterator it = all_blocks.begin();
    my_init_hook();
    
    while (1) {
        my_restore_hook();
        cout << "==========================" << endl;
        cout << "Menu. Heap size: " << heap_size << endl;
        cout << "1. Allocate" << endl;
        cout << "2. Free" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter command: ";
        int order = 0;
        cin >> order;
        cout << endl;
        my_init_hook();
        if (order == 1) {
            int in_size = 0;
            cout << "Write block size: ";
            my_restore_hook();
            cin >> in_size;
            cout << endl;
            my_init_hook();
            void* ptr = malloc(in_size);
        } else if (order == 2) {
            int index = 0;
            cout << "Choose, which block you want to delete: ";
            cin >> index;
            index--;
            list<block>::iterator it = all_blocks.begin();
            advance(it, index);
            free((void*)it->ptr);
        } else if (order == 0) {
            return 0;
        }
        int list_index = 1;
        cout << "Blocks list: " << endl;
        cout << "№  Size     IsFree" << endl;
        for (list<block>::iterator it = all_blocks.begin(); it != all_blocks.end(); it++)
            cout << list_index++ << ") " << left << setw(9) << it->size << boolalpha << !it->flag << endl;
        cout << endl;
    }
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
    return 0;
}
