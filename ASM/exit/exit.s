#PURPOSE: Simple program that returns status code to kernel
#
#

#INPUT: none
#
#

#OUTPUT: return status code. can be viewed by typing
#
#     echo $?
#
#   after running the program
#

#VARIABLES:
#     %eax holds the system call number
#     %ebx holds the return status
#
.section .data

.section .text
.globl _start
_start:
movl $1, %eax     # linux kernel command
                  # number (system call) for exiting
                  # a program

movl $0, %ebx     # status number we will return
                  # to the operating system
                  # Change this and it will return different things
                  # echo$?

int $0x80         # wakes up kernel to run
                  # the exit command


