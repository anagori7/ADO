#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Variable for storing the current test name
extern char *testName;

//Shortcut for test info
#define TEST_INFO  __FILE__, testName, __LINE__, __TIME__

//Checking the return code and exit if there is any error
#define TEST_CHECK(code)                                               \
    do {                                                               \
        int rc_internal = (code);                                      \
        if (rc_internal != RC_OK)                                      \
        {                                                              \
            char *message = errorMessage(rc_internal);                 \
            printf("[%s-%s-L%i-%s] FAILED: Operation returned an error: %s\n", TEST_INFO, message); \
            free(message);                                             \
            exit(1);                                                   \
        }                                                              \
    } while(0)

//Checking if two strings are equal
#define ASSERT_EQUALS_STRING(expected, real, message)                  \
    do {                                                               \
        if (strcmp((expected), (real)) != 0)                           \
        {                                                              \
            printf("[%s-%s-L%i-%s] FAILED: Expected <%s> but it was <%s>: %s\n", TEST_INFO, expected, real, message); \
            exit(1);                                                   \
        }                                                              \
        printf("[%s-%s-L%i-%s] OK: Expected <%s> and it was <%s>: %s\n", TEST_INFO, expected, real, message); \
    } while(0)

//Check if two int are equal
#define ASSERT_EQUALS_INT(expected, real, message)                     \
    do {                                                               \
        if ((expected) != (real))                                      \
        {                                                              \
            printf("[%s-%s-L%i-%s] FAILED: Expected <%i> but it was <%i>: %s\n", TEST_INFO, expected, real, message); \
            exit(1);                                                   \
        }                                                              \
        printf("[%s-%s-L%i-%s] OK: Expected <%i> and it was <%i>: %s\n", TEST_INFO, expected, real, message); \
    } while(0)

//Checking if a condition is true
#define ASSERT_TRUE(condition, message)                                \
    do {                                                               \
        if (!(condition))                                              \
        {                                                              \
            printf("[%s-%s-L%i-%s] FAILED: Expected True: %s\n", TEST_INFO, message); \
            exit(1);                                                   \
        }                                                              \
        printf("[%s-%s-L%i-%s] OK: Expected True: %s\n", TEST_INFO, message); \
    } while(0)

//Checking if a method returns error code
#define ASSERT_ERROR(expected, message)                                \
    do {                                                               \
        int result = (expected);                                       \
        if (result == RC_OK)                                           \
        {                                                              \
            printf("[%s-%s-L%i-%s] FAILED: Expected an error: %s\n", TEST_INFO, message); \
            exit(1);                                                   \
        }                                                              \
        printf("[%s-%s-L%i-%s] OK: Expected an error and was RC <%i>: %s\n", TEST_INFO, result, message); \
    } while(0)

//Defining test completion
#define TEST_DONE()                                                    \
    do {                                                               \
        printf("[%s-%s-L%i-%s] OK: Finished test\n\n", TEST_INFO);     \
    } while(0)

#endif
