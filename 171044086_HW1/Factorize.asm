; 8080 assembler code
        .hexfile Factorize.hex
        .binfile Factorize.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy 
  ; OS call list
PRINT_B    equ 4
PRINT_MEM  equ 3
READ_B    equ 7
READ_MEM  equ 2
PRINT_STR  equ 1
READ_STR  equ 8

  ; Position for stack pointer
stack   equ 0F000h

  org 000H
  jmp begin

  ; Start of our Operating System
GTU_OS:  PUSH D
  push D
  push H
  push psw
  nop  ; This is where we run our OS in C++, see the CPU8080::isSystemCall()
    ; function for the detail.
  pop psw
  pop h
  pop d
  pop D
  ret
  ; ---------------------------------------------------------------
  ; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE              

	;Given an integer, your code will find and show integer factors of it. For
	
	;0-255 factorizing program


abc:  dw 'Factor of ', 00H
abcd:  dw ' is ',  00H
abcde:  dw ',',  00H

begin:
	LXI SP,stack 	; always initialize the stack pointer
    MVI A, READ_B
    CALL GTU_OS

    MOV H,B
 	LXI B,abc
    MVI A, PRINT_STR
    CALL GTU_OS

    MOV B,H
    MVI A, PRINT_B
    CALL GTU_OS

    LXI B,abcd
    MVI A, PRINT_STR
    CALL GTU_OS
    mvi c, 0	    ; init c with 0
    MOV B,H
    mvi L,128
initial:
	MOV A,H    
    INR C       ;
    SUB c           ; a = a - c
	PUSH A
	MOV A,L
	SUB C
	JC end
	POP A
	JC end          ;
prog:
	SUB c           ; a = a - c
	JC initial          ;
	jZ	kat			;
	jMP prog        ;

	MOV B, A	; Load the address of the printed value to the B and C registers
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	hlt		; end program
kat: 
	MOV D,C

	LXI B,abcde
    MVI A, PRINT_STR
    CALL GTU_OS
	
	MOV C,D
	MVI D,0

	MOV B,C       ;
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	JMP initial ; 
end :
	DCR H
	MOV D,C	
	LXI B,abcde
    MVI A, PRINT_STR
    CALL GTU_OS
	
	MOV C,D

	MOV B, H	; Load the address of the printed value to the B and C registers
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS

	hlt		            ; end program