        ; 8080 assembler code
        .hexfile sender.hex
        .binfile sender.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 4
PRINT_MEM	equ 3
READ_B		equ 7
READ_MEM	equ 2
PRINT_STR	equ 1
READ_STR	equ 8
LOAD_EXEC	equ 5
PROCESS_EXIT 	equ 9
SET_QUANTUM	equ 6
PRINT_WHOLE	equ 10d	
RAND_INT	equ 12d
WAIT		equ 13d
SIGNAL		equ 14d



	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        

; Inspired from 131. Page in course book

MAX	equ 50d ; maximum mailbox size
MAX_MAIL_COUNT	equ 200d ; maximum # of mails (loop in loop  200+200)
MAILBOX_START_ADDR	equ 2003d ; mailbox starting address
MAILBOX_END_ADDR	equ 2052d
FULL	equ 2001d
EMPTY	equ 2002d
ID	equ 1d

begin:
	LXI SP,stack 	; always initialize the stack pointer
  
   	MVI A, 0
	LXI B, FULL
   	STAX B
   	MVI A, 50
   	LXI B, EMPTY
   	STAX B

   	MVI E,1
   	MVI D, 0 					

MainLoop:
	
	MVI A, 200
	SUB E
	CPI 0
	JZ FinishControl

SizeControl:
    LXI H, EMPTY 
	MOV A, M
	CPI 0
	JZ SizeControl
	DCR M
	
	LXI H, FULL
	MOV A, M
	LXI H, MAILBOX_START_ADDR

MailAddressCalc:
	CPI 0
	JZ InsertMail
	INX H
	DCR A
	JMP MailAddressCalc

InsertMail:
	MVI A, RAND_INT
	CALL GTU_OS           	
	MOV M, B

	LXI B,MAILBOX_START_ADDR

	MVI A, WAIT
	CALL GTU_OS           	

	LXI H, FULL	
	INR M 
	INR E
	EI
	JMP MainLoop

FinishControl: 

	MVI E, 1
	INR D
	MVI A,1
	SUB D
	CPI 0
	JZ MainLoop
	JMP Finish

Finish:
	MVI A, PROCESS_EXIT
	CALL GTU_OS