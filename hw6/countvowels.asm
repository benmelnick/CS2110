;;=============================================================
;;CS 2110 - Fall 2019
;;Homework 6 - Count the Vowels in a String
;;=============================================================
;;Name: Benjamin Melnick
;;=============================================================

;;    string[] = "TWENTY ONE TEN";
;;    vcount = 0;
;;    i = 0;
;;    while(string[i] != ’\0’){
;;        if(string[i] == ’A’ || string[i] == ’E’ ||
;;            string[i] == ’I’ || string[i] == ’O’ ||
;;            string[i] == ’U’) {
;;                vcount += 1; }
;;        i += 1;
;;    }
;;    ANSWER = vcount;

.orig x3000

	            AND    R0, R0, #0     ; clear R0 - vcount
                LD     R1, STRING     ; load R1 with first memory address in string, i = 0
    GET_CHAR    LDR    R2, R1, #0     ; load R2 with the letter at the memory address contained in R1, get string[i]
                NOT    R2, R2         ; negate ASCII value of char
                ADD    R2, R2, #1
                LD     R3, NULL       ; load R3 with ASCII value of null character
                ADD    R4, R3, R2     ; find the difference b/w the current character and the null terminated character - if 0, they are the same
                BRZ    TERMINATE      ; string[i] == '\0'
    CHECK_A     LD     R3, A          ; load R3 with ASCII value of A
                ADD    R4, R3, R2
                BRZ    INC_COUNT      ; string[i] == 'A'
    CHECK_E     LD     R3, E          ; load R3 with ASCII value of E
                ADD    R4, R3, R2
                BRZ    INC_COUNT      ; string[i] == 'E'
    CHECK_I     LD     R3, I          ; load R3 with ASCII value of I
                ADD    R4, R3, R2
                BRZ    INC_COUNT      ; string[i] == 'I'
    CHECK_O     LD     R3, O          ; load R3 with ASCII value of O
                ADD    R4, R3, R2
                BRZ    INC_COUNT      ; string[i] == 'O'
    CHECK_U     LD     R3, U          ; load R3 with ASCII value of U
                ADD    R4, R3, R2
                BRZ    INC_COUNT      ; string[i] == 'U'
                BR     INC_MEM        ; if none are vowels, start from beginning
    INC_COUNT   ADD    R0, R0, #1     ; vcount++
    INC_MEM     ADD    R1, R1, #1     ; increase the memory address for the string, i++
                BRNZP  GET_CHAR       ; evaluate next character from the beginning
    TERMINATE   ST R0, ANSWER         ; ANSWER = vcount
                HALT

A    .fill x0041    ;; A in ASII
E    .fill x0045
I    .fill x0049
O    .fill x004F
U    .fill x0055
NULL .fill x0000  ; \0 in ASCII

STRING .fill x4000
ANSWER .blkw 1
.end

.orig x4000
.stringz "TWENTY ONE TEN"
.end
