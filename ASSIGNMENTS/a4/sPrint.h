/*
 * ============================================================================
 *
 *       fileName:  print.h
 *
 *    Description:  Header file for print.c
 *
 *        Version:  1.0
 *        Created:  10/08/23 14:21:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern
 *   Organization:  CSSE2310 A1
 *
 * ============================================================================
 */
#ifndef S_PRINT_H
#define S_PRINT_H

/* PrintFunction
 * ---------------
 * Function pointer prototype defining printing for a 
 * generic object type stored in a data structure.
 * 
 * Parameters:
 *  void*: Generic object pointer that will be printed to stdout.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
typedef void PrintFunction (void*);

/* print_string()
 * ---------------
 * Print a formatted string to stdout from a generic pointer.
 * 
 * Parameters:
 *  object: A generic pointer to an object that is cast to a char*.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void print_string(void* object);

/* print_string_pointer()
 * ---------------
 * Print a formatted string and its pointer to stdout from a generic pointer.
 * 
 * Parameters:
 *  object: A generic pointer to an object that is cast to a char*.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void print_string_pointer(void* object);

/* print_int()
 * ---------------
 * Print a formatted int to stdout from	a generic pointer.
 * 
 * Parameters:
 *  object: A generic pointer to an object that is cast to a int*.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void print_int(void* object);

/* print_int_pointer()
 * ---------------
 * Print a pointer and formatted int to stdout from a generic pointer.
 * 
 * Parameters:
 *  object: A generic pointer to an object that is cast to a int*.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void print_int_pointer(void* object);

/* print_long()
 * ---------------
 * Print a formatted long to stdout from a generic pointer.
 * 
 * Parameters:
 *  object: A generic pointer to an object that is cast to a long*.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void print_long(void* object);

/* print_long_pointer()
 * ---------------
 * Print a pointer and formatted long to stdout from a generic pointer.
 * 
 * Parameters:
 *  object: A generic pointer to an object that is cast to a long*.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void print_long_pointer(void* object);

/* print_pointer()
 * ---------------
 * Print a pointer to stdout from a generic pointer.
 * 
 * Parameters:
 *  object: A generic pointer to an object that is cast to a char*.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void print_pointer(void* object);

#endif
