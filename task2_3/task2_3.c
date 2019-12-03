#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/signal.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Producer & Consumer Model for Hw3 Task2");

// node structure which has a key(number), next pointer of node, prev pointer of node 
typedef struct __node_t {
	int key;
	struct __node_t *next;
	struct __node_t *prev;	
} node_t;

// linked list structure which has a size, pointer of list's head and pointer of list's tail
typedef struct __list_t {
	node_t *head;
	node_t *tail;
	int size;
} list_t;

// function to initialize linked list
void InitList(list_t *linked_list) {
	// To initilize, head and tail is set as null pointer and size of the list is set as zero
	linked_list->head = NULL;
	linked_list->tail = NULL;
	linked_list->size = 0;
}

// function for Insert (enqueue) operation 
void InsertList(list_t *linked_list, int key) {
	// allocated new_node with node structure and set new_node->key = key
	node_t *new_node = vmalloc(sizeof(node_t));
	new_node->key = key;

	// if size of list is zero, then new_node is head and tail of list
	if(linked_list->size == 0) {
		linked_list->head = new_node;
		linked_list->tail = new_node;	
		new_node->prev = NULL;
		new_node->next = NULL;
	}
	else {
		// insert new_node after the tail of list
		new_node->prev = linked_list->tail;
		linked_list->tail->next = new_node;
		new_node->next = NULL;
		linked_list->tail = new_node;	
	}
	linked_list->size += 1; // size up the list
}

// function for delete (dequeue) operation
int DeleteList(list_t *linked_list) {
	if(linked_list->size > 0) {
		// set delete_node as head of list
		node_t *delete_node = linked_list->head;
		
		// set new head of list
		linked_list->head = linked_list->head->next;
		if(linked_list->head != NULL) {
			linked_list->head->prev = NULL;
		}
		
		linked_list->size -= 1; // size down the list
		int key = delete_node->key; // key varaible to store return value
		vfree(delete_node); // deallocate delete_node to delete it.
		return key; // return the value
	}
	else {
		// if size is zero, there is no node. Therefore it returns -1
		return -1;	
	}
}

// function to get list size
int GetSizeList(list_t *linked_list) {
	return linked_list->size; // return the size of list
}

// declare queue and initialize with NULL
list_t *queue = NULL;

// task structs for thread module (ExThread1, 2: producer / Exthread3, 4, 5: consumer)
static struct task_struct *ExThread1 = NULL;
static struct task_struct *ExThread2 = NULL;
static struct task_struct *ExThread3 = NULL;
static struct task_struct *ExThread4 = NULL;
static struct task_struct *ExThread5 = NULL;

// function which indicates producer that generates odd numbers
static int producer_func_odd(void *data) {
	allow_signal(SIGKILL);
	
	// define queue with data from parameter of function
	list_t *queue = (list_t *)data;
	int num = 1; // set as smallest odd number, 1
	
	printk("@producer_func_odd: called \n");
	while(!kthread_should_stop()) {
		InsertList(queue, num); // insert odd number to queue
		printk("@producer_func_odd: q_ptr->tail->key = %d\n", queue->tail->key);
		num += 2; // update num to next odd number
		ssleep(1);
		
		if(signal_pending(ExThread1)) {
			break;		
		}	
	}

	return 0;
}

// function which indicates producer that generates even numbers
static int producer_func_even(void *data) {
	allow_signal(SIGKILL);

	// define queue with data from parameter of function
	list_t *queue = (list_t *)data;
	int num = 2; // set as smallest even number, 2
	
	printk("@producer_func_even: called \n");
	while(!kthread_should_stop()) {
		InsertList(queue, num); // insert even number to queue
		printk("@producer_func_even: q_ptr->tail->key = %d\n", queue->tail->key);
		num += 2; // update num to next even number
		ssleep(1);
		
		if(signal_pending(ExThread2)) {
			break;		
		}	
	}

	return 0;
}

// function which indicates consumer
static int consumer_func1(void *data) {
	allow_signal(SIGKILL);
	
	// define queue with data from parameter of function
	list_t *queue = (list_t *)data;
	int num;
	
	printk("@consumer_func1: called \n");
	while(!kthread_should_stop()) {
		num = DeleteList(queue); // delete number from queue and store the number to num variable
		if(num == -1) {
			// if there is no numbers, print hit bottom
			printk("@consumer_func1: hit bottom \n", num);
		}
		else {
			printk("@consumer_func1: deQ = %d \n", num);
		}		
		ssleep(1);
		
		if(signal_pending(ExThread3)) {
			break;		
		}
	}

	do_exit(0);
	return 0;
}

// function which indicates consumer
static int consumer_func2(void *data) {
	allow_signal(SIGKILL);
	
	// define queue with data from parameter of function
	list_t *queue = (list_t *)data;
	int num;

	printk("@consumer_func2: called \n");
	while(!kthread_should_stop()) {
		num = DeleteList(queue); // delete number from queue and store the number to num variable
		if(num == -1) {
			// if there is no numbers, print hit bottom
			printk("@consumer_func2: hit bottom \n", num);
		}
		else {
			printk("@consumer_func2: deQ = %d \n", num);
		}
		ssleep(1);

		if(signal_pending(ExThread4)) {
			break;		
		}	
	}
	
	do_exit(0);
	return 0;
}

// function which indicates consumer
static int consumer_func3(void *data) {
	allow_signal(SIGKILL);
	
	// define queue with data from parameter of function
	list_t *queue = (list_t *)data;
	int num;
	
	printk("@consumer_func3: called \n");
	while(!kthread_should_stop()) {
		num = DeleteList(queue); // delete number from queue and store the number to num variable
		if(num == -1) {
			// if there is no numbers, print hit bottom
			printk("@consumer_func3: hit bottom \n", num);
		}
		else {
			printk("@consumer_func3: deQ = %d \n", num);
		}
		ssleep(1);

		if(signal_pending(ExThread5)) {
			break;		
		}
	}

	do_exit(0);
	return 0;
}

// function to initialize kernel module
static int __init kthread_task2_init(void) {
	// allocate queue and initialization
	queue = kmalloc(sizeof(list_t), GFP_KERNEL);
	InitList(queue);
	printk("@kthread_task2_init: called \n");

	// run producer and consumer functions with ExThread1,2,3,4,5 using kthread_run()
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

// function to exit kernel module
static void __exit kthread_task2_exit(void) {
	printk("@kthread_task2_exit: called \n");
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
	// deallocate queue
	kfree(queue);
}

// Initialize and exit module with above functions
module_init(kthread_task2_init);
module_exit(kthread_task2_exit);
