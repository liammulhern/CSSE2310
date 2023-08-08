### Computing Systems Principles And Programming (CSSE2310)
# 1. Initialising C programs

```
int main() {
    // Body of code entry point from shell
    return 1;
}
```

# 2. Building C programs
```dos
    gcc -o <file name> <c file name>

    ./<file name>
```

Compile flags 
    -std=c99
    -g : Debug Info
    -Wall
    -pedantic

# 3. GNU manual (man) command
Outlines command for help with linux commands and library functions

## Basic C Programming
# Printf

```
    printf("Hello World")
```

Placeholder (%) indicates the start of a format specifier that allows variables 
to be formatted in std out message. 

Placeholders must describe the type of variable being formatted e.g %d (int)i, 
%u (uint)

# Header Files

```
    #include <headername.h>
    #include <stdio.h>
```

Compiling with header files require the correct flags. Use

```
    man <function>  
```

To see the package flags to include 

```
    man man
```

Gives detail on man instructions (useful for looking for man page sections)

```
    man -k <function>
```

Give details for all functions with name.

# Types
int
    - integer (16 | 32 | 64 bit)
unsigned int 
    - unsigned integer
char 
    - character (8 bit)
float 
    - single precision floating point number
double 
    - double precision floating point number
array 
    - array of type (int numb[10];)
    - Initialised when declared (int num[] = {1, 2, 3};)
string 
    - array of char (char str[] = "hello";)
    - strings require mempory for one extra character than given

# Parameters of main

Main is the entry point to a program. It contains 2 Parameters that describe 
an array of strings.

```
    int argc: The number of strings in the array
    char** argv or char* argv[]: The array itself
```

argv[0]: program name
argv[...]: command line arguments

Note: C arrays are not range checked (i.e. can access elements off the 
end of the array)

# Pointers

Allows indirection of variables by referencing memory addresses.
A pointer is typed to the type of the variable it is pointing to.

e.g.
```
    int* a = 0; // Create vairable a that points to memory address 0.
```

Note: In MOSS pointer addresses are sized to 64 bits.

Pointers are dereferenced to grab the actual value at the memory location 
using the * operator.

e.g.
```
    int value = *a; // Dereference pointer to grab value in memory adress a.
```

# Memory Allocation

malloc allocates dynamic memory that can be assigned during run time.

e.g. Initalise pointer p with allocated memory address returned by malloc.
```
    int* p = (int*)malloc(sizeof(int));
```

Note: C does not guarantee that memory is initialised to 0

Memory leaks are caused when pointer references are lost and memory cannot
be freed.

e.g. Free memory at pointer memory address
```
    free(void *ptr);
```

Dangling pointers occur when memory has been freed but another pointer is 
still referencing the memory address. This means the memory can be changed 
unexpectedly when dereferenced.

e.g. Intialise memory of size and initialise memory to 0
```
    int* p = calloc (int *)calloc(10, sizeof(int));
```

Memory reallocation occurs when memory size has to increase to store more data.

e.g Reallocate memory from a previous memory address pointer.
```

```

# Dynamic Arrays

Arrays are inherently pointers with the address &arr[0] 

# NULL Pointers

Included in the stddef.h library. New pointers should be initialised to NULL.

# Structures (Structs)

Group data types together in 'parent' struct.


e.g. Difference between pointer and instanced struct
```
    struct Data {
	int length;
	char* str;
    };

    struct Data d1;
    struct Data* d2 = malloc(sizeof(struct Data));

    d1.length = 10;
    d2->length = 10;
```

# Allocating blocks of memory
```
    *memset(void *s //Pointer ot memory, int c //Value to initalise to, 
	size_t n //# of bytes);
    *memcpy(void *dest, const void *src, size_t n);
```
Note: When copying memory buffers must not overlap 

Dynamically allocated memory is stored on the heap whilst function variables 
are stored on the stack

Heap memory is only cleaned up when explicitly told to in the programs runtime.
The heap can store far larger data structures.

# Generic Pointer

    void*

These are pointers without a type and cannot be dereferenced.

# Parameter Passing to Functions

Changing local variables does not effect the variables passed to the 
function unless they are pointers.

# Strings

```
    char* string
```

# Multidimensional Arrays

int array[M][N] - 2D Array

##1D array

``` C
    int* array = malloc(sizeof(int) * M * N);
```

Look up with:
    
```
    arr[i*M+j] 
```

Where i and j are the rows an columns.

##2D array

``` C
    int** arr = malloc(sizeof(int*) * M);

    for (int i = 0; i < M; i++) {
	arr[i] = malloc(sizeof(int) * N);
    }

    arr[i][j]
```
  
#Files

The type for C standard I/O files is FILE\*

To interact use fopen(), use fclose() when finished.

##Special File Types

*   stdin is for reading from the console
*   stdout if for writing to the console
*   stderr is for writing errors to the console

##Reading Files

``` C
    FILE* in = fopen(<filename>, "r");
    do {
	int c = fgetc(in);

	if (c != EOF) {
	    continue...
	}

    } while (!feof(in));
```

If a file cannot be opened or does not exist, NULL is returned from
fopen().

errno returns the error number if file cannot be opened. perror() prints
a readable error message to stderr with prefix.

``` C
    perror("\<prefix\>");	
```

#Comma operator

Evaluates expressions in order give. 

#Output Functions

*   fprintf() - Handles printing formatted string to output
*   fputc() - Handles printing char data to the output
*   fputs() - Handles printing string data to the ouput
*   fwrite() - Handles printing binary data to the output

##Buffered Output

Buffers need to be flushed from memory to output to stream. 

#Input Functions

*   fgets() 
*   fgetc()
*   fread()

##Read Formatted Input

fscanf() reads input stream and returns typed pointers.
Ignores whitespace until it reads format specifier.

sscanf() reads string for format specifiers and returns typed pointer. 

#Preprocessor

Runs before the main compile and deals with # directives.

*   #define - Performs textual substitution
	- Expanded #define CUBE(X) ((X) * (X) * (X)) substitutes into X
*   #include - Includes library/header/etc

##Conditional Compilation

Header guards stop redefinition of defintions using conditional statements.
```
    #ifndef DEF
    #define DEF
	//Definitions here
    #endif
```

#Enums

Store states in a word that corresponds to an integer value.

```
    enum Day {
	SUNDAY = 0,
	MONDAY = 1,
	...
    };

    enum Day d = TUESDAY;
```

#Switch

Can be used with any integer-like type.
case statements must be constant.
Missing break statements cause fall throw.

```
    switch(d) {
	case SUNDAY:
	    ...
	    break;
	case MONDAY:
	    ...
	    break;
	default:
	    ...
	    break;
    };
```

#Break

Breaks out of inner most loop or switch stament 

#Continue


