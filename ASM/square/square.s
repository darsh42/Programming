#PURPOSE: recive an arg and return square
#
#
#
#
.section .data


.section .text
.globl _start
_start:
  pushl $4
  call square
  addl $4, %esp

  movl %eax, %ebx
  movl $1, %eax
int $0x80

#PURPOSE: compute square
#
#
#INPUTS: single value
#
#
#OUTPUTS: square
#
#
#VARIABLES:
#         %eax - holds the parameter
#
.type square, @function
square:
  pushl %ebp
  movl %esp, %ebp
  movl 8(%ebp), %eax
  imull %eax, %eax
  movl %ebp, %esp
  popl %ebp
ret


