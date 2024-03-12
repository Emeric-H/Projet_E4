# Makefile

CC = gcc
CFLAGS = 
LDFLAGS = -lfuse3

SOURCE = tst1.c #the source code who creates the fileSystem
EXECUTABLE = tst1 # the name of the executable that will be executated
LOG_DIR = ./logs # name of the log directory
MOUNT_DIR = ./mnt # name of the directory in which we will mount our fileSystem
Output.log = output.log # name of the output s directory of the command
Error.log = error.log # name of the error s directory of the the command

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	mkdir $(LOG_DIR)
	mkdir $(MOUNT_DIR)
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE) $(LDFLAGS)
	
run: $(EXECUTABLE)
	./$(EXECUTABLE) -f -s -d $(MOUNT_DIR) > $(LOG_DIR)/$(Output.log) 2> $(LOG_DIR)/$(Error.log)


clean:
	rm -f ./$(EXECUTABLE) $(LOG_DIR)/$(Output.log) $(LOG_DIR)/$(Error.log)
	rmdir $(MOUNT_DIR) $(LOG_DIR)
