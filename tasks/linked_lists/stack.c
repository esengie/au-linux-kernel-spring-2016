#include <linux/gfp.h>
#include <linux/slab.h>

#include "stack.h"

stack_entry_t* create_stack_entry(void *data)
{
    stack_entry_t* tmp = (stack_entry_t *)kmalloc(
        sizeof(stack_entry_t), GFP_KERNEL);
    if (!tmp)
        return tmp;
    tmp->data = data;
    return tmp;
}

void delete_stack_entry(stack_entry_t *entry)
{
    kfree(entry);
}

void stack_push(struct list_head *stack, stack_entry_t *entry)
{
    list_add(&(entry->lh), stack);
}

stack_entry_t* stack_pop(struct list_head *stack)
{
    if (!list_empty(stack))
    {
        stack_entry_t* tmp =  list_entry(stack->next, stack_entry_t, lh);
        list_del(stack->next);
        return tmp;
    }
    return NULL;
}
