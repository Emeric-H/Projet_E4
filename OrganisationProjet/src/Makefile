# Makefile

CC = gcc
CFLAGS = 
LDFLAGS = -lfuse3

SOURCE = Version0.c #the source code who creates the fileSystem
EXECUTABLE = Version0 # the name of the executable that will be executated
LOG_DIR = ./logs # name of the log directory
MOUNT_DIR = ./mnt # name of the directory in which we will mount our fileSystem
Output.log = ./logs/output.log # name of the output s directory of the command
Error.log = ./logs/error.log # name of the error s directory of the the command

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	mkdir $(LOG_DIR)
	mkdir $(MOUNT_DIR)
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE) $(LDFLAGS)
	
run: $(EXECUTABLE)
	./$(EXECUTABLE) -f -s -d $(MOUNT_DIR) > $(Output.log) 2>$(Error.log)


clean:
	rm -f ./$(EXECUTABLE) $(Output.log) $(Error.log)
	rmdir $(MOUNT_DIR) $(LOG_DIR)
