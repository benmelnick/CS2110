/**
 * @file 21tenmo.c
 * @author Benjamin Melnick
 */

// DO NOT MODIFY THE INCLUDE(s) LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "21tenmo.h"
#include "errcodes.h"

/**
 * @brief method for executing the command selected from the menu.
 *
 * @param choice a single number character indicating the command to be executed
 * @param accountNumber for retrieving info from the DB
 * @return State the state of the program (USER_EXIT is the 'finishing' state)
 */
State executeCommand(char choice, unsigned int accountNumber) {
    switch (choice) {
        case '1':
            printf("Current balance: $%.2f", GET_ACCOUNT(accountNumber).balance);
            return SHOW_BALANCE;
        case '2':
            modifyBalance(accountNumber);
            return MODIFY_BALANCE;
        case '3':
            sendMoney(accountNumber);
            return SEND_MONEY;
        case '4':
            requestMoney(accountNumber);
            return RECEIVE_MONEY;
        case '5':
            checkRequests(accountNumber);
            return REQUESTS;
        case '6':
            // until user exits keep all changes local to arr
            writeCSV(); // do not write to DB until user wants to exit
            return USER_EXIT;
        default:
            printf("ERROR: Invalid choice entered (%c)\n", choice);
            return MENU;
    }
}

/**
 * @brief helper method for depositing/withdrawing fund to the active account.
 *
 * @param accountNumber for retrieving info from the DB
 */
void modifyBalance(unsigned int accountNumber) {
    Account acc = GET_ACCOUNT(accountNumber);
    printf("Enter an amount to deposit/withdraw (for withdraw add a '-' before the number): $");

    char currencyInput[20];
    getLine(currencyInput, sizeof(currencyInput)); // read user input from command line
    if (formatAndCheckCurrency(currencyInput) == 1) {
        // error in the string, return
        return;
    }
    float difference = strtof(currencyInput, NULL); // parse formatted input to float
    if(acc.balance + difference < 0) {
        // balance can never be negative
        myErrNo = ERR_NOT_ENOUGH_FUNDS;
        printf("ERROR: Invalid amount, cannot withdraw more than your current balance\n");
    } else {
        acc.balance = acc.balance + difference;
        arr[accountNumber - FIRST_ACCOUNT_NUMBER] = acc;
    }
}

/**
 * @brief helper method for sending money to another account from current active account.
 *
 * @param senderNumber for retrieving info from the DB
 */
void sendMoney(unsigned int senderNumber) {
    Account sendingAccount = GET_ACCOUNT(senderNumber);

    printf("Enter the accountNumber of the receiver: ");
    char accountInput[5];
    getLine(accountInput, sizeof(accountInput));
    unsigned int receivingAccountNum = strtoul(accountInput, NULL, 10);
    if (receivingAccountNum < FIRST_ACCOUNT_NUMBER || receivingAccountNum > MAX_ACCOUNT_NUMBER) {
        // account number is not in the valid range
        myErrNo = ERR_INVALID_ACC_NUM;
        printf("ERROR: Invalid account number\n");
        return;
    }
    Account receivingAccount = GET_ACCOUNT(receivingAccountNum);
    if (receivingAccount.accountNumber == 0) {
        // specified account number is inactive
        myErrNo = ERR_INACTIVE_ACCOUNT;
        printf("ERROR: The specified account is inactive\n");
    } else if (senderNumber == receivingAccount.accountNumber) {
        myErrNo = ERR_SELF_REQUEST;
        printf("ERROR: Cannot send money to yourself\n");
    } else {
        printf("Enter the amount you want to send: $");

        char currencyInput[20];
        getLine(currencyInput, sizeof(currencyInput));
        if (formatAndCheckCurrency(currencyInput) == 1) {
            // error in the string, return
            return;
        }
        // if no errors, we have a formatted positive OR negative string
        if (strchr(currencyInput, '-') != NULL) {
            // user entered negative amount
            myErrNo = ERR_INVALID_AMOUNT;
            printf("ERROR: You entered a negative amount\n");
        } else {
            // positive amount entered, check if you have enough money
            float moneyToSend = strtof(currencyInput, NULL); // parse formatted input to float
            if (moneyToSend > sendingAccount.balance) {
                myErrNo = ERR_NOT_ENOUGH_FUNDS;
                printf("ERROR: Balance is too low to send this amount\n");
            } else {
                // perform the operation
                transferMoney(sendingAccount, receivingAccount, moneyToSend);
                printf("Money sent!\n");
            }
        }
    }
}

/**
 * @brief helper method for formatting and checking a currencyString
 *
 * In this program, a currencyString can have at most 1 dot and/or
 * at most one '-' sign, and many commas. Comma separtion
 * validation is not a requirement.
 *
 * Note: In case of error, the passed in string should NOT be modified
 *
 * @param currencyString the string to validate/format
 * @return int 0 on successful formatting, nonzero upon error
 */
int formatAndCheckCurrency(char *currencyString) {
    // search for occurences of dot
    char *dot1 = strchr(currencyString, '.'); // returns either null or a pointer
    if (dot1 != NULL) {
        // input contains a dot, check for a second
        dot1++;
        char *dot2 = strchr(dot1, '.');
        if (dot2 != NULL) {
            // input contains a second dot, return 1 for error
            myErrNo = ERR_INVALID_AMOUNT;
            printf("ERROR: Currency input cannot contain more than one decimal (.)\n");
            return 1;
        }
    }

    // search for multiple negative signs
    char *neg1 = strchr(currencyString, '-');
    // if there is a negative sign, it needs to be the first character
    if (neg1 == currencyString) {
        // then check for second negative sign
        neg1++;
        char *neg2 = strchr(neg1, '-');
        if (neg2 != NULL) {
            // input contains a second negative sign, return 1 for error
            myErrNo = ERR_INVALID_AMOUNT;
            printf("ERROR: Currency input cannot contain more than one negative sign (-)\n");
            return 1;
        }
    }

    //search for bad characters
    for (int i = 0; currencyString[i] != '\0'; i++) {
        char ch = currencyString[i];
        if (!((ch >= '0' && ch <= '9') || ch == '-' || ch == '.' || ch == ',')) {
            myErrNo = ERR_INVALID_AMOUNT;
            printf("ERROR: Currency input cannot contain bad characters\n");
            return 1;
        }
    }
    removeChar(currencyString, ','); // remove commas from string - only if no errors in the input
    return 0;
}

/**
 * @brief helper method for requesting money from a different account
 *
 * @param accountNumber for retrieving info from the DB
 */
void requestMoney(unsigned int accountNumber) {
    Account requestingAcc = GET_ACCOUNT(accountNumber);
    printf("Enter the accountNumber you want to request from: ");

    char accountInput[10];
    getLine(accountInput, sizeof(accountInput));
    unsigned int requestedAccountNum = strtoul(accountInput, NULL, 10);
    if (requestedAccountNum < FIRST_ACCOUNT_NUMBER || requestedAccountNum > MAX_ACCOUNT_NUMBER) {
        // account number is not in the valid range
        myErrNo = ERR_INVALID_ACC_NUM;
        printf("ERROR: Invalid account number\n");
        return;
    }
    Account requestedAcc = GET_ACCOUNT(requestedAccountNum);
    if (requestedAcc.accountNumber == 0) {
        // specified account number is inactive
        myErrNo = ERR_INACTIVE_ACCOUNT;
        printf("ERROR: The specified account is inactive\n");
    } else if (accountNumber == requestedAcc.accountNumber) {
        myErrNo = ERR_SELF_REQUEST;
        printf("ERROR: Cannot request money from yourself\n");
    } else if (requestedAcc.requesterNumber != 0) {
        myErrNo = ERR_EXISTING_REQUEST;
        printf("ERROR: This account already has an exisitng request\n");
    } else {
        printf("Enter the amount you want to ask this sender: $");

        char currencyInput[20];
        getLine(currencyInput, sizeof(currencyInput));
        if (formatAndCheckCurrency(currencyInput) == 1) {
            // error in the string, return
            return;
        }
        // if no errors, we have a formatted positive OR negative string
        if (strchr(currencyInput, '-') != NULL) {
            // user entered negative amount
            myErrNo = ERR_INVALID_AMOUNT;
            printf("ERROR: You entered a negative amount\n");
        } else {
            // positive amount entered, check if you have enough money
            float moneyRequested = strtof(currencyInput, NULL); // parse formatted input to float
            // create the request
            requestedAcc.requesterNumber = requestingAcc.accountNumber;
            requestedAcc.requestAmount = moneyRequested;
            arr[requestedAcc.accountNumber - FIRST_ACCOUNT_NUMBER] = requestedAcc;
        }
    }
}

/**
 * @brief helper method for checking/approving/denying pending requests.
 *
 * @param accountNumber for retrieving info from the DB
 */
void checkRequests(unsigned int accountNumber) {
    Account acc = GET_ACCOUNT(accountNumber);
    if(acc.requesterNumber == 0) {
        // no current requests
        printf("You have no pending requests.");
        return;
    }
    unsigned int requestingAccountNum = acc.requesterNumber;
    float requestedAmount = acc.requestAmount;
    printf("You have a pending request of $%.2f from accountNumber %i\n", requestedAmount, requestingAccountNum);
    printf("Do you want to approve this request? (Y) to approve, (N) to deny: ");

    char choiceBuffer[3];
    getLine(choiceBuffer, sizeof(choiceBuffer));
    if (strlen(choiceBuffer) > 1) {
        myErrNo = ERR_INVALID_ARGS;
        printf("ERROR: Invalid input\n");
    } else {
        // process approval or denial
        if (strcmp(choiceBuffer, "Y") == 0) {
            if (requestedAmount > acc.balance) {
                myErrNo = ERR_NOT_ENOUGH_FUNDS;
                printf("ERROR: Current balance is too low to complete this request");
                cancelRequest(acc);
            } else {
                Account requestingAcc = GET_ACCOUNT(requestingAccountNum);
                acc.requesterNumber = 0;
                acc.requestAmount = 0;
                transferMoney(acc, requestingAcc, requestedAmount);
                printf("Request processed.");
            }
        } else if (strcmp(choiceBuffer, "N") == 0) {
            // request denied
            cancelRequest(acc);
        } else {
            myErrNo = ERR_INVALID_ARGS;
            printf("ERROR: Invalid answer, must answer (Y) or (N)\n");
        }
    }
}

/**
 * @brief helper method for transferring money between two accounts
 *
 * @param sendingAccount account sending money
 * @param receivingAccount account receiving money
 * @param amount balance to transfer
 */
void transferMoney(Account sendingAccount, Account receivingAccount, float amount) {
    receivingAccount.balance = receivingAccount.balance + amount;
    arr[receivingAccount.accountNumber - FIRST_ACCOUNT_NUMBER] = receivingAccount;

    sendingAccount.balance = sendingAccount.balance - amount;
    arr[sendingAccount.accountNumber - FIRST_ACCOUNT_NUMBER] = sendingAccount;
}

/**
 * @brief helper method for cancelling requests
 *
 * @param accountNumber for retrieving info from the DB
 */
void cancelRequest(Account acc) {
    acc.requesterNumber = 0;
    acc.requestAmount = 0;
    arr[acc.accountNumber - FIRST_ACCOUNT_NUMBER] = acc;
    printf("Request has been cancelled.\n");
}

/**
 * @brief helper method for remove instances of a character in a string
 *
 * @param str string to operate on
 * @param remove character to remove
 */
void removeChar(char *str, char remove) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != remove) dst++;
    }
    *dst = '\0';
}

// ----------------------- DO NOT MODIFY CODE BELOW -----------------------
/**
 * @brief entry point of program for current users; called from main to display menu and process commands.
 *
 * @param accountNumber for retrieving info from the DB
 */
void userMenu(unsigned int accountNumber) {
    Account acc = getAccountForTAUseOnly(accountNumber); // HEADER FILE NEEDS TO BE IMPLEMENTED FIRST

    // User operations --> IMPORTANT NOTE: User has to exit to write the DB @see executeCommand
    State currState = MENU;

    // TA USE ONLY: For reading input --> this has to be at least bigger than 1 char + NULL terminator + 1 (extra character)
    // to determine invalid inputs such as "12" --> this is due to the way getLine() is setup
    char choiceBuffer[3];
    while (currState != USER_EXIT) {
        // Resetting err number
        myErrNo = NO_ERROR;

        showMenu(acc.accountNumber);

        // Reading input
        getLine(choiceBuffer, sizeof(choiceBuffer));

        if (strlen(choiceBuffer) == 1) {
            // Processing input
            currState = executeCommand(choiceBuffer[0], accountNumber);
        } else {
            myErrNo = ERR_INVALID_ARGS;
            printf("ERROR: Invalid input\n");
        }
    }
}

/**
 * @brief Helper function for displaying the user options menu.
 *
 * @param accountNumber for retrieving info from the DB
 */
void showMenu(unsigned int accountNumber) {
    (void) (accountNumber);
    // Feel free to add more details to the menu title such as the personName!
    Account acc = arr[accountNumber - FIRST_ACCOUNT_NUMBER];
    printf("\n$$$$$$$$$$$$$$$$$$$$$ Welcome to 21tenmo %s! $$$$$$$$$$$$$$$$$$$$$\n", acc.personName);
    printf("\nSelect one of the options below (just the number):\n");
    printf("1. Show Balance\n");
    printf("2. Modify Balance\n");
    printf("3. Send Money\n");
    printf("4. Request Money\n");
    printf("5. Check Requests\n");
    printf("6. Save & Exit\n");
    printf("\n> ");
}
