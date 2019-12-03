#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

// kernel module setting
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Hello World Module for Hw3 Task1");

// function to initialize kernel module
static int __init hello_init(void) {
	printk(KERN_INFO "Hello, World!\n"); // "print Hello, World!"
	return 0;
}

// function to exit kernel module
static void __exit hello_exit(void) {
	printk(KERN_INFO "Exit the Module\n");
}

// initialize and exit module with above function
module_init(hello_init);
module_exit(hello_exit);
