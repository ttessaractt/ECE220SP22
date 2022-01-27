;
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop


; the program uses a count down from 27 and a count up from 0 to 
; print all 27 values stored in the histogram.
; the start address of the histogram is added with the number of lines already printed (R5)
; then the value at that new address is loaded into the register to be printed.
; the hex value of @ is also added with the number of lines already printed to get the 
; character for the histogram value that will print. 

; Registers
; R0-what will print
; R1-digit counter
; R2-value stored
; R3-bit counter
; R4-digit
; R5-offset from start of histagram
; R6-loop counter of 27

; inital values of the full print_hist loop
PRINT_HIST
	LD R6, NUM_BINS		; load #27 into loop counter
	AND R5,R5,#0		; init R5 to 0

; print the bin label and space and load value to be printed 
LINE_START		
	AND R0,R0,#0		; clear R0
	LD R2, HIST_ADDR	; get start address of histogram
	ADD R2,R2,R5		; add offset for how many letters have printed
	LDR R2, R2, #0		; load value at address
	LD R0, LETTER		; get @
	ADD R0,R0,R5		; add offset for bin lables
	OUT			; print bin labels
	LD R0, SPACE		; load space
	OUT			; print space
; set digit counter
INITS	
	AND R1,R1,#0		; clear digit counter
	ADD R1,R1,#4		; set digit counter to 4

; start digit counter loop and set bit counter and digit
DIGITCOUNTER
	AND R3,R3,#0		; clear bit counter
	ADD R3,R3,#4    	; init bit counter
	AND R4,R4,#0		; set digit to 0

; start bit counter loop, add 1 to digit if value is negative
LOOP	
	ADD R4,R4,R4    	; left shift digit
        ADD R2,R2,#0    	; see if the value is + or -
        BRzp SKIP       	; if bit is 0 skip adding 1 to digit
	ADD R4,R4,#1    	; add 1 to digit

; continuation of bit counter loop to skip adding 1 to digit
SKIP
        ADD R2,R2,R2    	; left shift R2 to be ready to read next digit
        ADD R3,R3,#-1   	; decrement bit counter
        BRp LOOP        	; if there are more bits repeat LOOP

; the 4 bits have been counted now print correct character
NEGATIVE
        ADD R0,R4,#-9   	; is digit <= 9?
        BRnz PRINTZERO		; if digit is <= 9 branch
        LD R0, A        	; load A ASCII
        ADD R0,R0,R4		; add digit to A
        ADD R0,R0,#-10		; subtract 10 
	BRnzp OUTTIME		; branch to print

; digit >9 so ZERO is initally loaded to R0 to print correct character
PRINTZERO
	LD R0, ZERO     	; load 0 ASCII
	ADD R0,R4,R0		; add digit to 0

; print the character indicated by digit. check if all 4 digits have been printed
; decrement loop counter and increment offset 
OUTTIME
	OUT			; print the group of 4 bits in ASCII 
	ADD R1,R1,#-1		; decrement digit counter
	BRp DIGITCOUNTER	; there are more digits to print
	ADD R6,R6,#-1		; decrement outer loop counter
	BRnz DONE		; if R6 reaches 0 all lines have printed
	ADD R5,R5,#1		; increment offset

; all 4 digits have been printed, print a new line and loop to start a new line
NEW_LINE
	LD R0, ASCII_NL		; load new line
	OUT			; print new line
	BRnzp LINE_START	; all digits have printed.

DONE	HALT			; done

ZERO    	.FILL x30
A       	.FILL x41
ASCII_NL	.FILL x0A
LETTER		.FILL x40
SPACE		.FILL x20

; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00     ; histogram starting address
STR_START	.FILL x4000	; string starting address

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
