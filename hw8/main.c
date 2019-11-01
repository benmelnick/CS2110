/**
 * @file main.c
 * @author Benjamin Melnick
 */

// DO NOT MODIFY THE INCLUDE(s) LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "21tenmo.h"
#include "errcodes.h"

// Initializing the global err number variable to default
unsigned int myErrNo = NO_ERROR;

/**
 * @brief helper function for printing out the usage of the program.
 */
void printUsage(void) {
    printf("Usage for adding a new account:      ./21tenmo -a \"personName\"\n");
    printf("Usage for logging in a current user: ./21tenmo -l accountNumber\n");
}

/**
 * @brief MAIN Method, responsible for parsing & validating arguments and
 * handling events (e.g. modifying the database, logging in user).
 */
int main(int argc, char const *argv[])
{
    readCSV(); // read the database when the program starts up

    //0th arg is always name of the program itself
    if (argc > 1) {
        if (strcmp(argv[1], "-a") != 0 && strcmp(argv[1], "-l") != 0) {
            myErrNo = ERR_INVALID_ARGS;
            printf("ERROR: Not a valid argument\n");
            printUsage();
            myExit(EXIT_FAILURE);
        } else {
            if (strcmp(argv[1], "-a") == 0) {
                // handle adding an account
                if (DBsize == MAX_CSV_SIZE) {
                    // DB is full, can't add another account
                    myErrNo = ERR_DB_FULL;
                    printf("ERROR: Database is currently full\n");
                    myExit(EXIT_FAILURE);
                }
                if (argc == 2) {
                    // only two arguments passed
                    myErrNo = ERR_INVALID_ARGS;
                    printf("ERROR: Please enter a name\n");
                    myExit(EXIT_FAILURE);
                }
                // read the name entered on command line
                char name[200] = {0};
                for (int i = 2; i < argc; i++) {
                    strcat(name, argv[i]);
                    if (i < argc - 1) {
                        strcat(name, " ");
                    }
                }
                if (strlen(name) < MIN_NAME_SIZE) {
                    // provided name is too small
                    myErrNo = ERR_INVALID_NAME;
                    printf("ERROR: Name must have at least two characters\n");
                    myExit(EXIT_FAILURE);
                } else if (strlen(name) > MAX_NAME_SIZE) {
                    // provided name is too large
                    myErrNo = ERR_INVALID_NAME;
                    printf("ERROR: Name cannot have more than 49 characters\n");
                    myExit(EXIT_FAILURE);
                }
                // create a new account
                unsigned int newAccountNum = FIRST_ACCOUNT_NUMBER + DBsize;
                Account newAccount = {newAccountNum, "", 0, 0, 0};
                strcpy(newAccount.personName, name);
                arr[DBsize] = newAccount; // add new account to global arr
                writeCSV(); //write new account to CSV
                printf("Welcome to 21tenmo, %s. Your account number is %i\n", newAccount.personName, newAccount.accountNumber);
                myExit(EXIT_SUCCESS);
            } else {
                // handle logging in
                if (argc == 2) {
                    // no account number passed in
                    myErrNo = ERR_INVALID_ARGS;
                    printf("ERROR: Please specify an account number\n");
                    myExit(EXIT_FAILURE);
                }
                unsigned int accountNum = strtoul(argv[2], NULL, 10);
                if (accountNum < FIRST_ACCOUNT_NUMBER || accountNum > MAX_ACCOUNT_NUMBER) {
                    // account number is not in the valid range
                    myErrNo = ERR_INVALID_ACC_NUM;
                    printf("ERROR: Invalid account number\n");
                    myExit(EXIT_FAILURE);
                }
                Account acc = GET_ACCOUNT(accountNum);
                if (acc.accountNumber == 0) {
                    // specified account number is inactive
                    myErrNo = ERR_INACTIVE_ACCOUNT;
                    printf("ERROR: Inactive account\n");
                    myExit(EXIT_FAILURE); // TODO do we exit ????
                }
                userMenu(acc.accountNumber);
            }
        }
    }

    //*** Do NOT remove this ***
    return EXIT_SUCCESS;
}
