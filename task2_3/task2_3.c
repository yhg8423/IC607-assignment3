#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Producer & Consumer Model for Hw3 Task2");

typedef struct __node_t {
	int key;
	struct __node_t *next;
	struct __node_t *prev;	
} node_t;

typedef struct __list_t {
	node_t *head;
	node_t *tail;
	int size;
} list_t;

void InitList(list_t *linked_list) {
	linked_list->head = NULL;
	linked_list->tail = NULL;
	linked_list->size = 0;
}

void InsertList(list_t *linked_list, int key) {
	node_t *new_node = vmalloc(sizeof(node_t));
	new_node->key = key;
	if(linked_list->size == 0) {
		linked_list->head = new_node;
		linked_list->tail = new_node;	
		new_node->prev = NULL;
		new_node->next = NULL;
	}
	else {
		new_node->prev = linked_list->tail;
		linked_list->tail->next = new_node;
		new_node->next = NULL;
		linked_list->tail = new_node;	
	}
	linked_list->size += 1;
}

int DeleteList(list_t *linked_list) {
	if(linked_list->size > 0) {
		node_t *delete_node = linked_list->head;
		linked_list->head = linked_list->head->next;
		if(linked_list->head != NULL) {
			linked_list->head->prev = NULL;
		}
		
		linked_list->size -= 1;
		int key = delete_node->key;
		vfree(delete_node);
		return key;	
	}
	else {
		return -1;
	}
}

int GetSizeList(list_t *linked_list) {
	return linked_list->size;
}

list_t *queue = NULL;

static struct task_struct *ExThread1 = NULL;
static struct task_struct *ExThread2 = NULL;
static struct task_struct *ExThread3 = NULL;
static struct task_struct *ExThread4 = NULL;
static struct task_struct *ExThread5 = NULL;
	
static int producer_func_odd(void *data) {
	list_t *queue = (list_t *)data;
	int num = 1;
	while(!kthread_should_stop()) {
		printk("called\n");
		InsertList(queue, num);
		printk("q_ptr->tail->key = %d\n", queue->tail->key);
		num += 2;
		ssleep(1);		
	}

	return 0;
}

static int producer_func_even(void *data) {
	list_t *queue = (list_t *)data;
	int num = 2;
	while(!kthread_should_stop()) {
		printk("called\n");
		InsertList(queue, num);
		printk("q_ptr->tail->key = %d\n", queue->tail->key);
		num += 2;
		ssleep(1);		
	}

	return 0;
}

static int consumer_func1(void *data) {
	list_t *queue = (list_t *)data;
	int num;
	while(!kthread_should_stop()) {
		num = DeleteList(queue);
		printk("deQ = %d \n", num);
		ssleep(1);	
	}

	return 0;
}

static int consumer_func2(void *data) {
	list_t *queue = (list_t *)data;
	int num;
	while(!kthread_should_stop()) {
		num = DeleteList(queue);
		printk("deQ = %d \n", num);
		ssleep(1);	
	}

	return 0;
}

static int consumer_func3(void *data) {
	list_t *queue = (list_t *)data;
	int num;
	while(!kthread_should_stop()) {
		num = DeleteList(queue);
		printk("deQ = %d \n", num);
		ssleep(1);	
	}

	return 0;
}


static int __init kthread_task2_init(void) {
	queue = kmalloc(sizeof(list_t), GFP_KERNEL);
	InitList(queue);
	if(ExThread1 == NULL) {
		ExThread1 = kthread_run(producer_func_odd, (void *)queue, "producer odd");	
	}
	if(ExThread2 == NULL) {
		ExThread2 = kthread_run(producer_func_even, (void *)queue, "producer even");	
	}
	if(ExThread3 == NULL) {
		ExThread3 = kthread_run(consumer_func1, (void *)queue, "consumer 1");	
	}
	if(ExThread4 == NULL) {
		ExThread4 = kthread_run(consumer_func2, (void *)queue, "consumer 2");	
	}
	if(ExThread5 == NULL) {
		ExThread5 = kthread_run(consumer_func3, (void *)queue, "consumer 3");	
	}
	return 0;
}

static void __exit kthread_task2_exit(void) {
	if(ExThread1) {
		kthread_stop(ExThread1);
		ExThread1 = NULL;	
	}
	if(ExThread2) {
		kthread_stop(ExThread2);
		ExThread2 = NULL;	
	}
	if(ExThread3) {
		kthread_stop(ExThread3);
		ExThread3 = NULL;	
	}
	if(ExThread4) {
		kthread_stop(ExThread4);
		ExThread4 = NULL;	
	}
	if(ExThread5) {
		kthread_stop(ExThread5);
		ExThread5 = NULL;	
	}
	kfree(queue);
}

module_init(kthread_task2_init);
module_exit(kthread_task2_exit);
