;;=======================================
;; CS 2110 - Fall 2019
;; Homework 7
;;=======================================
;; Name: Benjamin Melnick
;;=======================================

;; In this file, you must implement the 'gcd' subroutine.

;; Little reminder from your friendly neighborhood 2110 TA staff:
;; don't run this directly by pressing 'RUN' in complx, since there is nothing
;; put at address x3000. Instead, load it and use 'Debug' -> 'Simulate Subroutine
;; Call' and choose the 'gcd' label.


.orig x3000

halt

gcd
;; Arguments of GCD: integer n, integer m
;;
;; Psuedocode:
;; GCD(n, m):
;;     if n < m:
;;         return GCD(m,n)
;;     if n > m:
;;         return GCD(n - m, m)
;;     else:
;;         return n

            ; STACK buildup
            ADD     R6, R6, #-4         ; create space in stack
            STR     R7, R6, #2          ; save return address
            STR     R5, R6, #1          ; save old frame pointer
            ADD     R5, R6, #0          ; copy stack pointer to frame pointer
            ADD     R6, R6, #-5         ; make room for registers on the stack
            STR     R0, R5, #-1         ; save registers
            STR     R1, R5, #-2
            STR     R2, R5, #-3
            STR     R3, R5, #-4
            STR     R4, R5, #-5

            LDR     R0, R5, #4          ; load int n into R0
            LDR     R1, R5, #5          ; load int m into R1
            NOT     R2, R0              ; negate n
            ADD     R2, R2, #1
            ADD     R3, R1, R2          ; R3 <- m - n ; check for inequality b/w the two
            BRP     GREATER             ; n < m
            NOT     R3, R1              ; negate m
            ADD     R3, R3, #1
            ADD     R3, R0, R3          ; R3 <- n - m
            BRP     LOWER               ; n > m
            STR     R0, R5, #3          ; return n
            BR      STACK_BREAKDOWN

    GREATER ADD     R6, R6, #-1         ; push n onto stack
            STR     R0, R6, #0
            ADD     R6, R6, #-1         ; push m onto stack
            STR     R1, R6, #0
            JSR     gcd                 ; gcd(m, n)
            LDR     R3, R6, #0          ; R6 currently points to return value - store it in R3
            STR     R3, R5, #3          ; return gcd(m, n)
            BR      STACK_BREAKDOWN

    LOWER   NOT     R4, R1              ; negate m
            ADD     R4, R4, #1
            ADD     R4, R0, R4          ; calculate n - m
            ADD     R6, R6, #-1         ; push m onto stack
            STR     R1, R6, #0
            ADD     R6, R6, #-1         ; push n - m onto stack
            STR     R4, R6, #0
            JSR     gcd                 ; gcd(n - m, m)
            LDR     R3, R6, #0          ; R6 currently points to return value - store it in R3
            STR     R3, R5, #3          ; return gcd(n - m, n)
            ADD     R6, R6, #3          ; move stack back to top of registers

    STACK_BREAKDOWN
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

; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end
