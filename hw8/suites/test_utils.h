
#include <stdio.h>
#include <check.h>
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)



/**
 * @brief Create Test Case and Add Test Case to Suite
 * 
 * @note setup/teardown fixture functions are run before and after each unit test inside the address space of the test
 * @note TCase *tc = tcase_create("func") : Creates test case to be added to the suite
 * 
 * 
 * @param suite             - Create suite variable in some function and pass it in here
 * @param setup_fixture     - Setup function
 * @param teardown_fixture  - Teardown function : NULL if none
 * @param func              - Name of test being added to the test case
 * 
 */
#define tcase_hack(suite, setup_fixture, teardown_fixture, func) { \
    TCase *tc = tcase_create(STRINGIFY(func)); \
    tcase_add_checked_fixture(tc, setup_fixture, teardown_fixture); \
    tcase_add_test(tc, func); \
    suite_add_tcase(s, tc); \
}


extern int first_or_second;
extern int testing_signal;
extern char test_account[100];
extern char test_amount[100];

void resetGlobalArray(void);
void resetGlobalArray_FILL_ARRAY_MAX(void);
void reset_error_and_test_codes(void);
