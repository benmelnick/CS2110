/**
 * @brief Header file for global macros, stuctures and fields to be used by the 21tenmo program.
 */
#ifndef __HW8_H__
#define __HW8_H__

/* DO NOT CHANGE THE PROVIDED MACROS/FUNCTION DEFINITIONS ;) */

// Max DB size
#define MAX_CSV_SIZE 256

// Name (length) validation macros
#define MAX_NAME_SIZE 49 // Without the NULL terminator
#define MIN_NAME_SIZE 2  // Without the NULL terminator

// Macros for validating accountNumber
#define FIRST_ACCOUNT_NUMBER    2110
#define MAX_ACCOUNT_NUMBER      2365

#define UNUSED_PARAM(x) ((void) x) // This macro is only used for turning off compiler errors initially
#define GET_ACCOUNT(accountNum) (arr[accountNum - FIRST_ACCOUNT_NUMBER]) // macro for indexing the arr/DB

// Account struct, matching the first row of the csv DB
typedef struct account {
    unsigned int accountNumber;         // The accountNumber associated, an accountNumber of ZERO indicates an invalid/closed account
    char personName[MAX_NAME_SIZE + 1]; // Name of the account holder (buffer contains the null terminator)
    float balance; // Current account balance
    unsigned int requesterNumber; // The accountNumber of the person requesting money from this account
    float requestAmount; // The requested amount to be sent by this account to requesterNumber
} Account;

// Global DB of accounts (sorted)
extern Account arr[MAX_CSV_SIZE];

// Number of elements in the database (arr)
extern unsigned int DBsize;

// Various states for when a user logs in @see 21tenmo.c
typedef enum state {
    MENU,           // Default state
    SHOW_BALANCE,
    MODIFY_BALANCE,
    SEND_MONEY,
    RECEIVE_MONEY,
    REQUESTS,
    USER_EXIT       // Main state for checking if user wants to log out
} State;

// Function prototypes for 21tenmo.c
void showMenu(unsigned int accountNumber);
State executeCommand(char choice, unsigned int accountNumber);
void modifyBalance(unsigned int accountNumber);
void sendMoney(unsigned int senderNumber);
int formatAndCheckCurrency(char *currencyString);
void requestMoney(unsigned int accountNumber);
void checkRequests(unsigned int accountNumber);
void cancelRequest(Account acc);
void removeChar(char *str, char remove);
void transferMoney(Account sendingAccount, Account receivingAccount, float amount);


// -------------- DO NOT MODIFY CODE BELOW --------------
// Entry point of the program when a user logs in
void userMenu(unsigned int accountNumber);

// Utility Function prototypes
void readCSV(void);                     /* Utility function for reading the database into the array*/
void writeCSV(void);                    /* Utility function for writing the array to the csv database */
void getLine(char *buffer, int size);   /* Utility function for reading input from the standard input */
void printArray(int numElements);       /* Utility function for printing out the first numElements of the array */
void myExit(int status);                /* Utility function for exiting your program */


Account getAccountForTAUseOnly(unsigned int accountNumber); // DO NOT USE/MODIFY
#endif
