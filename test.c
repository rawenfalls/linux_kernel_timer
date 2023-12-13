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

// функция форматирования времени в формат "hh:mm"
// static void format_time(struct timespec *ts, char *buffer)
// {
// 	struct tm tm;
// 	time64_to_tm(ts->tv_sec, 0, &tm);
// 	snprintf(buffer, 6, "%02d:%02d", tm.tm_hour, tm.tm_min);
// }

void my_timer_callback(struct timer_list *unused)
{
	// получаем текущее время
	struct timespec current_time;
	struct tm timeinfo;
	getnstimeofday(&current_time);
	// Форматируем текущее время в "hh:mm"
	// char buffer[6];
	// format_time(&current_time, buffer);
	time64_to_tm(current_time.tv_sec, 0, &timeinfo);
	//перезапуск таймера на время g_time_interval
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(g_time_interval));

	printk(KERN_INFO "Timer Handler called.\nCurrent time: %02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min);
}

static int __init my_init(void)
{
	printk(KERN_INFO "My module inserted into kernel!!!.\n");

	//инициализация таймера
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