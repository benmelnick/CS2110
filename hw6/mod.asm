;;=============================================================
;;CS 2110 - Fall 2019
;;Homework 6 - Modulus
;;=============================================================
;;Name: Benjamin Melnick
;;=============================================================

;; a = (argument 1)
;; b = (argumnet 2)
;; while (a - b >= 0)
;;      a = a - b

.orig x3000

          LD    R0, A      ; R0 = A
          LD    R1, B      ; R1 = B
          LD    R2, B      ; this will hold -B
          AND   R3, R3, 0  ; clear R3 - this will be the iterator
          NOT   R2, R2     ; negate R2, R2 = ~B
          ADD   R2, R2, #1 ; R2 = ~B + 1 = -B
    LOOP  ADD   R0, R0, R2 ; calculate A - B and store in R0
          BRZP LOOP        ; perform loop instruction again if result is not negative
          ADD R0, R0, R1   ; loop always goes one too many, add to compensate
          ST R0, ANSWER
          HALT

A   .fill x703A
B   .fill x00FE

ANSWER .blkw 1

.end
