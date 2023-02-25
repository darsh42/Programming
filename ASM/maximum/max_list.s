#PURPOSE:
# find the largest value in a list of items
#
#

#INPUT:
# list of numbers
#
#

#OUTPUT:
# max value in list
#
#

#VARIABLES:
# data_items -> start of list (marker)
# last item will be 0, (marker)
# current_position -> %edi
# current_max_val -> %ebx
# current_val -> %eai
# 

.section .data

data_items:           # data items
  .long 3,67,34,222,45,75,54,34,44,255,22,11,66,0

.section .text

.globl _start
_start:
                            # starting parameters
  movl $0, %edi             # move 0 into the index register
  movl data_items(,%edi,4), %eax # load the first byte
  movl %eax, %ebx           # since this is the first item, %eax
                            # the biggest

start_loop:
  cmpl $0, %eax             # check for end of list
  je loop_exit              # jump to marker "loop_exit" if true
  incl %edi                 # load next value
  movl data_items(,%edi,4), %eax 
  cmpl %ebx, %eax           # compare values
  jle start_loop            # IF: current val < current largest value,
                            #   jump to start loop

  movl %eax, %ebx           # ELSE: move the value as the largest
  jmp start_loop            # jump to loop beggining

loop_exit:
  # %ebx is the status code for the system call
  # and it already has the maximum number
  movl $1, %eax
  int $0x80


