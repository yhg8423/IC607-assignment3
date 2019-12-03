#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Node & Queue for Hw3 Task2");

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
	printk("Add key: %d\n", new_node->key); // print for testing operation
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
		printk("Delete key: %d\n", delete_node->key); // print for testing operation
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
	printk("Size of List: %d\n", linked_list->size); // print for testing operation
	return linked_list->size; // return the size of list
}

static struct task_struct *ExThread = NULL; // task struct for thread module

// function which the thread excutes
static int QueuePrintk(void *data) {
	// allocate queue and initilization
	list_t *queue = kmalloc(sizeof(list_t), GFP_KERNEL);
	InitList(queue);
	
	// testing queue operation
	int key = 0;
	int add_or_delete = 0; // 0 is add, 1 is delete
	while(!kthread_should_stop()) {
		if(add_or_delete == 0) {
			InsertList(queue, key);
			key += 1;	
		}
		else {
			DeleteList(queue);
			key -= 1;		
		}
		GetSizeList(queue);

		if(key == 5) {
			add_or_delete = 1;		
		}
		else if(key == 0) {
			add_or_delete = 0;		
		}
		ssleep(1);		
	}
	
	// deallocate queue
	kfree(queue);

	return 0;
}

// function to initialize kernel module
static int __init kthread_task2_init(void) {
	// run QueuePrintk() function with ExThread using kthread_run()
	if(ExThread == NULL) {
		ExThread = kthread_run(QueuePrintk, NULL, "kthread_queue");	
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

// Initialize and exit module with above function
module_init(kthread_task2_init);
module_exit(kthread_task2_exit);
