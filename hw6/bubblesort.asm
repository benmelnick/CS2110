;;=============================================================
;;CS 2110 - Fall 2019
;;Homework 6 - Bubble Sort
;;=============================================================
;;Name: Benjamin Melnick
;;=============================================================

;;    len = length of array;
;;    y = len - 1;
;;    while(y > 0) {
;;        x = 0;
;;        count = y;
;;        while(count > 0)
;;           if (array[x] > array[x+1])
;;                temp = ARRAY[x];
;;                ARRAY[x] = ARRAY[x+1];
;;                ARRAY[x+1] = temp;
;;           x++;
;;           count--;
;;        y--;

.orig x3000

                LD      R4, LENGTH  ; R4 = length of the array
    OUTER_LOOP  ADD     R4, R4, #-1 ; R4 = y = length - 1, loop y times
                BRNZ    SORTED      ; looping complete, exit
                ADD     R5, R4, #0  ; initialize inner loop counter to outer loop counter
                LD      R3, ARRAY   ; load R3 with beginning of ARRAY memory address - initially x4000 - the same as setting x to 0
    INNER_LOOP  LDR     R0, R3, #0  ; get array[x]
                LDR     R1, R3, #1  ; get array[x+1]
                NOT     R2, R1      ; negate array[x+1]
                ADD     R2, R2, #1
                ADD     R2, R0, R2  ; swap = item - next item
                BRNZ    SWAPPED     ; don't swap if items are in order
                STR     R1, R3, #0  ; perform the swap
                STR     R0, R3, #1
    SWAPPED     ADD     R3, R3, #1  ; increment pointer for array memory address - increment x
                ADD     R5, R5, #-1 ; decrement inner loop counter
                BRP     INNER_LOOP  ; end of inner loop - run again if R5 > 0
                BRNZP   OUTER_LOOP  ; end of outer loop - always go back to top
    SORTED      HALT

ARRAY .fill x4000
LENGTH .fill 7
.end

.orig x4000
.fill 4
.fill 9
.fill 0
.fill 2
.fill 9
.fill 3
.fill 10
.end
