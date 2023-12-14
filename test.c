#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/init.h> /* Needed for the macros */
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fs_struct.h>
#include <linux/ktime.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Your module description");

int g_time_interval = 59300;
struct timer_list my_timer;
static char my_file_path[] = "/tmp/current_time";


struct current_time{
	int hours;
	int minutes;
};

struct current_time time_now_func(void){
	struct current_time time_now;
	// получаем текущее время
	ktime_t current_time = ktime_get_real();
	s64 seconds = ktime_to_ns(current_time) / NSEC_PER_SEC;
	time_now.hours = (seconds+10800) %86400 / 3600;
	time_now.minutes = (seconds % 3600) / 60;
	return time_now;
}

// void write_time_to_file( struct current_time time_now, struct file *file) {
// 	char str[5];


// }

void my_timer_callback(struct timer_list *unused)
{
	struct file *file;
	char str[5];
	struct current_time time_now = time_now_func();

	file= filp_open(my_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (IS_ERR(file)) {
		printk(KERN_ERR "Failed to open file %s\n", my_file_path);
		return;
	}
	printk(KERN_INFO "Timer Handler called. Current time: %d:%d\n", time_now.hours, time_now.minutes);
	//write_time_to_file(time_now, file);
	sprintf(str, "%d:%d", time_now.hours, time_now.minutes);
	kernel_write(file, str, 5, &file->f_pos);
	filp_close(file, NULL);
	//перезапуск таймера на время g_time_interval
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(g_time_interval));
}

static int __init my_init(void)
{
	struct current_time time_now= time_now_func();
	

	printk(KERN_INFO "My module inserted into kernel!!!. Current time: %d:%d\n", time_now.hours, time_now.minutes);
	//write_time_to_file(time_now);
	//инициализация таймера
	timer_setup(&my_timer, my_timer_callback, 0);
	mod_timer( &my_timer, jiffies + msecs_to_jiffies(g_time_interval));

	return 0;
}

static void __exit my_exit(void)
{
	struct current_time time_now= time_now_func();

	del_timer(&my_timer);
	printk(KERN_INFO "My module exited from kernel!!! Current time: %d:%d\n", time_now.hours, time_now.minutes);
}

module_init(my_init);
module_exit(my_exit);