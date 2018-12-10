#include "linkedList.h"
#include <stdlib.h>

void linked_list_init(linked_list_t* linked_list) {
    // Initialise the linked_list
    linked_list->head = NULL;
}

void* linked_list_remove(linked_list_t* linked_list) {
    // Return the head of the list of blocks
    // Update the head pointer
    linked_list_element_t* oldHead = linked_list->head;
    linked_list->head = oldHead->next;
		void* item = oldHead->item;
		free(oldHead);
    return item;
}

void linked_list_add(linked_list_t* linked_list, void* newItem) {
		linked_list_element_t* newElement = malloc(sizeof(linked_list_element_t));
		newElement->item = newItem;
	  newElement->next = linked_list->head;
		// Add the new item to the head of the list
		linked_list->head = newElement;
    
}