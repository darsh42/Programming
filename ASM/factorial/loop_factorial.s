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
  pushl $5          # push the parameters
  call factorial    # call the function
  addl $4, %esp     # derefernce parameters


  movl %eax, %ebx   # move the return value to %ebx
  movl $1, %eax     # move status number
int $0x80         # throw interrupt


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

.type factorial, @function
factorial:
  pushl %ebp          # Push current base pointer value
  movl %esp, %ebp     # load current stack pointer into %ebp
  subl $4, %esp       # make room for result

  movl 8(%ebp), %ebx  # load the first param into %ebx
  movl $1, -4(%ebp)   # load $1 into result

factorial_loop_start:
  cmpl $0, %ebx       # check if loop has reached 0
  je factorial_end    # if it has end the loop
  movl -4(%ebp), %eax # temporarily move result to %eax
  imull %ebx, %eax    # multiply %ebx and %eax and store in %eax

  movl %eax, -4(%ebp) # move %eax into result
  decl %ebx           # decrement %ebx
  jmp factorial_loop_start

factorial_end:
  movl -4(%ebp), %eax # move the result into %eax
  movl %ebp, %esp     # reset the stack pointer
  popl %ebp           # reset the base pointer
ret                   # return the value computed
