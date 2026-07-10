/*
 * ============================================================================
 *
 *       Filename:  s_list.c
 *
 *    Description:  Thread safe linked list data structure
 *
 *        Version:  1.0
 *        Created:  23/10/23 11:58:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#include "sList.h"

List* list_create(
        ListCompareFunction* compareFunction,
        DestroyFunction* destroyFunction,
        PrintFunction* printFunction,
        pthread_mutex_t* mutex) {

    List* list = (List*) malloc(sizeof(List));

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->destroyFunction = destroyFunction;
    list->printFunction = printFunction;
    list->compareFunction = compareFunction;
    list->mutex = mutex;

    return list;
}

void list_append(List* list, void* value) {
    ListNode* node = (ListNode*) malloc(sizeof(ListNode));

    node->value = value;
    node->next = NULL;

    pthread_mutex_lock(list->mutex);

    // Check if list head exists
    if (list->head == NULL) {
        // Set new head/tail as head didn't exists
        list->head = node;
        list->tail = node;
    } else {
        // Append to tail
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;

    pthread_mutex_unlock(list->mutex);
}

void list_remove_first(List* list) {
    pthread_mutex_lock(list->mutex);

    ListNode* head = list->head;

    if (head == NULL) {
        return;
    }

    // Check if there is only 1 element
    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = head->next;
    }
    
    list->destroyFunction(head->value);
    free(head);
    list->size--;

    pthread_mutex_unlock(list->mutex);
}

void list_remove_search(List* list, void* value) {
    pthread_mutex_lock(list->mutex);
    
    ListNode* current = list->head;
    ListNode* previous = NULL;
    
    // Iterate over list and compare current value to search 
    while (current != NULL)  {
        if (list->compareFunction(current->value, value)) {
            break;
        }

        previous = current;
        current = current->next;
    }
    
    if (current == NULL) {
        // Didn't find search value
        pthread_mutex_unlock(list->mutex);
        return;
    }

    // Remove found node
    list_remove_node_unsafe(list, current, previous);

    pthread_mutex_unlock(list->mutex);
}

void list_remove_node_unsafe(
        List* list,
        ListNode* current,
        ListNode* previous) {

    if (current == NULL) {
        return;
    }

    // Reorder list nodes
    if (current == list->head) {
        // Reorder head edge case
        if (list->head == list->tail) {
            list->head = NULL;
            list->tail = NULL;
        } else {
            list->head = list->head->next;
        }
    } else {
        // Remove internal or tail node
        previous->next = current->next;
        
        if (current == list->tail) {
            list->tail = previous;
        }
    }

    list->destroyFunction(current->value);
    free(current);
    list->size--;
}

void* list_find(List* list, void* value) {
    pthread_mutex_lock(list->mutex);
    
    ListNode* current = list->head;
    
    // Iterate over nodes and compare serach value
    while (current != NULL)  {
        if (list->compareFunction(current->value, value)) {
            break;
        }

        current = current->next;
    }
    
    if (current == NULL) {
        // Didn't find search value
        pthread_mutex_unlock(list->mutex);
        return NULL;
    }

    void* returnValue = current->value;
    pthread_mutex_unlock(list->mutex);

    return returnValue;
}

void list_replace(List* list, void* find, void* replace) {
    pthread_mutex_lock(list->mutex);
    
    ListNode* current = list->head;
    
    // Iterate over list nodes and compare values
    while (current != NULL)  {
        if (list->compareFunction(current->value, find)) {
            break;
        }

        current = current->next;
    }
    
    if (current == NULL) {
        // Didn't find value 
        pthread_mutex_unlock(list->mutex);
        return;
    }

    void* returnValue = current->value;

    current->value = replace;

    list->destroyFunction(returnValue);

    pthread_mutex_unlock(list->mutex);
}

void list_print(List* list) {
    pthread_mutex_lock(list->mutex);
    ListNode* current = list->head;

    printf("List Size: %ld [", list->size);

    // Iterate over nodes and print each to the terminal
    while (current != NULL) {
        list->printFunction(current->value);

        if (current->next != NULL) {
            printf(", ");
        }

        current = current->next;
    }

    printf("]\n");
    fflush(stdout);

    pthread_mutex_unlock(list->mutex);
}

void list_destroy(void* object) {
    List* list = (List*) object;
    pthread_mutex_t* mutex = list->mutex;

    pthread_mutex_lock(mutex);
    ListNode* current = list->head;

    // Iterate over each node and free values and node
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;

        list->destroyFunction(temp->value);
        free(temp);
    }

    free(current);
    free(list);
    
    pthread_mutex_unlock(mutex);
}
