#------------------------------------------------------------------------------
# REF: Inspired by code from Gabriel Parmer found at
# REF: https://github.com/gwu-cs-os/evening_os_hour/blob/master/f19/
# REF:	10.2-makefiles/03_featureful_makefile/Makefile
#------------------------------------------------------------------------------

# Name of compiled binary file
SERVER_BINARY = auctioneer
CLIENT_BINARY = auctionclient

# Directory binary and object files will be built to
BINARYDIR =.

# Prefix for c files to build
SERVER_PREFIX = s
CLIENT_PREFIX = c

# List of included external library file names
LIBNAMES = csse2310a4 csse2310a3

# List of directories where .c files are located
CODEDIRS =. 

# List of directories where .h dependency files are located
INCDIRS =. /local/courses/csse2310/include

# List of directories where .c external library files are located
LIBDIRS = /local/courses/csse2310/lib

# Optional compiler flags
FLAGS = -g -pthread

# Optional library flags
LFLAGS = -lm -pthread

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
SERVER_CFILES = $(foreach D,$(CODEDIRS),$(wildcard $(D)/$(SERVER_PREFIX)*.c))
CLIENT_CFILES = $(foreach D,$(CODEDIRS),$(wildcard $(D)/$(CLIENT_PREFIX)*.c))

# Create object (.o) and dependency (.h) file lists from CFILES with 
# regular expression replacement
SERVER_OBJECTS = $(patsubst %.c,$(BINARYDIR)/%.o,$(SERVER_CFILES))
CLIENT_OBJECTS = $(patsubst %.c,$(BINARYDIR)/%.o,$(CLIENT_CFILES))

SERVER_DEPFILES = $(patsubst %.c,$(BINARYDIR)/%.d,$(SERVER_CFILES))
CLIENT_DEPFILES = $(patsubst %.c,$(BINARYDIR)/%.d,$(CLIENT_CFILES))

# Make all command makes binary with variable name
all: $(SERVER_BINARY) $(CLIENT_BINARY)

# Make binary with object list dependencies where
# $@ refers to the target (i.e BINARY) and $^ refers to the 
# dependencies (i.e OBJECTS)
$(SERVER_BINARY): $(SERVER_OBJECTS)
	$(CC) $(LIBFLAGS) -o $@ $^

$(CLIENT_BINARY): $(CLIENT_OBJECTS)
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
	rm -rf $(SERVER_BINARY) $(CLIENT_BINARY) $(SERVER_OBJECTS) $(CLIENT_OBJECTS)
	rm -rf $(SERVER_DEPFILES) $(CLIENT_DEPFILES)

# Include dependencies
-include $(SERVER_DEPFILES)
-include $(CLIENT_DEPFILES)

.PHONY: all clean 
