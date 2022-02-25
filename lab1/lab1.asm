


; R0-what will print
; R1-digit counter
; R2-bit counter
; R3-value stored
; R4-digit

.ORIG x3000
; write your code here

AND R1,R1,#0	; clear digit counter
ADD R1,R1,#4	; init digit counter
AND R4,R4,#0	; clear digit
AND R2,R2,#0	; clear bit counter

DIGITCOUNTER	ADD R1,R1,#0	; check value of digit counter
		BRz DONE	; all digits have printed
		ADD R2,R2,#4	; init bit counter
		AND R4,R4,#0		

LOOP		ADD R2,R2,#0	
		BRz NEGATIVE
		ADD R4,R4,R4	; left shift digit
		ADD R3,R3,#0	; R3 <0?
		BRp SKIP 	; skip adding 1 to digit 
		ADD R4,R4,#1	; add 1 to digit
SKIP		ADD R3,R3,R3	; left shift R3
		ADD R2,R2,#-1	; decrement bit counter
		BRp LOOP	; if there are more bits repeat LOOP
NEGATIVE 	ADD R0,R4,#-9	; is digit <= 9?
		BRn PRINTZERO
		LD R0, A	; load A ASCII
		ADD R0,R0,R4
		ADD R0,R0,#-10
		BRnzp OUTTIME
		
PRINTZERO	LD R0, ZERO	; load 0 ASCII
		ADD R0,R4,R0

OUTTIME		ADD R1,R1,#-1
		OUT
		BRnzp DIGITCOUNTER	

ZERO	.FILL x30
A	.FILL x41


DONE	HALT

.END

