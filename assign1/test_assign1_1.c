#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

//Current Test Name
char *testName;

//Test Page File Name
#define TESTPF "test_pagefile.bin"

//Declaring Test Functions
static void testCreateOpenClose(void);
static void testSinglePageContent(void);

//Main function to run all the tests
int main(void)
{
  testName = "";

  initStorageManager();

  testCreateOpenClose();
  testSinglePageContent();

  return 0;
}

//Test to create, open and closing a page file
void testCreateOpenClose(void)
{
  SM_FileHandle fh;

  testName = "Test Create Open and Close methods";

  //Creating a new page file
  TEST_CHECK(createPageFile(TESTPF));

  //Opening the page file
  TEST_CHECK(openPageFile(TESTPF, &fh));

  //Checking if the file name is correct
  ASSERT_TRUE(strcmp(fh.fileName, TESTPF) == 0, "Filename is Correct!");
  
  //Checking if the new file has 1 page
  ASSERT_TRUE((fh.totalNumPages == 1), "Expect 1 page in new file..");

  //Checking if the current page position is at the start(0)
  ASSERT_TRUE((fh.curPagePos == 0), "Freshly opened file page position should be 0");

  //Closing the file
  TEST_CHECK(closePageFile(&fh));

  //Deleting the file
  TEST_CHECK(destroyPageFile(TESTPF));

  //Trying to open the deleted file. It should get an error
  ASSERT_TRUE((openPageFile(TESTPF, &fh) != RC_OK), "opening a non-existing file should return an error...");

  TEST_DONE();
}

//Test Reading and Writing data to a page file
void testSinglePageContent(void)
{
  SM_FileHandle fh;
  SM_PageHandle ph;
  int i;

  testName = "Test Single Page Content";

  //Allocating memory for One Page
  ph = (SM_PageHandle) malloc(PAGE_SIZE);
  if (ph == NULL)
  {
    printf("Memory allocation is failed for page handle...\n");
    exit(1); //Exit if memory allocation fails
  }

  //Creating and opening a new page file
  TEST_CHECK(createPageFile(TESTPF));
  TEST_CHECK(openPageFile(TESTPF, &fh));
  printf("Created and Opened file..\n");

  //Reading the first page from the file
  TEST_CHECK(readFirstBlock(&fh, ph));

  //Checking that the page is NULL
  for (i = 0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "Expected zero byte in first page of freshly initialized page..");
  printf("First block was empty\n");

  //Filling the page with digits 0 to 9 and writing back to the file
  for (i = 0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
  TEST_CHECK(writeBlock(0, &fh, ph));
  printf("Writing the first block\n");

  //Reading the page again and also verifying if the data is correct
  TEST_CHECK(readFirstBlock(&fh, ph));
  for (i = 0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "Character in page read from disk is the one we expected.");
  printf("Reading first block\n");

  //Closing the file
  TEST_CHECK(closePageFile(&fh));

  //Deleting the file
  TEST_CHECK(destroyPageFile(TESTPF));

  //Making the allocated memory free
  free(ph);

  TEST_DONE();
}
