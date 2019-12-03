#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

// kernel module setting
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Name Kernel Thread Module for Hw3 Task2");

static struct task_struct *ExThread = NULL; // task_struct for thread module

// function that the thread excute
static int NamePrintk(void *data) {
	while(!kthread_should_stop()) {
		printk("Hyeonggeun Yun! \n"); // print my name
		ssleep(1); // delay 1 second	
	}

	return 0;
}

// function to initialize kernel module
static int __init kthread_task2_init(void) {
	// run NamePrintk() function with ExThread using kthread_run()
	if(ExThread == NULL) {
		ExThread = kthread_run(NamePrintk, NULL, "kthread_name");	
	}
	return 0;
}


// function to exit kernel module
static void __exit kthread_task2_exit(void) {
	if(ExThread) {
		kthread_stop(ExThread);
		ExThread = NULL;	
	}
}

// initialize and exit module with above functions
module_init(kthread_task2_init);
module_exit(kthread_task2_exit);
