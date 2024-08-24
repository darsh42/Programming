#PURPOSE:
# find the smallest value in a list of items
#
#

#INPUT:
# list of numbers
#
#

#OUTPUT:
# min value in list
#
#

#VARIABLES:
# data_items -> start of list (marker)
# last item will be 0, (marker)
# current_position -> %edi
# current_min_val -> %ebx
# current_val -> %eai
# 

.section .data
# data items
data_items:
  .long 3,4,67,34,222,45,75,54,34,44,254,22,11,66,1

.section .text

.globl _start
_start:
  movl $0, %edi                   # load %edi with first index 0
  movl data_items(,%edi, 4), %eax # load %eax with item from data_items with %edi index
  movl %eax, %ebx                 # Since first value, it is automatically the smallest

start_loop:
  cmpl $1, %eax                   # compare value in %eax to 0
  je loop_exit                    # IF equal then jump to loop_exit
  incl %edi                       # increment %edi
  movl data_items(,%edi,4), %eax  # move next value in data list into %eax
  cmpl %eax, %ebx                 # compare %ebx with %eax
  jle start_loop                  # IF %eax is greater than %ebx jump to start_loop

  movl %eax, %ebx                 # move value from %eax to %ebx
  jmp start_loop                  # jmp to start_loop

loop_exit:                        # exit call
  movl $1, %eax                   # load %eax with exit flag
  int $0x80                       # interrupt
