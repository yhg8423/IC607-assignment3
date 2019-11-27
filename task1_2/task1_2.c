#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Hello World Module for Hw3 Task1");

static int pid = 0;
module_param(pid, int, 0);

static int __init task_by_pid_init(void) {
	struct task_struct *task;
	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	printk("[Hyeonggeun] normal_prio: %d, state: %ld, time: %li\n", task->normal_prio, task->state, task->utime);
	return 0;	
}

static void __exit task_by_pid_exit(void) {
	printk("exit\n");
}

module_init(task_by_pid_init);
module_exit(task_by_pid_exit);

