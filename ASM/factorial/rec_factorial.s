#PURPOSE: Compute factorial of number 
#
#
#
#
#
#
.section .data


.section .text
.globl _start
_start:
  pushl $5
  call factorial
  addl $4, %esp

  movl %eax, %ebx
  movl $1, %eax
int $0x80

#PURPOSE: function to calculate factorial
#
#
#
#INPUT: single number
#
#
#
#OUTPUT: factorial
#
#NOTES:
#
#VARIABLES:
#     %ebx - stores first param
#     -4(%ebp) - stores result
#       -- starts with value $1
#     %eax as temp storage

factorial:
  pushl %ebp
  movl %esp, %ebp
  subl $4, %esp

  movl 8(%ebp), %ebx
  movl -4(%ebp), %eax

factorial_call:
  cmpl $0, %ebx
  je factorial_end

  pushl %ebx
  pushl %eax
  call factorial
  imull %ebx, %eax

factorial_end:
  movl $1, %eax
  movl %ebp, %esp
  popl %ebp
ret

