#include "os.h"
#include <stdio.h>
#include "utils/serial.h"
#include "simpleRoundRobin.h"
#include "fixedPri.h"
#include "sleep.h"
#include "mutex.h"
#include "circularBuffer.h"

#define ENTER(x) printf("ENTER "#x"\r\n")
#define MID(x) printf("MID "#x"\r\n")
#define EXIT(x) printf("EXIT "#x"\r\n")


//#define ENTER(x) 
//#define MID(x) 
//#define EXIT(x) 

static OS_mutex_t mutex;
static queue_t queue;

void task1(void const *const args) {
	int value;
	int status;
	while (1) {
		ENTER(1);
		OS_mutex_acquire(&mutex);
		MID(1);
		printf("Message from Task 1\r\n");
		
		status = queue_get(&queue, &value);
		printf("Message from Task 1: Status: %d Value: %d \r\n", status, value);
		
		status=queue_get(&queue, &value);
		printf("Message from Task 1: Status: %d Value: %d \r\n", status, value);
		
		status=queue_get(&queue, &value);
		printf("Message from Task 1: Status: %d Value: %d \r\n", status, value);
		//OS_sleep(500);
		OS_mutex_release(&mutex);
		EXIT(1);
		OS_sleep(1000);
		
	}
}
void task2(void const *const args) {
	int value;
	int status;
	while (1) {
		ENTER(2);
		OS_mutex_acquire(&mutex);
		MID(2);
		//status=queue_get(&queue, &value);
		printf("Message from Task 2\r\n");
		OS_sleep(3000);
		OS_mutex_release(&mutex);
		EXIT(2);
		OS_yield();
		OS_sleep(3000);
	}
}
void task3(void const *const args) {
	while (1) {
		ENTER(3);
		OS_mutex_acquire(&mutex);
		MID(3);
		printf("Message from Task 3\r\n");
		OS_sleep(3000);
		OS_mutex_release(&mutex);
		EXIT(3);
		//OS_sleep(4000);
		
	}
}
void task4(void const *const args) {
	while (1) {
		ENTER(4);
		OS_mutex_acquire(&mutex);
		MID(4);
		printf("Message from Task 4\r\n");
		MID(4b);
		//queue_put(&queue, 1);
		//queue_put(&queue, 2);
		//queue_put(&queue, 3);
		OS_sleep(2000);
		MID(4a);
		OS_mutex_release(&mutex);
		EXIT(4);
		OS_sleep(5000);
	}
}

void printTasks();

/* MAIN FUNCTION */
int main(void) {
	/* Initialise the serial port so printf() works */
	serial_init();
	mutex_init(&mutex);
	queue_init(&queue);
	
	int item1 = 1;
	int item2 = 2;
	int item3 = 3;
	
	linked_list_t testList;
	linked_list_init(&testList);
	linked_list_add(&testList, &item1);
	linked_list_add(&testList, &item2);
	linked_list_add(&testList, &item3);
	
	for (int i = 0; i<3; i++) {
		int* testItem = linked_list_remove(&testList);
		printf("\r\n Test: %d \r\n", *testItem);
	}
	printf("\r\nDocetOS Sleep and Mutex\r\n");

	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	__align(8)
	static uint32_t stack1[64], stack2[64], stack3[64], stack4[64];
	static OS_TCB_t TCB1, TCB2, TCB3, TCB4;

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+64, task1, 0);
	OS_initialiseTCB(&TCB2, stack2+64, task2, 0);
	OS_initialiseTCB(&TCB3, stack3+64, task3, 0);
	OS_initialiseTCB(&TCB4, stack4+64, task4, 0);
	TCB1.priority = 10;
	TCB2.priority = 6;
	TCB3.priority = 8;
	TCB4.priority = 5;

	/* Initialise and start the OS */
	//OS_init(&simpleRoundRobinScheduler);
	OS_init(&fixedPriScheduler);
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_addTask(&TCB3);
	OS_addTask(&TCB4);
	
	printTasks();
	
	OS_start();
}
