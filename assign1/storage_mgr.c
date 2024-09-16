#include "storage_mgr.h"
#include<stdio.h>
#include<stdlib.h>
#include "dberror.h"
#include <string.h>

void initStorageManager()
{
    //No initialization required yet
}

RC createPageFile(char *fileName)
{
    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
    {
        RC_message = "Could not create file.";
        return RC_WRITE_FAILED;
    }

    //Initializing one page with zero bytes
    char *emptyPage = (char *)calloc(PAGE_SIZE, sizeof(char));
    size_t written = fwrite(emptyPage, sizeof(char), PAGE_SIZE, file);

    if (written != PAGE_SIZE)
    {
        free(emptyPage);
        fclose(file);
        RC_message = "Could not write initial page.";
        return RC_WRITE_FAILED;
    }

    free(emptyPage);  //Free Allocated Memory
    fclose(file);     //Closing the file

    RC_message = "File created successfully.";
    return RC_OK;
}

RC openPageFile(char *filename, SM_FileHandle *fileHandle)
{
    FILE *file = fopen(filename, "rb+");
    if (file == NULL)
    {
        RC_message = "File does not exist.";
        return RC_FILE_NOT_FOUND;
    }

    fileHandle->curPagePos = 0;
    fileHandle->fileName = (char *)malloc(strlen(filename) + 1);
    if (fileHandle->fileName == NULL)
    {
        fclose(file);
        RC_message = "Memory allocation failed.";
        return RC_MEMORY_ALLOCATION_FAILED;
    }
    strcpy(fileHandle->fileName, filename);

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fileHandle->totalNumPages = (int)(fileSize / PAGE_SIZE);
    if (fileSize % PAGE_SIZE != 0)
    {
        fileHandle->totalNumPages += 1; 
    }
    fseek(file, 0, SEEK_SET); 

    fileHandle->mgmtInfo = file;
    return RC_OK;
}

RC closePageFile(SM_FileHandle *fileHandle)
{
    FILE *file = (FILE *)fileHandle->mgmtInfo;
    if (fclose(file) == 0)
    {
        free(fileHandle->fileName);
        fileHandle->mgmtInfo = NULL;
        RC_message = "File is closed successfully...";
        return RC_OK;
    }
    else
    {
        RC_message = "Failed to close the file...";
        return RC_FILE_CLOSE_FAILED; //Error code
    }
}

RC destroyPageFile(char *filename)
{
    if (remove(filename) == 0)
    {
        RC_message = "File is removed successfully...";
        return RC_OK;
    }
    else
    {
        RC_message = "Couldn't remove the file...";
        return RC_FILE_DESTROY_FAILED; //Error code
    }
}

RC readBlock(int pageNum, SM_FileHandle *fileHandle, SM_PageHandle memPage)
{
    if (fileHandle == NULL || fileHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle is not initialized.";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if (memPage == NULL)
    {
        RC_message = "Memory page pointer is Null!";
        return RC_NULL_MEM_PAGE; //Error code
    }
    if (pageNum < 0 || pageNum >= fileHandle->totalNumPages)
    {
        RC_message = "Attempting to read a non-existing page...";
        return RC_READ_NON_EXISTING_PAGE;
    }

    FILE *file = (FILE *)fileHandle->mgmtInfo;
    long offset = pageNum * PAGE_SIZE;
    if (fseek(file, offset, SEEK_SET) != 0)
    {
        RC_message = "Failed to seek to the desired page...";
        return RC_SEEK_FAILED; //Error code
    }
    size_t bytesRead = fread(memPage, sizeof(char), PAGE_SIZE, file);
    if (bytesRead < PAGE_SIZE)
    {
        RC_message = "Failed to read the complete page...";
        return RC_READ_FAILED; //Error code
    }
    fileHandle->curPagePos = pageNum;
    return RC_OK;
}

int getBlockPos(SM_FileHandle *fileHandle)
{
    if (fileHandle == NULL)
    {
        //Handle Error Correctly
        return -1; //Return invalid page number
    }
    return fileHandle->curPagePos;
}

RC readFirstBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage)
{
    if (fileHandle == NULL || fileHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle is not initialized...";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    return readBlock(0, fileHandle, memPage);
}

RC readCurrentBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage)
{
    if (fileHandle == NULL || fileHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle is not initialized...";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    return readBlock(fileHandle->curPagePos, fileHandle, memPage);
}

RC readNextBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage)
{
    if (fileHandle == NULL || fileHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle not initialized...";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    int nextPage = fileHandle->curPagePos + 1;
    if (nextPage >= fileHandle->totalNumPages)
    {
        RC_message = "Next block doesn't exist...";
        return RC_READ_NON_EXISTING_PAGE;
    }
    return readBlock(nextPage, fileHandle, memPage);
}

RC readLastBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage)
{
    if (fileHandle == NULL || fileHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle is not initialized...";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    int lastPage = fileHandle->totalNumPages - 1;
    if (lastPage < 0)
    {
        RC_message = "No blocks in the file...";
        return RC_READ_NON_EXISTING_PAGE;
    }
    return readBlock(lastPage, fileHandle, memPage);
}

RC readPreviousBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage)
{
    if (fileHandle == NULL || fileHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle is not initialized...";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    int prevPage = fileHandle->curPagePos - 1;
    if (prevPage < 0)
    {
        RC_message = "Previous block doesn't exist...";
        return RC_READ_NON_EXISTING_PAGE;
    }
    return readBlock(prevPage, fileHandle, memPage);
}

RC writeBlock(int pageNum, SM_FileHandle *fileHandle, SM_PageHandle memPage)
{
    if (fileHandle == NULL || fileHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle is not initialized...";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if (memPage == NULL)
    {
        RC_message = "Memory page pointer is Null!";
        return RC_NULL_MEM_PAGE; //Error code
    }
    if (pageNum < 0)
    {
        RC_message = "Invalid page number.";
        return RC_WRITE_FAILED;
    }

    //Ensuring the capacity
    if (pageNum >= fileHandle->totalNumPages)
    {
        RC rc = ensureCapacity(pageNum + 1, fileHandle);
        if (rc != RC_OK)
        {
            return rc;
        }
    }

    FILE *file = (FILE *)fileHandle->mgmtInfo;
    long offset = pageNum * PAGE_SIZE;
    if (fseek(file, offset, SEEK_SET) != 0)
    {
        RC_message = "Failed to seek to the desired page.";
        return RC_SEEK_FAILED;
    }
    size_t bytesWritten = fwrite(memPage, sizeof(char), PAGE_SIZE, file);
    if (bytesWritten < PAGE_SIZE)
    {
        RC_message = "Failed to write the complete page.";
        return RC_WRITE_FAILED;
    }

    //Flushing the output buffer
    fflush(file);

    //Updating the TotalNumPages if required
    long currentFileSize;
    if (fseek(file, 0, SEEK_END) == 0)
    {
        currentFileSize = ftell(file);
        int newTotalPages = (int)(currentFileSize / PAGE_SIZE);
        if (currentFileSize % PAGE_SIZE != 0)
        {
            newTotalPages += 1;
        }
        fileHandle->totalNumPages = newTotalPages;
    }
    else
    {
        RC_message = "Failed to update total no. of pages...";
        return RC_SEEK_FAILED;
    }

    fileHandle->curPagePos = pageNum;
    return RC_OK;
}

RC writeCurrentBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage)
{
    if (fileHandle == NULL || fileHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle is not initialized...";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    return writeBlock(fileHandle->curPagePos, fileHandle, memPage);
}

RC appendEmptyBlock(SM_FileHandle *fHandle)
{
    if (fHandle == NULL || fHandle->mgmtInfo == NULL)
    {
        RC_message = "File handle is not initialized...";
        return RC_FILE_HANDLE_NOT_INIT;
    }
    SM_PageHandle emptyBlock = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));
    if (emptyBlock == NULL)
    {
        RC_message = "Memory allocation has failed...";
        return RC_MEMORY_ALLOCATION_FAILED; //Error code
    }
    FILE *file = (FILE *)fHandle->mgmtInfo;
    if (fseek(file, 0, SEEK_END) != 0)
    {
        free(emptyBlock);
        RC_message = "Failed to find to the end of the file...";
        return RC_SEEK_FAILED;
    }
    size_t bytesWritten = fwrite(emptyBlock, sizeof(char), PAGE_SIZE, file);
    free(emptyBlock);
    if (bytesWritten < PAGE_SIZE)
    {
        RC_message = "Failed to write empty block...";
        return RC_WRITE_FAILED;
    }

    // Flushing the output buffer
    fflush(file);

    fHandle->totalNumPages += 1;
    fHandle->curPagePos = fHandle->totalNumPages - 1;
    return RC_OK;
}

RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
    if (fHandle->totalNumPages >= numberOfPages)
    {
        return RC_OK;
    }
    int pagesToAdd = numberOfPages - fHandle->totalNumPages;
    for (int i = 0; i < pagesToAdd; i++)
    {
        RC rc = appendEmptyBlock(fHandle);
        if (rc != RC_OK)
        {
            return rc;
        }
    }
    return RC_OK;
}
