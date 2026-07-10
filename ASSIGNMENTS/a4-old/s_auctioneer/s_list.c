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

#include "s_list.h"

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

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
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
    
    while (current != NULL)  {
        if (list->compareFunction(current->value, value)) {
            break;
        }

        previous = current;
        current = current->next;
    }
    
    if (current == NULL) {
        pthread_mutex_unlock(list->mutex);
        return;
    }

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

    if (current == list->head) {
        if (list->head == list->tail) {
            list->head = NULL;
            list->tail = NULL;
        } else {
            list->head = list->head->next;
        }
    } else {
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
    
    while (current != NULL)  {
        if (list->compareFunction(current->value, value)) {
            break;
        }

        current = current->next;
    }
    
    if (current == NULL) {
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
    
    while (current != NULL)  {
        if (list->compareFunction(current->value, find)) {
            break;
        }

        current = current->next;
    }
    
    if (current == NULL) {
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

bool list_compare(void* object1, void* object2) {
    return object1 == object2;
}
