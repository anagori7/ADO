//Include Gaurd
#ifndef DBERROR_H
#define DBERROR_H

// Error codes
#define RC_NULL_MEM_PAGE 1001            //When memory page is null
#define RC_SEEK_FAILED 1002              //When moving file pointer fail
#define RC_READ_FAILED 1003              //When reading from file fail
#define RC_FILE_CLOSE_FAILED 1004        //When closing file fail
#define RC_FILE_DESTROY_FAILED 1005      //When deleting file fail
#define RC_MEMORY_ALLOCATION_FAILED 1006 //When allocating memory fail

//Including standard input/output library
#include <stdio.h>

//Defining the page size
#define PAGE_SIZE 4096

//Additional error codes
#define RC_OK 0                      //No error/Operation Successful
#define RC_FILE_NOT_FOUND 1          //When file can't be found
#define RC_FILE_HANDLE_NOT_INIT 2    //When the file handle is not initialized
#define RC_WRITE_FAILED 3            //When writing to a file fail
#define RC_READ_NON_EXISTING_PAGE 4  //When trying to read a page that does not exist

//Let's define "RC" as a new name for "int" which will make it clear that functions return a return code
typedef int RC;

//Variable to hold error message when any error occur
extern char *RC_message;

//Function to print error message based on the Return Code
void printError(RC error);

//Function to return a string containing the error message
char *errorMessage(RC error);

//Macro for error handling
#define THROW(rc, message) \
  do { \
    RC_message = message; \
    return rc;            \
  } while (0)


//Macro to check successfull function call
#define CHECK(code) \
  do { \
    RC rc_internal = (code);               \
    if (rc_internal != RC_OK) {            \
      char *message = errorMessage(rc_internal); \
      printf("ERROR: %s\n", message);      \
      free(message);                       \
      exit(1);                             \
    } \
  } while(0)


#endif
