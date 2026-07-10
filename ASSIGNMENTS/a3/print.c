/*
 * ============================================================================
 *
 *       fileName:  print.c
 *
 *    Description:  Implements default functions for printing generic objects.
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
#include <stdio.h>

#include "print.h"

void print_string(void* object) {
    char* word = (char*) object;

    printf("\"%s\"", word);
}

void print_string_pointer(void* object) {
    char* word = (char*) object;

    printf("\"%s\" -> %p", word, object);
}

void print_int(void* object) {
    int* number = (int*) object;

    printf("%d", *number);
}

void print_int_pointer(void* object) {
    int* number = (int*) object;

    printf("%d -> %p", *number, object);
}

void print_long(void* object) {
    long* number = (long*) object;

    printf("%ld", *number);
}

void print_long_pointer(void* object) {
    long* number = (long*) object;

    printf("%ld -> %p", *number, object);
}

void print_pointer(void* object) {
    printf("%p", object);
}
