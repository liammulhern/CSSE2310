/*
 * ============================================================================
 *
 *       Filename:  s_list.h
 *
 *    Description:  Header file for s_list.c
 *
 *        Version:  1.0
 *        Created:  23/10/23 11:58:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef S_LIST_H
#define S_LIST_H

#include <stdbool.h>
#include <pthread.h>

#include "s_destroy.h"
#include "s_print.h"

/* ListCompareFunction()
 * ---------------
 * Compares 2 objects in a linked list and returns true if they are
 * considered the same.
 * 
 * Parameters:
 *  void* : First list object pointer to compare.
 *  void* : Second list object pointer to compare.
 *
 * Returns:
 *  True if objects are considered the same, false otherwise.
 * 
 * Errors:
 *  None
 */
typedef bool ListCompareFunction(void*, void*);

/* ListNode
 * ---------------
 * Satellite node in linked list that contains a pointer to an object and
 * the next node in the list.
 * 
 * Parameters:
 *  value: Pointer to the object that the element is storing.
 *  next: Pointer to the next ListNode in the chain.
 */
typedef struct ListNode {
    void* value;
    struct ListNode* next; 
} ListNode;

/* List
 * ---------------
 * Struct represnting a linked List's state.
 * 
 * Parameters:
 *  head: The head of the list (i.e first element node).
 *  tail: The tail of the list (i.e last element node).
 *  destroyFunction: The function pointer used to free the objects 
 *	contained in the ListNodes.
 *  printFunction: The function pointer used to print the objects contained
 *	in the ListNodes to stdout.
 *  mutex: Thread lock for concurrency.
 *  size: The number of elements in the linked list.
 */
typedef struct List {
    ListNode* head;
    ListNode* tail;
    DestroyFunction* destroyFunction;
    PrintFunction* printFunction;
    ListCompareFunction* compareFunction;
    pthread_mutex_t* mutex;
    size_t size;
} List;

List* list_create(
        ListCompareFunction* compareFunction,
        DestroyFunction* destroyFunction,
        PrintFunction* printFunction,
        pthread_mutex_t* mutex);

void list_append(List* list, void* value);

void list_remove_search(List* list, void* value);

void list_remove_node_unsafe(
        List* list,
        ListNode* current,
        ListNode* previous);

void* list_find(List* list, void* value);

void list_replace(List* list, void* find, void* replace);

void list_print(List* list);

void list_destroy(void* object);

#endif

