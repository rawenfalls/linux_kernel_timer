void write_on_file(){
	file = filp_open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (IS_ERR(file)) {
		printk(KERN_ERR "Failed to open file %s\n", file_path);
		return;
	}
	kernel_write(file, buffer, strlen(buffer), &file->f_pos);
	filp_close(file, NULL);
}

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fs_struct.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Your module description");

#define FILE_PATH "/tmp/current_time"

struct current_time {
    int hours;
    int minutes;
};

struct timer_list my_timer;
int g_time_interval = 59300;

struct current_time time_now_func(void) {
    struct current_time time_now;
    // получаем текущее время
    ktime_t current_time = ktime_get_real();
    s64 seconds = ktime_to_ns(current_time) / NSEC_PER_SEC;
    time_now.hours = (seconds + 10800) % 86400 / 3600;
    time_now.minutes = (seconds % 3600) / 60;
    return time_now;
}

void write_time_to_file(const char *file_path, struct current_time time_now) {
    struct file *file;
    mm_segment_t old_fs;
    loff_t pos = 0;

    file = filp_open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (IS_ERR(file)) {
        printk(KERN_ERR "Failed to open file %s\n", file_path);
        return;
    }

    old_fs = get_fs();
    set_fs(get_ds());

    // Записываем форматированное время в файл
    vfs_write(file, "Current time: ", 14, &pos);
    vfs_write(file, time_now.hours, 2, &pos);
    vfs_write(file, ":", 1, &pos);
    vfs_write(file, time_now.minutes, 2, &pos);
    vfs_write(file, "\n", 1, &pos);

    set_fs(old_fs);

    filp_close(file, NULL);
}

void my_timer_callback(struct timer_list *unused) {
    struct current_time time_now = time_now_func();
    printk(KERN_INFO "Timer Handler called. Current time: %d:%d\n", time_now.hours, time_now.minutes);

    // Пишем текущее время в файл
    write_time_to_file(FILE_PATH, time_now);

    // Перезапуск таймера на время g_time_interval
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(g_time_interval));
}

static int __init my_init(void) {
    struct current_time time_now = time_now_func();
    printk(KERN_INFO "My module inserted into kernel!!!. Current time: %d:%d\n", time_now.hours, time_now.minutes);

    // Инициализация таймера
    timer_setup(&my_timer, my_timer_callback, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(g_time_interval));

    return 0;
}

static void __exit my_exit(void) {
    struct current_time time_now = time_now_func();

    // Удаление таймера при выгрузке модуля
    del_timer_sync(&my_timer);

    printk(KERN_INFO "My module exited from kernel!!! Current time: %d:%d\n", time_now.hours, time_now.minutes);
}

module_init(my_init);
module_exit(my_exit);
