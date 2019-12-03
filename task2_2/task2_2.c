#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hyeonggeun");
MODULE_DESCRIPTION("Node & Queue for Hw3 Task2");

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
	printk("Add key: %d\n", new_node->key);
}

int DeleteList(list_t *linked_list) {
	if(linked_list->size > 0) {
		node_t *delete_node = linked_list->head;
		linked_list->head = linked_list->head->next;
		if(linked_list->head != NULL) {
			linked_list->head->prev = NULL;
		}
		
		linked_list->size -= 1;
		printk("Delete key: %d\n", delete_node->key);
		int key = delete_node->key;
		vfree(delete_node);
		return key;	
	}
	else {
		return -1;	
	}
}

int GetSizeList(list_t *linked_list) {
	printk("Size of List: %d\n", linked_list->size);
	return linked_list->size;
}

static struct task_struct *ExThread = NULL;

static int QueuePrintk(void *data) {
	list_t *queue = vmalloc(sizeof(list_t), GFP_KERNEL);
	InitList(queue);
	
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

	kfree(queue);

	return 0;
}

static int __init kthread_task2_init(void) {
	if(ExThread == NULL) {
		ExThread = kthread_run(QueuePrintk, NULL, "kthread_queue");	
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
