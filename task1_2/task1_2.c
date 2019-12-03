#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

// kernel module setting
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Find Task by PID Module for Hw3 Task1");

static int pid = 0; // integer variable to store pid number
module_param(pid, int, 0); // get pid number by using module_param()

// function to initialize kernel module
static int __init task_by_pid_init(void) {
	struct task_struct *task; // structure which will be stored a task related to pid
	task = pid_task(find_vpid(pid), PIDTYPE_PID); // find the task which pid indicates
	printk("[Hyeonggeun] normal_prio: %d, state: %ld, time: %li\n", task->normal_prio, task->state, task->utime);
	return 0;	
}

// function to exit kernel module
static void __exit task_by_pid_exit(void) {
	printk("exit\n");
}

// initialize and exit module with above function
module_init(task_by_pid_init);
module_exit(task_by_pid_exit);

