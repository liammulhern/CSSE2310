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

Placeholder (%) indicates the start of a format specifier that allows variables to be 
formatted in std out message. 

Placeholders must describe the type of variable being formatted e.g %d (int)i, %u (uint)

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
