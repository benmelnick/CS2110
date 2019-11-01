;=======================================
;; CS 2110 - Fall 2019
;; Homework 7
;;=======================================
;; Name: Benjamin Melnick
;;=======================================

;; In this file, you must implement the 'factorial' and 'sumAndFacLL' subroutines.
;; You are given the 'MULT' and 'sumtorial' subroutines; use these as you wish.

;; Little reminder from your friendly neighborhood 2110 TA staff:
;; don't run this directly by pressing 'RUN' in complx, since there is nothing
;; put at address x3000. Instead, load it and use 'Debug' -> 'Simulate Subroutine
;; Call' and choose the 'factorial' or 'sumAndFacLL' label.


.orig x3000

halt

factorial
;; Arguments of Factorial: integer n
;;
;; Psuedocode
;; Factorial(n):
;;     if n = 0:
;;         return 1
;;     else:
;;         return n * Factorial(n-1)

                ; STACK BUILDUP
                ADD     R6, R6, #-4     ; moves R6 down 4 to point (EFFC)
                ;Return Value will go in R6,#3 once we are done
                STR     R7, R6, #2      ; save return address (EFFC)
                STR     R5, R6, #1      ; save old FP (EFFD)
                ADD     R5, R6, #0      ; establish FP (move FP to current stack pointer) (this is where first temp variable is)
                ADD     R6, R6, #-2     ; make space for saved registers (2 of them) and move stack pointer
                STR     R0, R5, #-1     ; SAVE REGISTERS
                STR     R1, R5, #-2

                ; SUBROUTINE CODE
                ; if (n = 0):
                LDR     R0, R5, #4      ; R0 <- n
                BRP     IFELSE1
                ; return 1
                AND     R0, R0, #0      ; clear R0
                ADD     R0, R0, #1      ; R0 <- 1
                STR     R0, R5, #3      ; return value = 1
                BR      STACK_BREAKDOWN
                ; else:
                ; return n * fact(n-1)
                ; rewrite: R0 = fact(n - 1) ; return mult(n, R0)
    IFELSE1     LDR     R0, R5, #4      ; push n - 1 onto stack
                ADD     R0, R0, #-1
                ADD     R6, R6, #-1
                STR     R0, R6, #0
                JSR     factorial       ; factorial(n-1)
                LDR     R0, R6, #0      ; when subroutine ends, R6 will point to the return value - put the return value in R0
                ADD     R6, R6, #2      ; pop return value and n-1, point R6 back to top of registers
                ; multiple n and R0
                ADD     R6, R6, #-1     ; push R0
                STR     R0, R6, #0
                ADD     R6, R6, #-1     ; push n
                LDR     R0, R5, #4      ; R0 <- n
                STR     R0, R6, #0
                JSR     MULT            ; mult(n, R0)
                LDR     R0, R6, #0      ; get return value of multiply
                ADD     R6, R6, #3      ; pop the return value and arguments of MULT subroutine stack
                STR     R0, R5, #3      ; store result of multiplication into original stack frame return value

    STACK_BREAKDOWN
                LDR     R1, R5, #-2
                LDR     R0, R5, #-1
                ADD     R6, R5, #0      ; move stack pointer back down
                LDR     R5, R6, #1      ; Restore old FP
                LDR     R7, R6, #2      ; Restore return address
                ADD     R6, R6, #3      ; move stack pointer to RV (return value)
                RET

sumAndFacLL
;;Arguments: Node head (the address of the current node - head to begin)

;; Psuedocode
;; sumtorialAndFactorialLL(Node head) {
;;     if (head == null) {
;;         return head
;;     }
;;
;;     if (head.data % 2 == 0) {
;;         head.data = sumtorial(head.data)
;;
;;     } else {
;;         head.data = factorial(head.data)
;;     }
;;
;;     sumtorialAndFactorialLL(head.next)
;;     return head;
;; }

            ; STACK buildup
            ADD     R6, R6, #-4         ; create space in stack by moving stack pointer up
            STR     R7, R6, #2          ; save return address
            STR     R5, R6, #1          ; save old frame pointer
            ADD     R5, R6, #0          ; copy stack pointer to frame pointer
            ADD     R6, R6, #-5         ; make room for registers on the stack
            STR     R0, R5, #-1         ; save registers
            STR     R1, R5, #-2
            STR     R2, R5, #-3
            STR     R3, R5, #-4
            STR     R4, R5, #-5

            LDR     R0, R5, #4          ; R0 <- address of head node
            BRZ     DONE
            LDR     R1, R0, #0          ; R1 <- head.next
            LDR     R2, R0, #1          ; R2 <- head.data
            AND     R3, R2, #1          ; data AND 1 to check if even or odd - odd numbers will have a 1 bit at the end
            BRZ     EVEN                ; head.data % 2 == 0
            ADD     R6, R6, #-1         ; data is odd - push head.data onto stack
            STR     R2, R6, #0
            JSR     factorial
            LDR     R3, R6, #0          ; R6 currently points to return value - store it in R3
            STR     R3, R0, #1          ; store factorial(head.data) into head.data
            ADD     R6, R6, #2          ; move stack pointer back to registers
            BR      NEXT

    EVEN    ADD     R6, R6, #-1         ; push head.data onto stack
            STR     R2, R6, #0
            JSR     sumtorial
            LDR     R3, R6, #0          ; R6 currently points to return value - store it in R3
            STR     R3, R0, #1          ; store sumtorial(head.data) into head.data memory address
            ADD     R6, R6, #2          ; move stack pointer back to top of registers

    NEXT    ADD     R6, R6, #-1         ; push head.next onto stack
            STR     R1, R6, #0
            JSR     sumAndFacLL

            ADD     R6, R6, #2
    DONE    STR     R0, R5, #3          ; return head

    BREAKDOWN
            LDR     R4, R5, #-5     ; RESTORE SAVED REGISTERS
            LDR     R3, R5, #-4
            LDR     R2, R5, #-3
            LDR     R1, R5, #-2
            LDR     R0, R5, #-1
            ADD     R6, R5, #0      ; move stack pointer back down
            LDR     R5, R6, #1      ; Restore old FP
            LDR     R7, R6, #2      ; Restore return address
            ADD     R6, R6, #3      ; move stack pointer to RV (return value)
            RET

MULT
    ; Recursive MULT
    ;;BUILDUP
        ADD R6,R6,#-4 ; moves R6 down 4 to point (EFFC)
        ;Return Value will go in R6,#3 once we are done
        STR R7,R6,#2 ; save return address (EFFC)
        STR R5,R6,#1 ; save old FP (EFFD)
        ADD R5,R6,#0 ; establish FP (move FP to current stack pointer) (this is where first temp variable is - answer)
        ADD R6,R6,#-5 ; make space for saved registers (5 of them) and move stack pointer
        STR R0,R5,#-1 ; SAVE REGISTERS
        STR R1,R5,#-2 ;
        STR R2,R5,#-3 ;
        STR R3,R5,#-4 ;
        STR R4,R5,#-5 ;


    ;;SUBROUTINE CODE
        ;; int a is the leftmost arg (R5 + 4)
        LDR R0,R5,#4 ; Load int a into R0
        LDR R1,R5,#5 ; Load int b into R1

        AND R3,R3,#0 ; Clear R3 for keeping track of positive or negative
        AND R2,R2,#0 ; Clear R2 and reserve it for VALUE (the thing you'll return)
        AND R1,R1,R1 ; Get condition code of int b
        BRzp MULTLOOPmult ; if b ≥ 0, continue as normal
        ; if b is negative, make it positive and set R3 = 1 (used later)
        NOT R1,R1
        ADD R1,R1,#1
        AND R3,R3,#0
        ADD R3,R3,#1

        MULTLOOPmult
        AND R1,R1,R1 ; Just to make sure we're looking at int b
        BRnz OUTOFMULTLOOPmult ; if b ≤ 0, break the loop
        ADD R2,R2,R0 ; Add a to VALUE
        ADD R1,R1,#-1 ; decrement b
        BR MULTLOOPmult

        OUTOFMULTLOOPmult
            ADD R3,R3,#-1 ; Subtract 1 from R3
            BRnp NEXTmult; if R3 != 1, then skip this step
            ; if R3 == 1 (b was negative earlier), make return value negative
            NOT R2,R2
            ADD R2,R2,#1
            NEXTmult
            STR R2,R5,#3 ; Store VALUE in the return value spot on stack (R5 + 3)


    ;;TEARDOWN
        LDR R4,R5,#-5 ; RESTORE SAVED REGISTERS
        LDR R3,R5,#-4 ;
        LDR R2,R5,#-3 ;
        LDR R1,R5,#-2 ;
        LDR R0,R5,#-1 ;
        ADD R6,R5,#0 ; move stack pointer back down
        LDR R5,R6,#1 ; Restore old FP
        LDR R7,R6,#2 ; Restore return address
        ADD R6,R6,#3 ; move stack pointer to RV (return value)
        RET


sumtorial
;; Arguments of sumtorial: integer n
;;
;; Psuedocode
;; sumtorial(n):
;;     if n = 0:
;;         return 0
;;     else:
;;         return n + sumtorial(n-1)

    ;; BUILDUP
        ADD R6,R6,#-4 ; moves R6 down 4 to point (EFFC)
        ;Return Value will go in R6,#3 once we are done
        STR R7,R6,#2 ; save return address (EFFC)
        STR R5,R6,#1 ; save old FP (EFFD)
        ADD R5,R6,#0 ; establish FP (move FP to current stack pointer) (this is where first temp variable is - answer)
        ADD R6,R6,#-5 ; make space for saved registers (5 of them) and move stack pointer
        STR R0,R5,#-1 ; SAVE REGISTERS
        STR R1,R5,#-2 ;
        STR R2,R5,#-3 ;
        STR R3,R5,#-4 ;
        STR R4,R5,#-5 ;


    ;; SUBROUTINE CODE
        LDR R0,R5,#4 ; R0 <- n

        BRz DONEsumtorial ; if n == 0, return it

        ; Else, do sumtorial(n-1) and then add result with n
        ;; Call to sumtorial(n-1)
        AND R1,R1,#0 ; Clear R1
        ADD R1,R0,#-1 ; R1 <- n-1
            STR R1,R6,#-1 ; Push n-1 on the stack
            ADD R6,R6,#-1 ; Update the stack pointer
            JSR sumtorial ; Make the call to SUMTORIAL
            LDR R1,R6,#0 ; R1 <- return value
            ADD R6,R6,#2 ; pop return value and arg
        ; Now sumtorial(n-1) is in R1 so add R1 + R0 (n)
            ADD R0,R1,R0 ; R2 <- n + sumtorial(n-1)

        DONEsumtorial
            STR R0,R5,#3 ; Store return value on the stack



    ;; TEARDOWN
        LDR R4,R5,#-5 ; RESTORE SAVED REGISTERS
        LDR R3,R5,#-4 ;
        LDR R2,R5,#-3 ;
        LDR R1,R5,#-2 ;
        LDR R0,R5,#-1 ;
        ADD R6,R5,#0 ; move stack pointer back down
        LDR R5,R6,#1 ; Restore old FP
        LDR R7,R6,#2 ; Restore return address
        ADD R6,R6,#3 ; move stack pointer to RV (return value)
        RET

; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end
