#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/init.h> /* Needed for the macros */
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fs_struct.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Your module description");

int g_time_interval = 60000;
struct timer_list my_timer;

void my_timer_callback(struct timer_list *unused)
{
    /* Restarting the timer... */
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(g_time_interval));

    printk(KERN_INFO "Timer Handler called.\n");
}

static int __init my_init(void)
{
    printk(KERN_INFO "My module inserted into kernel!!!.\n");

    /*Starting the timer.*/
    timer_setup(&my_timer, my_timer_callback, 0);
    mod_timer( &my_timer, jiffies + msecs_to_jiffies(g_time_interval));

    return 0;
}

static void __exit my_exit(void)
{
    del_timer(&my_timer);
    printk(KERN_INFO "My module exited from kernel!!!\n");
}

module_init(my_init);
module_exit(my_exit);