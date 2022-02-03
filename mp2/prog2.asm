;
;
;
.ORIG x3000
	
;your code goes here
	
;R0->reads input from keyboard
;R1->load and manipulate constants for char checking and checking at the end for extra '('
;R2->loads stack top to check if there are nay extra '('
;R5->

AND R5, R5, #0	;clear out R5
ADD R5, R5, #1	;zero of aything is balanced

EVALUATE
	ST R7, EVAL_SaveR7
	GETC	; get and echo out from keyboard
	OUT
	
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

	;LD R2, INVALIDSIGN	; see if input is ':'
	;JSR COMPARE
	;ADD R1, R1, #0
	;BRn OPERAND		; if negative x2F<input<x3A, thus an operand (0-9)
	;BRzp INVALIDEXPRESSION	; since '^' is already checked all remaining invalid. 

ISEQUAL	; to change
	OUT
	BRnzp DONE

INVALIDEXPRESSION	; to change
	LD R0, INVALIDCHAR	; load 'I' into R0
	OUT			; print 'I'
	BRnzp	DONE

OPERAND
	JSR PUSH
	;LD R7, EVAL_SaveR7
	BRnzp EVALUATE	

OPERATORA
	;ST R7, OPERATOR_SaveR7
	JSR POP
	JSR POP
	ADD R5, R5, #0		; check for underflow
	BRp INVALIDEXPRESSION	; stack underflow, branch to print 'invalid expression'
	JSR ADDITION		; apply operator
	BRnzp OPERAND		; push

OPERATORS
	JSR POP
        JSR POP
        ADD R5, R5, #0          ; check for underflow
        BRp INVALIDEXPRESSION   ; stack underflow, branch to print 'invalid expression'
        JSR SUBTRACT		; apply operator
	BRnzp OPERAND		; push

OPERATORM
	JSR POP
        JSR POP
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
;PRINT_HEX

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output
;
;
;EVALUATE

;your code goes here


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; input R0, R2
; out R1
;
COMPARE
	ST R3, COMP_SaveR3
	AND R1, R1, #0
	NOT R2, R2
	ADD R2, R2, #1	; make R2 negative
	ADD R3, R2, R0
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
	AND R0, R0, #0		; set R0 to 0
	ADD R0, R3, R4		; add R3 and R4 into R0 
	RET
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
SUBTRACT	
;your code goes here

	ST R3, SUBTRACT_SaveR3	; save R3
	AND R0, R0, #0		; set R0 to 0
	NOT R3, R3		
	ADD R3, R3, #1		; make R3 negative
	ADD R0, R1, R4		; subtract
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
MULTLOOP
	AND R0, R0, #0		; clear R0
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
	LD R4, STACk_TOP	;
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
