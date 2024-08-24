#PURPOSE: Program that computes n^m
#         - specifically 2^3 + 5^2 = 33
#         - Will help to illustrate how functions work
#
#
#ILLUSTRATION:
#
#             STACK
#
#         --------------
#   +     | Param_2    | <-- BOTTOM
#   ^     --------------
#   |     | Param_1    | <-- %ebp +8
#   |     --------------
#   |     | return     | <-- %ebp +4
#   |     --------------
#   |     | %ebp       | <-- (after params loaded %esp then vaulue stored in %ebp)
#   |     --------------
#   |     | result_1   | <-- %ebp -4
#   |     --------------
#   |     | result_2   | <-- %ebp -8
#   |     --------------
#   |     |            |
#   |     --------------
#   |     |            | <-- TOP
#   |     --------------
#
#
#Everything in the main program is stored in the main registers,
#so the data section doesn,t have any definitions.
.section .data


.section .text


.globl _start
_start:
  # parameter and function call convention in C
  pushl $3               # push second argument i.e. "m"
  pushl $2               # push first argument
  call power            # call function
  addl $8, %esp          # move stack pointer back
  pushl %eax             # save first ans, all results from
                        # functions are stored in the %eax register

  pushl  $2               # push second argument i.e. "m"
  pushl  $5               # push first argument i.e. "n"
  call power            # call function
  addl $8, %esp           # move stack pointer back

  popl %ebx              # pop the first answer



  addl %eax, %ebx        # add the results
  movl $1, %eax          # move sys call
int $0x80                # throw interrupt



#PURPOSE: Funtion used to calculate power
#         of n^m
#
#
#
#INPUT: First Argument - 'n'
#       Second Argument - 'm'
#
#
#
#OUTPUT: will give the result of the computation
#
#NOTES: Power 'm' must be greater than 1
#
#VARIABLES: 
#         %ebx - first argument 'n'
#         %ecx - second argument 'm'
#         -4(%ebp) - holds the result
#
#         %eax is used as temp var
#
.type power, @function
power:
  pushl %ebp            # push current %ebp
  movl %esp, %ebp       # load %ebp with current stack, it will tell where params and result are
  subl $4, %esp         # make room for the results

  movl 8(%ebp), %ebx    # put first arg in %ebx
  movl 12(%ebp), %ecx   # put second arg in %ecx
  movl %ebx, -4(%ebp)

power_loop_start:
  cmpl $1, %ecx         # check if power has reached 1
  je end_power
  movl -4(%ebp), %eax   # temporarily store current result for computation
  imull %ebx, %eax      # multiply current ans and base and store in %eax

  movl %eax, -4(%ebp)   # store current result on stack
  decl %ecx             # decrement %ecx
  jmp power_loop_start # jump to the start of the loop

end_power:
  movl -4(%ebp), %eax   # store result in %eax
  movl %ebp, %esp       # restore stack pointer
  popl %ebp
ret














