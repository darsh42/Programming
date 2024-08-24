#PURPOSE - Given a number, this program computes the
#           factorial. For example, the factorial of
#           3 is 3 * 2 * 1, or 6. The factorial of
#           4 is 4 * 3 * 2 * 1, or 24, and so on.
#

# Will show recursion in ASM

.section .data

# No global data

.section .text
.globl _start
_start:
  pushl $4            # Push the first parameter on stack
  call factorial      # call factorial function
  addl $4, %esp       # reset stack pointer

  movl %eax, %ebx     # move the result to %ebx
  movl $1, %eax       # load the sys call into %eax
int $0x80             # throw interrupt


# Function definition
.type factorial, @function
factorial:
  pushl %ebp          # push current base pointer to stack
  movl %esp, %ebp     # load current stack pointer into %ebp
  movl 8(%ebp), %eax  # load first parameter into %eax


  cmpl $1, %eax       # |BASE CASE| check if %eax == 1
  je end_factorial    # if cmpl true then goto end_factorial
  decl %eax           # decrement %eax
  pushl %eax          # push current %eax for recursive call
  call factorial      # |RECURSIVE CALL|
  movl 8(%ebp), %ebx  # Load original first value as %eax has result
                      # from previous computations
                      # This is because we can never know the value of
                      # registers after a function call

  imull %ebx, %eax    # multiply by the result from the last call of
                      # factorial, the result will be stored in 
                      # %eax which is where the return value should be

end_factorial:
  movl %ebp, %esp
  popl %ebp
ret

