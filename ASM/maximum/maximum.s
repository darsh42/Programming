#PURPOSE: find maximum value in 3 list based on the pointer
#         the function 'maximum' will receive
#
#
#DATA: three lists will be defined
#
#
#
.section .data
list_1:
  .long 46,52,94,36,20,44,36,31,3,28,75,16,99,97,1,0
list_2:
  .long 7,63,47,63,87,68,6,82,19,95,79,28,98,23,86,0
list_3:
  .long 69,73,56,20,41,62,15,83,52,99,47,39,28,91,6,0

.section .text
.globl _start
_start:
  pushl [list_3]                  # push pointer to third list
  pushl [list_2]                  # push pointer to second list
  pushl [list_1]                  # push pointer to first list
  call maximum                  # call function maximum
  addl $12, %esp                # clean up the parameters from stack
  movl %eax, %ebx               # load return to %ebx to show after exit
  movl $1, %eax                 # load sys call into %eax
int $0x80                        # throw interrupt


#PURPOSE: find maximum within all the lists given
#
#
#INPUTS: pointers to where the lists start
#
#
#OUTPUTS: maximum value out of the lists given
#
#
#NOTES: might work with recursion
#
#
#VARIABLES:
#       
#

.type maximum, @function
maximum:
  pushl %ebp
  movl %esp, %ebp
  subl $4, %esp
  movl $4, %edi

start_loop:
  cmpl $1, %edi
  je end_loop
  movl -4(%ebp), %ebx
  movl (%ebp, %edi, 4), %eax
  subl $4, %edi
  addl %eax, %ebx
  movl %ebx, -4(%ebp)
  jmp start_loop

end_loop:
  movl -4(%ebp), %eax
  movl %ebp, %esp
  popl %ebp
ret

