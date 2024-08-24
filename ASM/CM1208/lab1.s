#PURPOSE: Calculate value of X for expression below
#         X = 3 * (10A - (B + 2))/ (A + B) - (B - 2)

.section .data
# Example used
# A = 4
# B = 5
#

.section .text
.globl _start
_start:
    pushl $5        # push the parameter
    pushl $4        # push the parameter
    call function   # call the function
    addl $8, %esp   # clean stack

    movl %eax, %ebx # move result into %ebx to hold status
                    # number for echo $?
    movl $1, %eax   # load correct system call
int $0x80           # throw interrupt

# PURPOSE: complete calculation
#
#
#
# INPUT: A, B
#    
#
#
#
# OUTPUT: X = 3 * (10A - (B + 2))/ (A + B) - (B - 2)
#
# VARIABLES:
#           %eax - stores first param  (A)
#           %ebx - stores second param (B)
#           %ecx as tmp storage


.type function, @function
function:
    pushl %ebp                # push current %ebp
    movl %esp, %ebp           # move current %esp -> %ebp

    addl $4, %esp

    movl 8(%ebp), %eax       # load param 1 int %eax
    movl 12(%ebp), %ebx        # load param 2 into %ebx

function_right:
    imull $10, %eax           # 10A
    addl $2, %ebx             # B + 2
    subl %ebx, %eax           # 10A - (B + 2)
    imull $3, %eax            # 3 * 10A - (B + 2)
    movl %eax, -4(%ebp)           # store result tmp

    movl 8(%ebp), %eax       # reload param
    movl 12(%ebp), %ebx        # reload param

    addl %ebx, %eax           # A + B
    movl %eax, %ecx
    movl -4(%ebp), %eax
    subl $2, %ebx             # B - 2
    idivl %ecx                # current result / (A + B)
    subl %ebx, %eax           # current result - (B - 2)

function_end:
    movl %ebp, %esp           # reset stack pointer
    popl %ebp                 # reset base pointer
ret                           # return computed value
