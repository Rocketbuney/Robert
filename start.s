.text

			;@ system call macro 
.macro syscall, name, value
 .global \name
\name:
	mov r7, \value
	swi #0
	bx lr
.endm

			;@ define system calls
syscall write, #4
syscall open, #5
syscall close, #6

.global _start
_start:
        bl main		;@ call the main c function
	
	mov r7, #1	;@ load syscall for exit
	swi 0		;@ execute the syscall

