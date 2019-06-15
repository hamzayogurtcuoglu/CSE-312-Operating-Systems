				; 8080 assembler code
        .hexfile receiver.hex
        .binfile receiver.com
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
GTU_OS:
	DI
	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	EI
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        

; Inspired from 131. Page in course book

MAILBOX_START_ADDR	equ 1491d ; mailbox starting address
MAILBOX_END_ADDR	equ 1540d
MUTEX_ADDR	equ 1488d
SEM_FULL_ADDR	equ 1489d
SEM_EMPTY_ADDR	equ 1490d

LOCAL_LIST equ 3001d    ;  
LOCAL_LIST_SIZE equ 3000d  ; This go to 255

begin:
	LXI SP,stack 	; always initialize the stack pointer

   	MVI A, 0
   	LXI B, LOCAL_LIST_SIZE
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
	CPI 50
	JZ SizeControl
	INR M

MutexControl:
	LXI H, MUTEX_ADDR
	MOV A, M
	CPI 0
	JZ MutexControl			
	DI
	MVI M,0 

	LXI H, SEM_FULL_ADDR	
	DCR M             ; critical region
	MOV A, M
	LXI H, MAILBOX_START_ADDR
	
MailAddressCalc:
	CPI 0
	JZ InsertLocal
	INX H
	DCR A
	JMP MailAddressCalc


InsertLocal:
	MOV B,M
	MVI M,0
	LXI H,LOCAL_LIST_SIZE
	MOV A,M
	LXI H, LOCAL_LIST

LocalAddressCal:
	CPI 0
	JZ InsertLocalCon
	INX H
	DCR A
	JMP LocalAddressCal

InsertLocalCon:
	MOV M,B
	LXI H,LOCAL_LIST_SIZE
	INR M

	LXI B,LOCAL_LIST_SIZE

	MVI A, SIGNAL
	CALL GTU_OS   

	LXI H,MUTEX_ADDR   		
	MVI M,1 

	LXI H, SEM_FULL_ADDR
	DCR M
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
