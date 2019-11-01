;;=============================================================
;;CS 2110 - Fall 2019
;;Homework 6 - Sum the Elements of a Linked List
;;=============================================================
;;Name: Benjamin Melnick
;;=============================================================

;;  length = LL.length;
;;  curr = LL.head;
;;  sum = 0;
;;  while (curr != null) {
;;      sum += curr.value;
;;      curr = curr.next;
;;  }

.orig x3000

            AND   R2, R2, #0    ; clear R3 to store sum
            LDI   R0, LL        ; gets the address of head node, curr = LL.head
            BRZ   END           ; check if head node is null
    LOOP    LDR   R1, R0, #1    ; gets the value of the current node, curr.value
            ADD   R2, R2, R1    ; sum += curr.value
            LDR   R0, R0, #0    ; gets the address of the next node, curr = curr.next
            BRP   LOOP          ; loop back through
    END     ST R2, ANSWER       ; ANSWER = sum
            HALT

LL .fill x6000
ANSWER .blkw 1
.end

.orig x4000
.fill x4008     ; address of next node
.fill 5         ; value at node
.fill x400A
.fill 2
.fill x4002
.fill 9
.fill x0000
.fill 3
.fill x4004
.fill 10
.fill x4006
.fill 7
.end

.orig x6000
.fill x4000  ; address of head node
.fill 6      ; length of linked list
.end
