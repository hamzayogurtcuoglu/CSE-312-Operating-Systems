; 8080 assembler code
        .hexfile ShowPrimes.hex
        .binfile ShowPrimes.com
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

  ;file that prints all integer from 0 to 1000 on the screen if the number is
    ;prime than it will print “prime” next to number. Each number will be printed on a new line.


   ; This program just 0-255 prime number but print to 0-1000 


abc:  dw ' PRIME', 0AH, 00H
abcd:  dw ' ', 0AH, 00H
until: DB 00D
	   DB 10D
begin:
  LXI SP,stack   ; always initialize the stack pointer
  
MADD:
  
  MVI B,0
  MVI A, PRINT_B   
  CALL GTU_OS
  						
  LXI B,abcd
  MVI A, PRINT_STR		;Printing the file first 0,1,2
  CALL GTU_OS


  MVI B,1
  MVI A, PRINT_B	
  CALL GTU_OS

  LXI B,abcd
  MVI A, PRINT_STR
  CALL GTU_OS
  
  MVI B,2				;Start of Prime is 2
  MVI A, PRINT_B
  CALL GTU_OS

  LXI B,abc
  MVI A, PRINT_STR 
  CALL GTU_OS
MVI A, 2		;a = 2
MVI D, 2		;d = 2
first:  		
  mvi c, 1	   	;c = 1 	  
  INR D         ;d++
init:  
  INR C         ;c++ 
  MOV A,D       ; A = D
loop:         
  SUB c         ; A = a-c   
  JC cont       ; If a is zero that mean D is not divide by C 
  JZ no         ; If a is zero that mean D can divide by C that divider
  JMP loop      
cont:
  MOV A,D       ;A = D
  INR C         ;c++
  CMP C         ; IF A - C then zeroflag = 1 else 0
  JZ yes        ; If is prime
  DCR C         ; c--
  JMP init      
yes:
  MOV B,D      ; B = D
  MVI A, PRINT_B   
  CALL GTU_OS

  LXI B,abc
  MVI A, PRINT_STR
  CALL GTU_OS
  MVI H,255          ;I gonna find to 255 prime
  MOV A,D
  CMP H
  JZ BITTI
  JMP first	  
no:
  MOV B,D
  MVI A, PRINT_B
  CALL GTU_OS
                         ;If it is not prime just number that is written the file
  LXI B,abcd
  MVI A, PRINT_STR
  CALL GTU_OS
  	
  MVI H,255
  MOV A,D
  CMP H          
  JZ BITTI	 	

  JMP first
BITTI:
	
	MVI D,2    ;2 is 200 steps
 	MVI E,55    ; this part 2 - (55) we can present 255 with two register   
loop2:
	MVI A,100          ; if E is 99 D increasing exp : 3 - (00)
	INR E
  	CMP E
	JZ loop3
	MOV B,D
  	MVI A, PRINT_B
  	CALL GTU_OS
  	
  	MVI A,0
  	CMP E
  	JZ zero2
  	MVI A,1
  	CMP E
  	JZ zero2
  	MVI A,2
  	CMP E              ;We can not present two zero integer 
  	JZ zero2		   ;we write it
  	MVI A,3
  	CMP E
  	JZ zero2
  	MVI A,4
  	CMP E
  	JZ zero2
  	MVI A,5
  	CMP E
  	JZ zero2
  	MVI A,6
  	CMP E
  	JZ zero2
  	MVI A,7
  	CMP E
  	JZ zero2
  	MVI A,8
  	CMP E
  	JZ zero2
  	MVI A,9
  	CMP E
  	JZ zero2

  	
zero:
  	MOV B,E
  	MVI A, PRINT_B
  	CALL GTU_OS          

  	LXI B,abcd
  	MVI A, PRINT_STR
  	CALL GTU_OS	
  	
  	
  	JMP loop2
loop3:
  	MVI E,0
  	INR D
  	MVI A,10
  	CMP D
  	JZ BIT
  	MOV B,D
  	MVI A, PRINT_B
  	CALL GTU_OS

	MVI B,0
  	MVI A, PRINT_B   ;Zero writing file for 00
  	CALL GTU_OS

  	MVI B,0
  	MVI A, PRINT_B
  	CALL GTU_OS

  	LXI B,abcd
  	MVI A, PRINT_STR
  	CALL GTU_OS	

  	JMP loop2

BIT:
	MVI L,10
	MOV B,L	
  	MVI A, PRINT_B
  	CALL GTU_OS

  	MVI L,0
  	MOV B,L
  	MVI A, PRINT_B
  	CALL GTU_OS           ;1000 is not prime written with by one character

  	MVI L,0
  	MOV B,L
  	MVI A, PRINT_B
  	CALL GTU_OS

  	LXI B,abcd
  	MVI A, PRINT_STR
  	CALL GTU_OS	
	
	HLT

zero2:
	MVI B,0
  	MVI A, PRINT_B
  	CALL GTU_OS
  	JMP zero