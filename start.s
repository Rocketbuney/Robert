.data
gpio_path:              ;@ path to gpio file
  .ascii "/dev/gpiomem\0\0"

gpio_fd:                ;@ returned file descripter from open()
  .word 1

.global gpio_ptr        ;@ pointer to mmaped memory region
gpio_ptr:
  .word 4

.text
.macro syscall, name, value
 .global \name          ;@ basic system call macro
\name:
  mov r7, \value
  swi #0
  bx lr
.endm

syscall write, #4
syscall open, #5
syscall close, #6
syscall exit, #1

.global _start
.type _start, %function
_start:
  ldr r0, =gpio_path  ;@get ready for open()
  ldr r1, =#1052674   ;@ O_RDWR | O_SYNC
  bl open             ;@ call open 

  ldr r1, =gpio_fd    ;@ load gpio_fd address
  str r0, [r1]        ;@ store file descryptor
                      ;@ get ready for mmap2()
  mov r4, r0          ;@ move returned file pointer to be passed to mmap2
  mov r0, #0
  mov r1, #4096       ;@ block size
  mov r2, #3          ;@ PROT_READ | PROT_WRITE
  mov r3, #1          ;@ MAP_SHARED
  mov r5, #0          ;@ expose GPIO base address
  mov r7, #192        ;@ set syscall code for mmap2()
  swi #0              ;@ mmap2(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED, 3, 0x500)
  
  ldr r1, =gpio_ptr   ;@ load memory address of gpio_ptr
  str r0, [r1]        ;@ store the returned memory address in gpio_ptr

  bl robert_main      ;@ start c code
  
  push {r0}
  ldr r1, =gpio_fd    ;@ presserve c code return value
  ldr r0, [r1]        ;@ load back in the file descryptor
  bl close            ;@ cleanup program 
  pop {r0}

  bl exit             ;@ exit program
