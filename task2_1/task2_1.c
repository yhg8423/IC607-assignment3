#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Name Kernel Thread Module for Hw3 Task2");

static struct task_struct *ExThread = NULL;

static int NamePrintk(void *data) {
	while(!kthread_should_stop()) {
		printk("Hyeonggeun Yun! \n");
		ssleep(1);		
	}

	return 0;
}

static int __init kthread_task2_init(void) {
	if(ExThread == NULL) {
		ExThread = kthread_run(NamePrintk, NULL, "kthread_name");	
	}
	return 0;
}

static void __exit kthread_task2_exit(void) {
	if(ExThread) {
		kthread_stop(ExThread);
		ExThread = NULL;	
	}
}

module_init(kthread_task2_init);
module_exit(kthread_task2_exit);
