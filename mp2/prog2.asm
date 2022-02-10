;
;
;
.ORIG x3000
	
;your code goes here
	
;R0->reads input from keyboard
;R1->load and manipulate constants for char checking and checking at the end for extra '('
;R2->loads stack top to check if there are nay extra '('
;R5->

AND R5, R5, #0	; clear R5
AND R3, R3, #0	; clear R3
AND R4, R4, #0	; clear R4

EVALUATE
	AND R3, R3, #0
	AND R4, R4, #0
	ST R7, EVAL_SaveR7
	GETC			; get and echo out from keyboard
	OUT
	LD R1, SUBFORTYEIGHT	; load #-48 to R1
	ADD R3, R0, #0		; copy hex input of ASCII char to R3
	ADD R0, R0, R1		; subtract #48 from ASCII hex to get correct decimal value

	; check what input is using R3
	LD R2, EQUALSIGN	; see if input is '='
	JSR COMPARE
	ADD R1, R1, #0
	BRz ISEQUAL		; input is equal sign
	
	LD R2, SPACESIGN	; see if input is ' '
	JSR COMPARE
	ADD R1, R1, #0
	BRz EVALUATE		; input is ' ', skip
	BRn INVALIDEXPRESSION	; inpute < x20, not valid char
	
	LD R2, POWERSIGN	; see if input is '^' before declaring char invalid
	JSR COMPARE
	ADD R1, R1, #0
	BRz OPERATORP		; input is '^' (operator), go to power
			
	LD R2, ADDSIGN		; see if input is '+'
	JSR COMPARE
	ADD R1, R1, #0
	BRz OPERATORA		; input is '+', go to add

	LD R2, SUBTRACTSIGN	; see if input is '-'
	JSR COMPARE
	ADD R1, R1, #0
	BRz OPERATORS		; input is '-', go to subtract

	LD R2, MULTIPLYSIGN	; see if input is '*'
	JSR COMPARE
	ADD R1, R1, #0
	BRz OPERATORM		; input is '*', go to multiply
	
        LD R2, DIVIDESIGN       ; see if input is '/'
        JSR COMPARE
        ADD R1, R1, #0
        BRz OPERATORD           ; input is '/', go to divide
	BRn INVALIDEXPRESSION	; input is < x2F, all operators checked and not a #
	
	LD R2, INVALIDSIGN	; see how input relates to x3A aka ':'
	JSR COMPARE
	ADD R1, R1, #0
	BRn OPERAND		; input < x3A, its a #
	BRzp INVALIDEXPRESSION	; input >= x3A, all operators checked and not a # 

ISEQUAL
	JSR POP			; get value
	AND R3, R3, #0
	ADD R3, R0, #0		; store that value in R3
	JSR POP			; checking if just one value
	ADD R5, R5, #0		
	BRz INVALIDEXPRESSION	; more than one value in stack
	AND R5, R5, #0		; clear R5
	ADD R5, R3, #0		; load result in R5
	BRnzp PRINT_HEX

INVALIDEXPRESSION	; to change
	LD R0, INVALIDCHAR	; load 'I' into R0
	OUT			; print 'I'
	BRnzp	DONE

OPERAND
	JSR PUSH
	BRnzp EVALUATE

OPERATORA
	;ST R7, OPERATOR_SaveR7
	JSR POP
	ADD R3, R0, #0
	JSR POP
	ADD R4, R0, #0
	ADD R5, R5, #0		; check for underflow
	BRp INVALIDEXPRESSION	; stack underflow, branch to print 'invalid expression'
	JSR ADDITION		; apply operator
	BRnzp OPERAND		; push

OPERATORS
	JSR POP
        ADD R3, R0, #0
	JSR POP
	ADD R4, R0, #0
        ADD R5, R5, #0          ; check for underflow
        BRp INVALIDEXPRESSION   ; stack underflow, branch to print 'invalid expression'
        JSR SUBTRACT		; apply operator
	BRnzp OPERAND		; push

OPERATORM
	JSR POP
	ADD R3, R0, #0
        JSR POP
	ADD R4, R0, #0
        ADD R5, R5, #0          ; check for underflow
        BRp INVALIDEXPRESSION   ; stack underflow, branch to print 'invalid expression'
        JSR MULTIPLY		; apply operator
	BRnzp OPERAND		; push 

OPERATORP
	JSR POP
	JSR POP
	ADD R5, R5, #0		; check for underflow
	BRp INVALIDEXPRESSION	; stack underflow, branch to print 'invalid expression'
	JSR POWER		; apply operator
	BRnzp OPERAND		; push
 
OPERATORD
	JSR POP
	JSR POP
	ADD R5, R5, #0		; check for underflow
	BRp INVALIDEXPRESSION	; stack underflow
	JSR DIVIDE		; apply operator
	BRnzp OPERAND		; push

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
INVALIDCHAR     .FILL x49
SUBFORTYEIGHT	.FILL #-48

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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


SPACE		.FILL x20		; space char
ZERO		.FILL x30
A		.FILL x41

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output
;
;
;EVALUATE

;your code goes here


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; input R3, R2
; out R1
;
COMPARE
	ST R3, COMP_SaveR3
	AND R1, R1, #0
	NOT R2, R2
	ADD R2, R2, #1	; make R2 negative
	ADD R3, R2, R3
	BRn NEG
	BRz DONE_COMP
	ADD R1, R1, #1
	BRnzp DONE_COMP
NEG
	ADD R1, R1, #-1
DONE_COMP
	LD R3, COMP_SaveR3
	RET

COMP_SaveR3	.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
ADDITION
	ADD R0, R3, R4		; add R3 and R4 into R0 
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
SUBTRACT	
;your code goes here

	ST R3, SUBTRACT_SaveR3	; save R3
	NOT R3, R3		
	ADD R3, R3, #1		; make R3 negative
	ADD R0, R3, R4		; subtract
DONE_SUBTRACT	
	LD R3, SUBTRACT_SaveR3	; restore R3
	RET 

SUBTRACT_SaveR3	.BLKW #1
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MULTIPLY	
;your code goes here

	ST R3, MULTIPLY_SaveR3	; save R3
	ST R4, MULTIPLY_SaveR4	; save R4
	AND R0, R0, #0
MULTLOOP
	ADD R0, R0, R3		; add R3 to R0 
	ADD R4, R4, #-1		; decrement loop counter
	BRp MULTLOOP		; keep multiplying 
DONE_MULTIPLY
	LD R3, MULTIPLY_SaveR3	; restore R3
	LD R4, MULTIPLY_SaveR4	; restore R4
	RET
		
MULTIPLY_SaveR3	.BLKW #1
MULTIPLY_SaveR4	.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0 (quotient)
DIVIDE	
;your code goes here
	
	ST R3, DIVIDE_SaveR3	; save R3
	ST R4, DIVIDE_SaveR4	; save R4
	AND R0, R0, #0		; clear out R0
DIVLOOP
	ADD R0, R0, #1		; add 1 to quotient
	ADD R3, R3, R4		; sub divisor from dividend
	BRzp DIVLOOP		; there is still division to do
	ADD R0, R0, #-1		; correct for going 1 loop too far 
DONE_DIVIDE	
	LD R3, DIVIDE_SaveR3	; restore R4
	LD R4, DIVIDE_SaveR4	; resotre R4
	RET

DIVIDE_SaveR3	.BLKW #1
DIVIDE_SaveR4	.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
POWER
;your code goes here

	ST R1, POWER_SaveR1	; store R1
	ST R3, POWER_SaveR3	; store R3
	ST R4, POWER_SaveR4	; store R4
	AND R0, R0, #0		; clear R0
	AND R1, R1, #0		; clear R1	
POWERLOOP
	ADD R1, R1, R3		; set inner loop counter
	; multiply R3 by itself R4 times
POWERINNERLOOP
	; multiply R3 by itself
	ADD R0, R0, R3		; add R3 to R0
	ADD R1, R1, #-1
	BRp POWERINNERLOOP	; havent finished multiplying R3 by itself
	ADD R4, R4, #-1		; decrement outer loop counter
	BRp POWERLOOP
DONE_POWER
	LD R1, POWER_SaveR1	; restore R1
	LD R3, POWER_SaveR3	; restore R3
	LD R4, POWER_SaveR4	; restore R4
	RET

POWER_SaveR1	.BLKW #1
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
