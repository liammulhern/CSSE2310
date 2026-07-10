/*
 * ============================================================================
 *
 *       fileName:  list.c
 *
 *    Description:  Provides functionality for linked list and associated
 *		    CRUD functions.
 *
 *        Version:  1.0
 *        Created:  09/08/23 15:48:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern 
 *   Organization:  CSSE2310 A1 
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

ListNode* node_create(void* object) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->object = object;
    node->next = NULL;

    return node;
}

List* list_create(DestroyFunction destroyFunction, 
	PrintFunction* printFunction) {
    
    // Initialise default list values
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->printFunction = printFunction;
    list->destroyFunction = destroyFunction;
    list->printFunction = printFunction; 

    return list;
}

void list_destroy(void* object) {
    if (object == NULL) {
	return;
    }
    
    List* list = (List*) object;
    ListNode* currentNode = list->head;
    
    // Travel list and free each node and associate objects.
    while (currentNode != NULL) {
	ListNode* temp = currentNode;
	currentNode = currentNode->next;
	
	list->destroyFunction(temp->object);
	free(temp);
    }

    free(currentNode);
    free(list);
}

void list_append(List* list, void* object) {
    if (list == NULL) {
	return;
    }

    ListNode* node = node_create(object);
    
    // Check if tail exists and append
    if (list->tail != NULL) {
	list->tail->next = node;
	list->tail = node;
    } else {
	list->head = node;
	list->tail = node;
    }

    list->size++;
}

void list_insert_at_head(List* list, void* object) {
    if (list == NULL) {
	return;
    }

    ListNode* node = node_create(object);

    // Insert at head and reorder node
    node->next = list->head;
    list->head = node;
    list->size++;
}

void list_insert(List* list, void* object, size_t index) {
    if (list == NULL || index < 0 || index >= list->size) {
	return;
    }

    if (index == 0) {
	list_insert_at_head(list, object);
	return;
    }
    
    ListNode* node = node_create(object);
    ListNode* previous = list->head;

    // Travel list for specified number of nodes before inserting.
    for (int i = 0; i < index - 1; i++) {
	previous = previous->next;	
    }
    
    ListNode* next = previous->next;

    // Insert node into list.
    previous->next = node;
    node->next = next;
    list->size++;
}

void list_replace_at_head(List* list, void* object) {
    if (list == NULL) {
	return;
    }

    ListNode* head = list->head;

    // Create node and swap head
    ListNode* node = node_create(object);
    node->next = head->next;
    list->head = node;

    // Destroy head node and object
    list->destroyFunction(head->object);
    free(head);
}

void list_replace(List* list, void* object, size_t index) {
    if (list == NULL || index < 0 || index >= list->size) {
	return;
    }

    if (index == 0) {
	list_replace_at_head(list, object);
	return;
    }
    
    ListNode* node = node_create(object);
    ListNode* previous = list->head;

    // Travel list for specified number of nodes before inserting.
    for (int i = 0; i < index - 1; i++) {
	previous = previous->next;	
    }
    
    ListNode* next = previous->next;
    
    // Replace node and rearrange pointers.
    previous->next = node;
    node->next = next->next;

    list->destroyFunction(next->object);
    free(next);
}

void* list_get(List* list, size_t index) {
    if (list == NULL || index < 0 || index >= list->size) {
	return NULL;
    }
    
    ListNode* temp = list->head;

    // Travel list for specified number of nodes before inserting.
    for (int i = 0; i < index; i++) {
	temp = temp->next;	
    }

    return temp->object;
}

ListNode* list_iterate(List* list, ListNode* currentNode, size_t* index) {
    if (list == NULL) {
	return NULL;
    }

    if (currentNode == NULL && *index == 0) {
	// If first iteration set the node to the head of the list
	currentNode = list->head;
    } else {
	// Increase the index for iterations after the head.
	(*index)++;
	currentNode = currentNode->next;
    }

    if (currentNode != NULL) {
	return currentNode;
    } else {
	return NULL;
    }
}

int list_find(List* list, void* object, ListCompareFunction* compare) {
    if (list == NULL || object == NULL) {
	return NOT_IN_LIST;
    }

    ListNode* node = list->head;
    int index = 0;

    // Iterate over linked list and compare each node's object
    while (node != NULL) {
	// If objects match return the index
	if (compare(node->object, object)) {
	    return index;
	}

	index++;
	node = node->next;
    }

    return NOT_IN_LIST;
}

void list_remove_at_head(List* list) {
    if (list == NULL) {
	return;
    }
    
    // Remove head and replace with following node.
    ListNode* head = list->head;
    list->head = head->next;

    // Free memory associated with original head node
    list->destroyFunction(head->object);
    free(head);
    list->size--;
}

void list_remove(List* list, size_t index) { 
    if (list == NULL || index < 0 || index >= list->size) {
	return;
    }
    
    if (index == 0) {
	list_remove_at_head(list);
	return;
    }

    ListNode* previous = list->head;
    
    // Travel list until the index before the node we want to remove
    for (int i = 0; i < index - 1; i++) {
	previous = previous->next;	
    }
    
    // Remove node and reorder nodes 
    ListNode* next = previous->next;
    previous->next = next->next;
    
    // Free memory associated with removed node
    list->destroyFunction(next->object);
    free(next);
    list->size--;
}

void list_print(void* object) {
    if (object == NULL) {
	return;
    }

    List* list = (List*) object;
    ListNode* node = list->head;
    
    printf("List Size: %ld [", list->size);
    
    // Travel the list and print each node using print function.
    while (node != NULL) {
	list->printFunction(node->object);

	if (node->next != NULL) {
	    printf(", ");
	}

	node = node->next;
    }

    printf("]\n");
}

size_t list_size(List* list) {
    if (list == NULL) {
	return 0;
    }
    
    return list->size;
}

bool list_compare_pointer(void* object1, void* object2) {
    return object1 == object2;
}

bool list_compare_char(void* object1, void* object2) {
    char* word1 = (char*) object1;
    char* word2 = (char*) object2;

    return strcmp(word1, word2) == 0;
}
