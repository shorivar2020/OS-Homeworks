#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

/*To create the code, I was inspired by the code from https://arjunsreedharan.org/post/148675821737/write-a-simple-memory-allocator*/

static void *nbrk(void *address)
{
    void *current_brk = sbrk(0);
    if (address != NULL)
    {
        int ret = brk(address);
        if (ret == -1)
            return NULL;
    }
    return current_brk;
}

struct st_malloc
{
    unsigned long size;
    int free;
    struct st_malloc *next;
};

void *head = 0;

void *my_malloc(unsigned long size)
{
    if (size <= 0)
    {
        return (void *)0;
    }
    struct st_malloc *block;
    struct st_malloc *tail = head;
    struct st_malloc *cur = head;
    if (head)
    {
        while (cur && !(cur->free && cur->size >= size))
        {
            tail = cur;
            cur = cur->next;
        }
        block = cur;
        if (block)
        {
            if (block->size > size + sizeof(struct st_malloc))
            {
                struct st_malloc *new_block;
                new_block = (void *)((unsigned long)block + (size + sizeof(struct st_malloc)));
                new_block->size = block->size - size - sizeof(struct st_malloc);
                new_block->free = 1;
                block->size = size;
                new_block->next = block->next;
                block->next = new_block;
            }
            block->free = 0; // CPU FAILD
            return (void *)(block + 1);
        }
    }
    block = nbrk((void *)(size + sizeof(struct st_malloc) + (unsigned long)nbrk(0)));
    if (block == (void *)0)
    {
        return (void *)0;
    }
    if (block == (void *)-1)
    {
        return (void *)0;
    }
    if (!head)
    {
        head = block; 
    }
    if (tail)
    {
        tail->next = block;
    }
    block->size = size;
    block->next = 0;
    block->free = 0;
    return (void *)(block + 1);
}

int my_free(void *address)
{
    struct st_malloc *tmp = (struct st_malloc *)address - 1;
    if (!address || !tmp || tmp->free == 1 || tmp < (struct st_malloc *)head)
    {
        return 222;
    }
    tmp->free = 1;
    tmp = (struct st_malloc *)head;
    while (tmp)
    {
        if (tmp->next != 0)
        {
            if (tmp->free == 1 && tmp->next->free == 1)
            {
                tmp->size += tmp->next->size + sizeof(struct st_malloc);
                tmp->next->free = 0;
                tmp->next->size = 0;
                struct st_malloc *dop = tmp->next->next;
                tmp->next->next = 0;
                tmp->next = dop;
            }
        }
        tmp = tmp->next;
    }
    return 0;
}
