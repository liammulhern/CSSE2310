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

#include "sDestroy.h"
#include "sPrint.h"

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

/* list_create()
 * ---------------
 * Creates a thread safe linked list and returns a pointer to the head.
 * 
 * Parameters:
 *  compareFunction: The function pointer used to compare objects
 *  destroyFunction: The function pointer used to free the objects 
 *	in ListNodes
 *  printFunction: The function pointer used to print the objects contained
 *	in the ListNodes to stdout.
 *  mutex: The thread mutex to use when manipulating data.
 *
 * Returns:
 *  Pointer to the initialised linked list.
 * 
 * Errors:
 *  None
 */
List* list_create(
        ListCompareFunction* compareFunction,
        DestroyFunction* destroyFunction,
        PrintFunction* printFunction,
        pthread_mutex_t* mutex);

/* list_append
 * ---------------
 * Appends an object to the end of the linked list.
 * 
 * Parameters:
 *  list: The list to append to.
 *  value: The object that will be appended as a node to the end of the 
 *	linked list.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void list_append(List* list, void* value);

/* list_remove_first
 * ---------------
 * Thread safe removes first element of linked list.
 * 
 * Parameters:
 *  list: The list to remove from.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void list_remove_first(List* list);

/* list_remove_search
 * ---------------
 * Thread safe removal of the object that matches value in the linked list.
 * 
 * Parameters:
 *  list: The list to remove from.
 *  value: The object pointer to search the list for.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If the value cannot be found in the list, it not removed.
 */
void list_remove_search(List* list, void* value);

/* list_remove_search
 * ---------------
 * Nonthread safe removal of the object that matches value in the linked list.
 * 
 * Parameters:
 *  list: The list to remove from.
 *  current: The node to remove from the list.
 *  previous: The node to previous to the current.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void list_remove_node_unsafe(
        List* list,
        ListNode* current,
        ListNode* previous);

/* list_find
 * ---------------
 * Thread safe attempt to find object pointer in list, if it exists 
 * return value, else NULL.
 * 
 * Parameters:
 *  list: The list to search.
 *  value: The object pointer to search the list for.
 *
 * Returns:
 *  If the object pointer is in the list return the value, otherwise returns 
 *  NULL.
 * 
 * Errors:
 *  None
 */
void* list_find(List* list, void* value);

/* list_replace()
 * ---------------
 * Replace object in linked list that matches value.
 * 
 * Parameters:
 *  list: The list to insert to.
 *  object: The object that will be appended as a node at the found index 
 *	of the linked list.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If the value cannot be found in the list, it no updated.
 */
void list_replace(List* list, void* find, void* replace);

/* list_print()
 * ---------------
 * Prints the linked list to stdout.
 * 
 * Parameters:
 *  list: The list to print from.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void list_print(List* list);

/* list_destroy()
 * ---------------
 * Destroys a linked list and frees associated memory including nodes.
 * Implements DestroyFunction function pointer typdef.
 * 
 * Parameters:
 *  object: Generic pointer to the list to destroy.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void list_destroy(void* object);

#endif

