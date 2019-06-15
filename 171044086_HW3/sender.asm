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
MUTEX_ADDR	equ 2000d
SEM_FULL_ADDR	equ 2001d
SEM_EMPTY_ADDR	equ 2002d
ID	equ 1d

begin:
	LXI SP,stack 	; always initialize the stack pointer
   	MVI A, 1
   	LXI B, MUTEX_ADDR
   	STAX B
   	MVI A, 0
	LXI B, SEM_FULL_ADDR
   	STAX B
   	MVI A, 50
   	LXI B, SEM_EMPTY_ADDR
   	STAX B

   	MVI E,1
   	MVI D, 0 					

MainLoop:
	
	MVI A, 200
	SUB E
	CPI 0
	JZ FinishControl


SizeControl:
    LXI H, SEM_EMPTY_ADDR 
	MOV A, M
	CPI 0
	JZ SizeControl
	DCR M

MutexControl:
	LXI H, MUTEX_ADDR
	MOV A, M
	CPI 0
	JZ MutexControl			
	DI
	MVI M,0 						; critical region
	LXI H, SEM_FULL_ADDR
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

	LXI B,SEM_EMPTY_ADDR

	MVI A, WAIT
	CALL GTU_OS           	

	LXI H,MUTEX_ADDR   		
	MVI M,1

	LXI H, SEM_FULL_ADDR	
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