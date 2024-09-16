#include "dberror.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *RC_message = NULL;

char *errorMessage(RC error)
{
    char *message;

    switch (error)
    {
    case RC_OK:
        message = "Operation successful...";
        break;
    case RC_FILE_NOT_FOUND:
        message = "File not found!";
        break;
    case RC_FILE_HANDLE_NOT_INIT:
        message = "File handle not initialized!";
        break;
    case RC_WRITE_FAILED:
        message = "Write failed...";
        break;
    case RC_READ_NON_EXISTING_PAGE:
        message = "Attempted to read non-existing page.";
        break;
    case RC_NULL_MEM_PAGE:
        message = "Memory page pointer is NULL!!";
        break;
    case RC_SEEK_FAILED:
        message = "File Seek failed...";
        break;
    case RC_READ_FAILED:
        message = "File Read failed...";
        break;
    case RC_FILE_CLOSE_FAILED:
        message = "File Close failed...";
        break;
    case RC_FILE_DESTROY_FAILED:
        message = "File Destroy failed...";
        break;
    case RC_MEMORY_ALLOCATION_FAILED:
        message = "Memory Allocation failed...";
        break;
    
    default:
        message = "Unknown Error";
        break;
    }

    char *result = (char *)malloc(strlen(message) + 1);
    strcpy(result, message);
    return result;
}

