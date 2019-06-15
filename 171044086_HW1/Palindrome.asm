; 8080 assembler code
        .hexfile Palindrome.hex
        .binfile Palindrome.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

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

 ;Show whether a string given from keyboard is a palindrome by printing the
 ;“string” semicolon: “Palindrome” or “Not Palindrome”

 ; Reading string then it is palindrome is not 


str:  ds 0FFH
abc:  dw ': Not Palindrome', 0AH, 00H
abcd:  dw ': Palindrome', 0AH, 00H
size:  dw 2D

STRLEN:
  PUSH D
  push B
  push H
  push psw
  
  LXI H, str    ;reading the file for a string 
  MVI A, 0D
  MVI E, 0
  loop:
    MOV B, M
    CMP B
    JZ done    ;Finding length of string
    INX H      ;if it coming to null of string 
    INR E      ;it will recognize the end of string then finish the loop
    JMP loop 
  done:
  pop psw
  pop H
  pop B
  JMP hamza     


begin:
  LXI SP,stack   ; always initialize the stack pointer
  
  LXI B, str
  MVI A, READ_STR
  CALL GTU_OS       
  MVI A, PRINT_STR
  CALL GTU_OS


 
  CALL STRLEN 
hamza:    
  LXI H, str     
  MVI A,0

last:   
  CMP E
  JZ cont
  INX H      ;going end of the string in memory adress
  INR B
  DCR E
  JMP last  
cont:
  DCR L
  MOV E, B ; character number copy again
  MOV D, E ; character number copy again
  MVI C, 0 ; character number copy again 
  DCR E
  DCR D

loopA:
  MVI A,0
  CMP D    
  JZ updat
  DCR L
  MOV A, M    ;getting character start part to middle
  DCR D
  JZ updat
  JMP loopA

loopB:
  MVI A,0
  MOV B, M
  CMP C    
  JZ upda     ;getting character end part to middle 
  DCR L
  DCR C
  JZ upda
  JMP loopB

com:
  pop A
  CMP B       ;if all character is same will be palidrome or not  
  JZ pali
  JMP npali
  
con:             ;if not yet coming to middle and not yet unsame 
  push A         ;character loop will continue
  MOV A,C     
  CMP D          
  JZ pali     
  pop A
  JMP loopA
  
  pali:

    LXI B,abcd
    MVI A, PRINT_STR   ;printing palindrome keyword
    CALL GTU_OS
    HLT

  npali:

    LXI B,abc
    MVI A, PRINT_STR  ;printing not palindrome keyword
    CALL GTU_OS
    HLT

updat:
  MOV D,E
  push A    ;updating index of start of left side string
  MOV A,D   
  SUB C
  DCR A         
  MOV D,A
  MVI A,0
retur:
  INX H
  CMP D
  JZ loopB  ;updating index of start of right side string
  INR A
  JMP retur  
  
upda:
  MOV C,E
  MOV A,C
  SUB D
  MOV C,A
  MVI A,0
retu:
  INX H
  CMP C
  JZ com
  INR A
  JMP retu  
  