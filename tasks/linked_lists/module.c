#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "stack.h"
#include "assert.h"

static int __init test_stack(void)
{
    // Должна бы возвращать ошибку но не менял
    LIST_HEAD(data_stack);
    stack_entry_t *tos = NULL;
    const char *tos_data = NULL;
    const char* test_data[] = { "1", "2", "3", "4" };
    long i = 0;

    pr_alert("Testing basic stack");

    for (i = 0; i != ARRAY_SIZE(test_data); ++i) {
        stack_push(&data_stack,
            create_stack_entry((void*)test_data[i])
        );
    }

    for (i = ARRAY_SIZE(test_data) - 1; i >= 0; --i) {
        tos = stack_pop(&data_stack);
        tos_data = STACK_ENTRY_DATA(tos, const char*);
        delete_stack_entry(tos);
        printk(KERN_ALERT "%s == %s\n", tos_data, test_data[i]);
        assert(!strcmp(tos_data, test_data[i]));
    }

    assert(stack_empty(&data_stack));
    return 0;
}

static int __init print_processes_backwards(void)
{
    int ret = 0;
    LIST_HEAD(stack);
    struct task_struct *task;
    stack_entry_t *tos = NULL;
    char* buf = NULL;
    for_each_process(task) 
    {
        buf = (char *)kmalloc(sizeof(task->comm), GFP_KERNEL);
        if (!buf){
            ret = -ENOMEM;
            break;
        }
        buf = get_task_comm(buf, task);
        tos = create_stack_entry((void*)buf);
        if (!tos){
            kfree(buf);
            ret = -ENOMEM;
            break;
        }
        stack_push(&stack, tos);
    }
    while (!stack_empty(&stack))
    {
        tos = stack_pop(&stack);
        buf = STACK_ENTRY_DATA(tos, char*);
        delete_stack_entry(tos);
        printk(KERN_ALERT "%s\n", buf);
        kfree(buf);
    }
    return ret;
}

static int __init ll_init(void)
{
    int err = 0;
    printk(KERN_ALERT "Hello, linked_lists\n");
    err = test_stack();
    if (err)
        goto error;
    err = print_processes_backwards();
error:
    return err;
}

static void __exit ll_exit(void)
{
    printk(KERN_ALERT "Goodbye, linked_lists!\n");
}

module_init(ll_init);
module_exit(ll_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list exercise module");
MODULE_AUTHOR("Kernel hacker!");
