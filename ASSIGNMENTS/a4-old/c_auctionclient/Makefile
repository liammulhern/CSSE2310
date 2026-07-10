#------------------------------------------------------------------------------
# REF: Inspired by code from Gabriel Parmer found at
# REF: https://github.com/gwu-cs-os/evening_os_hour/blob/master/f19/
# REF:	10.2-makefiles/03_featureful_makefile/Makefile
#------------------------------------------------------------------------------

# Name of compiled binary file
BINARY = auctionclient

# Directory binary and object files will be built to
BINARYDIR =.

# Prefix for c files to build
CFILEPREFIX = c

# List of included external library file names
LIBNAMES = csse2310a4 csse2310a3

# List of directories where .c files are located
CODEDIRS =. 

# List of directories where .h dependency files are located
INCDIRS =. /local/courses/csse2310/include

# List of directories where .c external library files are located
LIBDIRS = /local/courses/csse2310/lib

# Optional compiler flags
FLAGS = -g 

# Optional library flags
LFLAGS = -lm -lpthread

# Compiler
CC = gcc

# Optimisation flags
OPT =

#------------------------------------------------------------------------------

# Compile object files that encode make rules for the .h dependencies
DEPFLAGS = -MP -MD

# Automatically add the -I onto each include directory
INCDIRFLAGS = $(foreach D,$(INCDIRS),-I$(D))

# Automatically add the -L onto each include library directory
LIBDIRFLAGS = $(foreach D,$(LIBDIRS),-L$(D))

# Automatically add the -l onto each Include library name
LIBNAMEFLAGS = $(foreach D,$(LIBNAMES),-l$(D)) 

# Generate library link flags 
LIBFLAGS = $(LIBDIRFLAGS) $(LIBNAMEFLAGS) $(LFLAGS)

# Generate C flags combination
CFLAGS = -Wall -Werror -pedantic -std=gnu99 $(FLAGS)
CFLAGS += $(INCDIRFLAGS) $(OPT) $(DEPFLAGS)

# Iterate over directories where .c files are present and use regular 
# expression completions to create list of file names 
CFILES = $(foreach D,$(CODEDIRS),$(wildcard $(D)/$(CFILEPREFIX)_*.c))

# Create object (.o) and dependency (.h) file lists from CFILES with 
# regular expression replacement
OBJECTS = $(patsubst %.c,$(BINARYDIR)/%.o,$(CFILES))
DEPFILES = $(patsubst %.c,$(BINARYDIR)/%.d,$(CFILES))

# Make all command makes binary with variable name
all: $(BINARY)

# Make binary with object list dependencies where
# $@ refers to the target (i.e BINARY) and $^ refers to the 
# dependencies (i.e OBJECTS)
$(BINARY): $(OBJECTS)
	$(CC) $(LIBFLAGS) -o $@ $^

# Only want the .c file dependency as the header files are compiled
# seperately using the -MP -MD flags. Use $< instead of $^ where
# $< refers to the first .c file dependency.
$(BINARYDIR)/%.o:%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Remove binary, object files, and dependency files to clean directory
# of all compiled files
clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

# Include dependencies
-include $(DEPFILES)

.PHONY: all clean 
