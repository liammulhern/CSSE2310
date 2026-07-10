/*
 * ============================================================================
 *
 *       fileName:  list.h
 *
 *    Description:  Header file for list.c 
 *
 *        Version:  1.0
 *        Created:  09/08/23 15:48:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern  
 *   Organization:  CSSE2310 A1 
 *
 * ============================================================================
 */
#ifndef LIST_H
#define LIST_H

#define NOT_IN_LIST -1

#include <stddef.h>
#include <stdbool.h>

#include "destroy.h"
#include "print.h"

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
 *  object: Pointer to the object that the element is storing.
 *  next: Pointer to the next ListNode in the chain.
 */
typedef struct ListNode {
    void* object;
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
 *  size: The number of elements in the linked list.
 */
typedef struct List {
    ListNode* head;
    ListNode* tail;
    DestroyFunction* destroyFunction;
    PrintFunction* printFunction;
    size_t size;
} List;

/* node_create()
 * ---------------
 * Creates a linked list node that contains a pointer to an object and the 
 * next node.
 * 
 * Parameters:
 *  object: The object that will be assigned to the node.
 *
 * Returns:
 *  Pointer to the initialised node. 
 * 
 * Errors:
 *  None
 */
ListNode* node_create(void* object);

/* list_create()
 * ---------------
 * Creates a linked list and returns a pointer to the head.
 * 
 * Parameters:
 *  destroyFunction: The function pointer used to free the objects 
 *	in ListNodes
 *  printFunction: The function pointer used to print the objects contained
 *	in the ListNodes to stdout.
 *
 * Returns:
 *  Pointer to the initialised linked list.
 * 
 * Errors:
 *  None
 */
List* list_create(
	DestroyFunction destroyFunction, 
	PrintFunction* printFunction);

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

/* list_append()
 * ---------------
 * Appends an object to the end of the linked list.
 * 
 * Parameters:
 *  list: The list to append to.
 *  object: The object that will be appended as a node to the end of the 
 *	linked list.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void list_append(List* list, void* object);

/* list_insert_at_head()
 * ---------------
 * Inserts an object to the head of the linked list.
 * 
 * Parameters:
 *  list: The list to append to.
 *  object: The object that will be appended as a node to the head of the 
 *	linked list.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void list_insert_at_head(List* list, void* object);

/* list_insert()
 * ---------------
 * Insert object into the linked list at the given index.
 * 
 * Parameters:
 *  list: The list to insert to.
 *  object: The object that will be appended as a node to the head of the 
 *	linked list.
 *  index: The index at which the object will be inserted.
 *	Must be between 0 (head) and the size of the list.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If index is not between 0 and the size of the list - 1.
 */
void list_insert(List* list, void* object, size_t index);

/* list_replace_at_head()
 * ---------------
 * Replaces the head with object node.
 * 
 * Parameters:
 *  object: The object that will be appended as a node to the head of the 
 *	linked list.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If index is not between 0 and the size of the list - 1.
 */
void list_replace_at_head(List* list, void* object);

/* list_replace()
 * ---------------
 * Replace object in linked list at index.
 * 
 * Parameters:
 *  list: The list to insert to.
 *  object: The object that will be appended as a node at the given index 
 *	of the linked list.
 *  index: The index at which the object will be replaced.
 *	Must be between 0 (head) and the size of the list - 1.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If index is not between 0 and the size of the list - 1.
 */
void list_replace(List* list, void* object, size_t index);

/* list_get()
 * ---------------
 * Gets the object at a given index of the linked list.
 * 
 * Parameters:
 *  list: The list to get from.
 *  index: The index at which the object will be returned.
 *	Must be between 0 (head) and the size of the list - 1.
 *
 * Returns:
 *  Pointer to the object at the index. NULL if index is out of range.
 * 
 * Errors:
 *  If index is not between 0 and the size of the list - 1.
 */
void* list_get(List* list, size_t index);

/* list_iterate()
 * ---------------
 * Iterate over a List and return the node and its index in the 
 * currentNode and index parameters. Iteration returns NULL when the List 
 * contains no more nodes.
 * 
 * Parameters:
 *  list: The list to iterate over.
 *  currentNode: The pointer to the returned node (Initialise to NULL).
 *  index: The HashTable index of the node (Initialise to 0);
 *
 * Returns:
 *  The node at the current index, or NULL when the iteration finishes.
 * 
 * Errors:
 *  None
 */
ListNode* list_iterate(List* list, ListNode* currentNode, size_t* index);

/* list_find()
 * ---------------
 * Attempt to find object pointer in list, if it exists return true, 
 * else false.
 * 
 * Parameters:
 *  list: The list to search.
 *  object: The object pointer to search the list for.
 *  compare: The function used to compare objects.
 *
 * Returns:
 *  If the object pointer is in the list return the index, otherwise returns 
 *  NOT_IN_LIST (-1).
 * 
 * Errors:
 *  None
 */
int list_find(List* list, void* object, ListCompareFunction* compare);

/* list_remove_at_head()
 * ---------------
 * Removes the object at the head of the list.
 * 
 * Parameters:
 *  list: The list to remove the object from the head.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void list_remove_at_head(List* list);

/* list_remove()
 * ---------------
 * Removes the object at a given index of the linked list.
 * 
 * Parameters:
 *  list: The list to remove from.
 *  index: The index at which the object will be removed.
 *	Must be between 0 (head) and the size of the list - 1.
 *
 * Returns:
 *  Pointer to the object at the index. 
 * 
 * Errors:
 *  If index is not between 0 and the size of the list -1.
 */
void list_remove(List* list, size_t index);

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
void list_print(void* object);

/* list_size()
 * ---------------
 * Gets the size of a list of elements.
 * 
 * Parameters:
 *  list: The list to get the size from.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
size_t list_size(List* list);

/* list_compare_pointer()
 * ---------------
 * Compares 2 generic pointers, if they are the same returns true, 
 * false otherwise. Implements ListCompareFunction function pointer 
 * typedef.
 * 
 * Parameters:
 *  object1: First pointer to compare.
 *  object2: Second pointer to compare.
 *
 * Returns:
 *  True if the pointer are the same, false otherwise.
 * 
 * Errors:
 *  None
 */
bool list_compare_pointer(void* object1, void* object2);

/* list_compare_char()
 * ---------------
 * Compares 2 char pointers, cast from generic pointers, and if they are the
 * same word returns ture, false otherwise. Implements ListCompareFunction 
 * function pointer typedef.
 * 
 * Parameters:
 *  object1: First pointer to cast to a char* and compare.
 *  object2: Second pointer to cast to a char* and compare.
 *
 * Returns:
 *  True if the words are the same, false otherwise.
 * 
 * Errors:
 *  None
 */
bool list_compare_char(void* object1, void* object2);

#endif
