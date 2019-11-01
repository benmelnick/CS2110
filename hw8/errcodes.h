// DO NOT MODIFY THIS FILE

/**
 * @brief Header file for global predefined error field and codes for usage of the 21tenmo program.
 */
#ifndef __ERRCODES_H__
#define __ERRCODES_H__

// Global error number to be set whenever a DATABASE operation fails (invalid "menu" inputs do not modify this variable)
extern unsigned int myErrNo;

// List of error codes (possible values of myErrNo in this program)
#define NO_ERROR                0
#define ERR_INVALID_ACC_NUM     300     /* Invalid accountNumber specified (accountNumber specified is out of range of possible values for arr @see 21tenmo.h) */
#define ERR_INVALID_ARGS        301     /* Invalid arguments/flag passed in */

// ----- Specific to main.c -------
#define ERR_INVALID_NAME        302     /* Invalid name passed in @see 21tenmo.h for name length requirements */
#define ERR_DB_FULL             303     /* Database is full */

// ---- Specific to 21tenmo.c -----
#define ERR_NOT_ENOUGH_FUNDS    304     /* Not enough funds to perform the specified operation */
#define ERR_INACTIVE_ACCOUNT    305     /* Inactive account (i.e. accountNumber = 0) */
#define ERR_INVALID_AMOUNT      306     /* Invalid amount/currency entered for the specified operation */
#define ERR_SELF_REQUEST        307     /* Operation's requester is the same as requestee */
#define ERR_EXISTING_REQUEST    308     /* Account already has a existing/pending request */


// Side-Note: error values start at (arbitrary value) 300 to avoid confusion with the possible values for
// the errno macro within the errno.h library and also HTTP and socket programming error codes

#endif
