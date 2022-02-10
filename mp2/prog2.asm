; this program takes the input from the keyboard, uses the compare subreoutine to see what the input is,
; send the PC to the appropriate address depending on the input, from there the program will print
; 'Invalid Expression', push the input to the stack, or pop 2 values from the stack and call the appropriate
; subroutine for the operator input. That new value is then pushed to the stack where the process repeates until '='
; is the input, which will cause the program to print invalid expression or to print the result of the equation to
; the screen in hexadecimal 
;
.ORIG x3000
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;R0->reads input from keyboard
;R1->result of compare subroutine
;R2->char that compare subroutine checks input against


EVALUATE
	AND R3, R3, #0
	AND R4, R4, #0
	GETC			; get and echo out from keyboard
	OUT

; take hex of keyboard input, store it in R3 to use in compare subroutine,
; since the decimal value of the input will be the decimal of the ASCII input,
; subtrac 48 from input to get proper decimal value from ASCII operand hex.
	LD R1, SUBFORTYEIGHT	; load #-48 to R1
	ADD R3, R0, #0		; copy hex input of ASCII char to R3
	ADD R0, R0, R1		; subtract #48 from ASCII value

; begin checking what the input is using the compare subroutine
; which will compare the hex of the ASCII char stored in R2 with the hex of the ASCII input
; the output of the compare subroutine is stored in R1 and is 0 if R2 and R3 are equal
; negative if the input has a smaller hex value than R3 and positive if the input has a larger
; hex value than R3. the negative and positive are used to help establish invalid inputs
	; check if input is '='
	LD R2, EQUALSIGN	; load '=' hex into R2
	JSR COMPARE		; comapre
	ADD R1, R1, #0		
	BRz ISEQUAL		; input is equal sign
	
	; check if input is [SPACE]
	LD R2, SPACESIGN	; load [SPACE] hex into R2
	JSR COMPARE		; compare
	ADD R1, R1, #0	
	BRz EVALUATE		; input is [SPACE], go to retreive next input
	BRn INVALIDEXPRESSION	; input < x20, not valid input

	; check if input is '^'
	LD R2, POWERSIGN	; load '^' hex into R2
	JSR COMPARE		; compare
	ADD R1, R1, #0
	BRz OPERATORP		; input is '^', go to power
	BRp INVALIDEXPRESSION	; input > x5E, not a valid input
	
	; check if input is '+'
	LD R2, ADDSIGN		; load '+' hex into R2
	JSR COMPARE		; compare
	ADD R1, R1, #0
	BRz OPERATORA		; input is '+', go to add

	; check if input is '-'
	LD R2, SUBTRACTSIGN	; load '-' hex into R2
	JSR COMPARE		; compare
	ADD R1, R1, #0
	BRz OPERATORS		; input is '-', go to subtract

	; check if input is '*'
	LD R2, MULTIPLYSIGN	; load '*' hex into R2
	JSR COMPARE		; compare
	ADD R1, R1, #0
	BRz OPERATORM		; input is '*', go to multiply
	
	; check if input it '/', establish more invalid inputs
        LD R2, DIVIDESIGN       ; load '/' hex into R2
        JSR COMPARE		; compare
        ADD R1, R1, #0
        BRz OPERATORD           ; input is '/', go to divide
	BRn INVALIDEXPRESSION	; input is < x2F, all operators checked and not a #, invalid input
	
	; seperate the invalid inputs from the operands
	LD R2, INVALIDSIGN	; load ':' hex into R2
	JSR COMPARE		; compare
	ADD R1, R1, #0
	BRn OPERAND		; input < x3A, its a operand, go to operand
	BRzp INVALIDEXPRESSION	; input >= x3A, all operators checked and not a #, invalid input 

; the input is '='
ISEQUAL
	JSR POP			; get value from stack
	ADD R3, R0, #0		; store value from stack in R3
	ADD R5, R5, #0		; check if zero values in stack
	BRnp INVALIDEXPRESSION	; stack is empty
	JSR POP			; check if just one value
	ADD R5, R5, #0		; see if pop was succesful
	BRz INVALIDEXPRESSION	; if it was there is more than one value in stack
	AND R5, R5, #0		; clear R5
	ADD R5, R3, #0		; load R3 into R5
	BRnzp PRINT_HEX		; go to print value

; the expression/input is invalid
INVALIDEXPRESSION
	LEA R0, INVALIDEXP	; load string address start into R0
	PUTS			; print string
	BRnzp	DONE		; go to done

; the input is an operand or a new value needs to be pushed to the stack
OPERAND
	JSR PUSH		; push value to stack
	BRnzp EVALUATE		; get next value

; this section of code has parts called depending on what the operator is
; for each operator 2 values are popped from the stack and stored in R3 and R4 respectiviely
; the stack is checked for underflow, then the operators subroutine is called. 
; then the code branches to push the new value from the operator subroutine to the stack
; the input is '+'
OPERATORA
	JSR POP			; pop first value from stack
	ADD R4, R0, #0		; store popped value in R4
	JSR POP			; pop second value from stack
	ADD R3, R0, #0		; store popped value in R3
	ADD R5, R5, #0		; check for underflow
	BRp INVALIDEXPRESSION	; stack underflow, branch to print 'invalid expression'
	JSR ADDITION		; go to add subroutine
	BRnzp OPERAND		; branch to push new value to stack

; the input is '-'
OPERATORS
	JSR POP			; pop first value from stack
        ADD R4, R0, #0		; store popped value in R4
	JSR POP			; pop second value from stack
	ADD R3, R0, #0		; store popped value in R3
        ADD R5, R5, #0          ; check for underflow
        BRp INVALIDEXPRESSION   ; stack underflow, branch to print 'invalid expression'
        JSR SUBTRACT		; go to subtract subroutine
	BRnzp OPERAND		; branch to push new value to stack

; the input is '*'
OPERATORM
	JSR POP			; pop first value from stack
	ADD R4, R0, #0		; store popped value in R4
        JSR POP			; pop second value from stack
	ADD R3, R0, #0		; store popped value in R3
        ADD R5, R5, #0          ; check for underflow
        BRp INVALIDEXPRESSION   ; stack underflow, branch to print 'invalid expression'
        JSR MULTIPLY		; go to multiply subroutine
	BRnzp OPERAND		; branch to push new value to stack

; the input is '^'
OPERATORP
	JSR POP			; pop first value from stack
	ADD R4, R0, #0		; store popped value in R4
	JSR POP			; pop second value from stack
	ADD R3, R0, #0		; store second value in R3
	ADD R5, R5, #0		; check for underflow
	BRp INVALIDEXPRESSION	; stack underflow, branch to print 'invalid expression'
	JSR POWER		; go to power subroutine
	BRnzp OPERAND		; branch to push new value to stack

; the input is '/'
OPERATORD
	JSR POP			; pop first value from stack
	ADD R4, R0, #0		; store popped value in R4
	JSR POP			; pop second value from stack
	ADD R3, R0, #0		; store second value in R3
	ADD R5, R5, #0		; check for underflow
	BRp INVALIDEXPRESSION	; stack underflow
	JSR DIVIDE		; go to divide subroutine
	BRnzp OPERAND		; brach to push new value to stack

DONE
	HALT


OPERATOR_SaveR7	.BLKW #1
EVAL_SaveR7	.BLKW #1

EQUALSIGN       .FILL x3D
SPACESIGN       .FILL x20
ADDSIGN         .FILL x2B
SUBTRACTSIGN    .FILL x2D
MULTIPLYSIGN    .FILL x2A
DIVIDESIGN      .FILL x2F
POWERSIGN       .FILL x5E
INVALIDSIGN     .FILL x3A 
SUBFORTYEIGHT	.FILL #-48

INVALIDEXP	.STRINGZ "Invalid Expression"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; printing the result in hex
;R0- what will print
;R1- digit counter
;R2- bit counter
;R3- value to print in hexadecimal
;R4- digit

PRINT_HEX
	AND R1, R1, #0		; clear digit counter
	ADD R1, R1, #4		; set digit counter to 4

; start digit counter loop and set bit counter
DIGITCOUNTER
	AND R2, R2, #0		; clear bit counter
	ADD R2, R2, #4		; init bit counter
	AND R4, R4, #0		; set digit to 0

; start bit counter loop, add 1 to digit if value is negative
LOOP
	ADD R4, R4, R4		; left shift digit
	ADD R3, R3, #0		; see if the value is + or -
	BRzp SKIP		; if bit is 0 skip adding 1 to digit
	ADD R4, R4, #1		; add 1 to digit

; continuation of bit counter loop to skip adding 1 to digit
SKIP
	ADD R3, R3, R3		; left shift R3 to be ready to read next digit
	ADD R2, R2, #-1		; decrement bit counter
	BRp LOOP		; if there are more bits repeat LOOP

; the 4 bits have been counted, now print correct character
NEGATIVE
	ADD R0, R4, #-9		; is digit <= 9?
	BRnz PRINTZERO		; if digit is <= 9 branch
	LD R0, A		; load A ASCII char
	ADD R0, R0, R4		; add digits to A
	ADD R0, R0, #-10	; subract 10
	BRnzp OUTTIME		; branch to print

; digit >9 so ZERO is initially loaded to R0 to print correct character
PRINTZERO
	LD R0, ZERO		; load 0 ASCII char
	ADD R0, R4, R0		; add digit to 0

; print the character indicated by digit. check if all 4 digits have been printed
OUTTIME
	OUT
	ADD R1, R1, #-1		; decrement digit counter
	BRp DIGITCOUNTER	; there are more digits to print
	BRnz DONE	

SPACE		.FILL x20
ZERO		.FILL x30
A		.FILL x41

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; compare the values in R2 and R3 
; input R2, R3
; out R1
; the subroutine negates R2, adds the new R2 and R3 together, and based on the results sets the value of R1
; if R2 and R3 are the same value then R1 stays #0, if R3 is smaller than the original R2 value then R1 is #-1
; and it R3 is larger than the original R2 value then R1 is #1. 

COMPARE
	ST R3, COMP_SaveR3	; store R3
	AND R1, R1, #0
	NOT R2, R2
	ADD R2, R2, #1		; negate R2
	ADD R3, R2, R3		; compare R2 and R3 values
	BRn NEG			; branch to set R1 to #-1
	BRz DONE_COMP		; branch to keep R1 #0
	ADD R1, R1, #1		; set R1 to #1
	BRnzp DONE_COMP		; branch to end
NEG
	ADD R1, R1, #-1		; set R1 to #-1
DONE_COMP
	LD R3, COMP_SaveR3	; restore R3
	RET			; return

COMP_SaveR3	.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; subroutine adds R3 and R4

ADDITION
	ADD R0, R3, R4		; add R3 and R4 into R0 
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; subroutine negates R4 then adds R3 and R4 to do subtraction R3-R4

SUBTRACT	
	ST R4, SUBTRACT_SaveR4	; save R4
	NOT R4, R4		
	ADD R4, R4, #1		; negate R4
	ADD R0, R3, R4		; add R3 and R4 
DONE_SUBTRACT	
	LD R4, SUBTRACT_SaveR4	; restore R4
	RET 			; return

SUBTRACT_SaveR4	.BLKW #1
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; subroutine makes R4 into a loop counter for a loop that adds R3 to itself to do multiplication
; there is an extra loop in case the value in R4 (the loop counter) is negative

MULTIPLY	
	ST R3, MULTIPLY_SaveR3	; save R3
	ST R4, MULTIPLY_SaveR4	; save R4
	AND R0, R0, #0		; clear R0
	ADD R4, R4, #0		; see if R4 is negative
	BRn NEGMULT		; R4 is negative
MULTLOOP
	ADD R0, R0, R3		; add R3 to R0 
	ADD R4, R4, #-1		; decrement loop counter
	BRp MULTLOOP		; there is more to multiply so keep multiplying 
	BRnz DONE_MULTIPLY	
NEGMULT
	ADD R3, R3, #0		; check if R3 is also negative
	BRn NEGMULTLOOP		; R3 is negative
	ADD R0, R0, R3		; add R3 to R0
	ADD R4, R4, #1		; increment loop counter
	BRn NEGMULT		; more to multiply
	BRzp DONE_MULTIPLY
; R3 and R4 are negative and -*-=+ so make both + and send to regular loop
NEGMULTLOOP
	NOT R4, R4		
	ADD R4, R4, #1		; negate R4
	NOT R3, R3
	ADD R3, R3, #1		; negate R3
	BRnzp MULTLOOP		; send to regular loop
DONE_MULTIPLY
	LD R3, MULTIPLY_SaveR3	; restore R3
	LD R4, MULTIPLY_SaveR4	; restore R4
	RET			; return
		
MULTIPLY_SaveR3	.BLKW #1
MULTIPLY_SaveR4	.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0 (quotient)
; subroutine performs division R3/R4
; dividend = R3, divisor = R4
DIVIDE	
	ST R3, DIVIDE_SaveR3	; save R3
	ST R4, DIVIDE_SaveR4	; save R4
	AND R0, R0, #0		; clear out R0
	NOT R4, R4		
	ADD R4, R4, #1		; negate R4
DIVLOOP
	ADD R0, R0, #1		; add 1 to quotient
	ADD R3, R3, R4		; sub divisor from dividend
	BRzp DIVLOOP		; there is still division to do
	ADD R0, R0, #-1		; correct for going 1 loop too far 
DONE_DIVIDE	
	LD R3, DIVIDE_SaveR3	; restore R4
	LD R4, DIVIDE_SaveR4	; resotre R4
	RET			; return

DIVIDE_SaveR3	.BLKW #1
DIVIDE_SaveR4	.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; subroutine uses a loop within a loop to multiply R3 by itself R4 times
; the value in R3 is multiplied by itself, that new value is multiplied by the original
; value of R3, and so on until R4 is 0

POWER
	ST R1, POWER_SaveR1	; store R1
	ST R2, POWER_SaveR2	; store R2
	ST R3, POWER_SaveR3	; store R3
	ST R4, POWER_SaveR4	; store R4
	ADD R4, R4, #-1		; decrement R4 to keep accurate count
	ADD R2, R3, #0		; store R2 in R3 
POWERLOOP
	ADD R1, R3, #0		; set inner loop counter
	AND R0, R0, #0		; clear R0
	; multiply R3 by itself R4 times
POWERINNERLOOP
	; multiply R3 by itself
	ADD R0, R0, R2		; add add R2(R3) to R0
	ADD R1, R1, #-1		; decrement inner loop count
	BRp POWERINNERLOOP	; havent finished multiplying R3 by itself	
	ADD R2, R0, #0		; store inner loop count in R2 for next loop
	ADD R4, R4, #-1		; decrement outer loop counter
	BRp POWERLOOP		; still more math to do

DONE_POWER
	LD R1, POWER_SaveR1	; restore R1
	LD R2, POWER_SaveR2	; restore R2
	LD R3, POWER_SaveR3	; restore R3
	LD R4, POWER_SaveR4	; restore R4
	RET			; return

POWER_SaveR1	.BLKW #1
POWER_SaveR2	.BLKW #1
POWER_SaveR3	.BLKW #1
POWER_SaveR4	.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACK_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;

.END
