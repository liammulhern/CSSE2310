/*
 * ============================================================================
 *
 *       fileName:  destory.h
 *
 *    Description:  Header file for destroy.c
 *
 *        Version:  1.0
 *        Created:  10/08/23 14:12:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (), 
 *   Organization:  
 *
 * ============================================================================
 */
#ifndef DESTROY_H
#define DESTROY_H

/* DestroyFunction
 * ---------------
 * Function Pointer prototype used to destroy (free memory) an object
 * and its associated memory.
 * 
 * Parameters:
 *  void*: The object to free.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
typedef void DestroyFunction (void*);

/* destroy_null()
 * ---------------
 * Default non-destructive free function that is used when pointer is handled
 * by other data structure to stop double freeing.
 * 
 * Parameters:
 *  object: Generic pointer that does not handle allocated memory.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void destroy_null(void* object);

#endif
