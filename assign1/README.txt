Storage Manager Project

In this project we have implemented a simple storage manager in C which is designed to manage fixed-size pages within a page file. The storage manager will provide a basic file operations such as creating, opening, reading, writing and destroying the page files. This project is a foundational component for building more complex database systems.

---

Overview:

The storage manager provides an interface for managing page files, including:

1. Page File Management: Create, open, close, and destroy page files.
2. Page Operations: Read & Write pages including specific blocks such like the first, last, previous, next and current pages.
3. Capacity Management: Append empty pages to ensure a page file has a min. no. of pages.
4. Error Handling: Comprehensive error codes and messages for debugging and robustness.

---

Solution Description:

Data Structures:

1. `SM_FileHandle`

It is a structure that represents a file handle for a page file. It includes the following:

- `char *fileName`: Name of the page file.
- `int totalNumPages`: Total no. of pages in the file.
- `int curPagePos`: Page number or Current position in the page file.
- `void *mgmtInfo`: Pointer to hold any necessary management information (example: `FILE *` pointer).

2. `SM_PageHandle`

It is defined as a `char *` and it represents a block of memory (a page) of size `PAGE_SIZE` bytes.

Function Implementations

1. `initStorageManager()`

- Purpose: It initializes the storage manager. Currently there is no initialization required.
- Implementation: Empty function as per the requirements.

2. `createPageFile(char *fileName)`

- Purpose: To create a new page file with one page initialized to zero bytes.
- Implementation:
  - It open a file in write-binary mode (`"wb"`).
  - It allocates memory for an empty page (`PAGE_SIZE` bytes) initialized to zero.
  - It writes the empty page to the file.
  - It free allocated memory and close the file.
  - It return error codes if operations fails.

3. `openPageFile(char *fileName, SM_FileHandle *fHandle)`

- Purpose: To open an existing page file and to initialize the file handle.
- Implementation:
  - Opens the file in Read-Write-Binary mode (`"rb+"`).
  - Allocate memory for the `fileName` in the file handle.
  - It determine the total no. of pages by calculating the file size divided by `PAGE_SIZE`.
  - It initializes `curPagePos` to `0` and sets `mgmtInfo` to the `FILE *` pointer.
  - Return error codes if the file doesn't exist or memory allocation fail.

4. `closePageFile(SM_FileHandle *fHandle)`

- Purpose: To close an open page file.
- Implementation:
  - Close the file using `fclose`.
  - Free the allocated `fileName` in the file handle.
  - Set `mgmtInfo` to `NULL`.
  - Return error codes if file can't be closed.

5. `destroyPageFile(char *fileName)`

- Purpose: To delete a page file from the file system.
- Implementation:
  - It uses the `remove` function to delete the file.
  - It returns error codes if the file can't be removed.

6. `readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)`

- Purpose: To read the specified block(page) from the page file into memory.
- Implementation:
  - It validates the `fHandle` and `memPage` pointers.
  - It check if `pageNum` is in the valid range.
  - It uses `fseek` to move to the correct position in the file.
  - It reads `PAGE_SIZE` bytes into `memPage`.
  - Update `curPagePos` in the file handle.
  - Return error codes for invalid operations.

7. `writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)`

- Purpose: To write a block of data to a specified page in the page file.
- Implementation:
  - Validate pointers and page number.
  - Ensure the capacity of the file using `ensureCapacity`.
  - Use `fseek` to move to the correct position.
  - Writes `PAGE_SIZE` bytes from `memPage` to the file.
  - Flush the file buffer using `fflush`.
  - Update `totalNumPages` and `curPagePos`.
  - Return error codes for invalid operations.

8. `appendEmptyBlock(SM_FileHandle *fHandle)`

- Purpose: To append an empty page(initialized to zero) to the end of the page file.
- Implementation:
  - Validate the `fHandle`.
  - Move to the end of the file using `fseek`.
  - Write an empty page to the file.
  - Flush the file buffer.
  - Update `totalNumPages` & `curPagePos`.
  - Return error codes if operations fail.

9. `ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)`

- Purpose: To ensure the page file has at least `numberOfPages` pages.
- Implementation:
  - Compare `numberOfPages` with `totalNumPages`.
  - Append empty pages as needed using `appendEmptyBlock`.
  - Return appropriate error codes.

10. Additional Functions:

- Purpose: These functions are implemented to provide additional functionalities for navigating and manipulating pages within the file:
- `getBlockPos`, `readFirstBlock`, `readPreviousBlock`, `readCurrentBlock`, `readNextBlock`, `readLastBlock`, `writeCurrentBlock` 

Error Handling:

- Error codes are defined in `dberror.h` and the meaningful messages are assigned to `RC_message`.
- Function return appropriate error codes based on the success or failure or operations.
- Common error codes are: `RC_OK`, `RC_FILE_NOT_FOUND`, `RC_WRITE_FAILED`, `RC_READ_NON_EXISTING_PAGE`

Design Decisions:

- File Management: The `mgmtInfo` field in `SM_FileHandle` is used to store the `FILE *` pointer which allows multiple files to be managed simultaneously without any global variables.
- Memory Management: Dynamically allocated memory is properly freed to prevent memory leaks.
- Error Codes: Custom error codes are defined for specific error conditions which enhances debuggability.
- Modularity: Functions are designed to be modular with common operations like seeking and Reading/Writing encapsulated within functions.

---

Compilation and Execution:

Prerequisites:

- C Compiler: Any GCC or any C compiler.
- Make: Utility to automate the build process.

Compilation Steps:

1. Navigate to the Project Directory:

   Open terminal and navigate to the directory containing the project files after cloning or downloading the project files.

2. Compile the Project:

   Run the following command to compile source files and create the executable `test_assign1_1`:

   ```bash
   make
   ```

Running the Tests:

Now execute the test program to verify that all the functionalities are working:

```bash
./test_assign1_1
```

Expected Output:

```
Created and opened file
First block was empty
Writing first block
Reading first block
[test_assign1_1.c-Test Single Page Content-L118-22:15:53] OK: Finished test

MacBook-Pro:Assignment-1 ansafnagori$ 
```

All tests should pass without errors. If a test fail then the program will output a message with the failure.

Cleaning Up:

Run the following command to remove the compiled object files and executables:

```bash
make clean
```

---

Testing:

- This project include a test suite (`test_assign1_1.c`) that verifies the storage manager's functionalities.
- Test Cases Include:
  - Creating, Opening and Closing a page file.
  - Reading from and Writing to a single page.
  - Validating the content of pages after operations.

---

Memory Leak Checks:

It is very important to make sure that the program doesn't have memory leaks.

Using Valgrind:

If Valgrind is installed then simply run the following command to report any memory leaks or invalid memory accesses.:

```bash
valgrind --leak-check=full ./test_assign1_1
```

---

Project Files:

- storage_mgr.h: Includes header file with data structures and function declarations.
- storage_mgr.c: Includes implementation of storage manager functions.
- dberror.h and dberror.c: Includes fefinition and implementation for error handling.
- test_helper.h: Includes macros and utilities for testing.
- test_assign1_1.c: Includes test suite for verifying functionalities.
- Makefile: To automate the compilation process.
- README.txt: Documentation.

---

Conclusion:

This storage manager project satisfies all the requirements specified in the assignment. It provides a robust and modular solution for managing page files, including comprehensive error handling and memory management.

---