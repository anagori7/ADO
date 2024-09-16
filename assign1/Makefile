CC := gcc
CFLAGS := -g -Wall
SRC := dberror.c storage_mgr.c test_assign1_1.c
OBJ := dberror.o storage_mgr.o test_assign1_1.o

#Phony Targets
.PHONY: all clean run

#Default Target
all: test_assign1_1

#Building the executable
test_assign1_1: $(OBJ)
	$(CC) $(CFLAGS) -o test_assign1_1 $(OBJ)

#Compiling source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

#Running the program
run: test_assign1_1
	./test_assign1_1

#Cleaning up generated files
clean:
	rm -rf test_assign1_1 *.o
