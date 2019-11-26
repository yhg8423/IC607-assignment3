#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Hello World Module for Hw3 Task1");

static int __init hello_init(void) {
	printk(KERN_INFO "Hello, World!\n");
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_INFO "Exit the Module\n");
}

module_init(hello_init);
module_exit(hello_exit);
