# load-store.s

# the data segments contains program data, e.g., static arrays,
#  hard-coded strings, etc.
.data
array:    .word    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
abcd:     .byte    3000   # ???????????????
welcome:  .asciiz  "ABCDEFGH\n"
newline:  .asciiz  "\n"

# the data segment is similar to this C code:
#
# int array[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
#
# char * welcome = "ABCDEFGH\n";
#
# char * newline = "\n";
#

# the text segment contains our program code
.text
.globl main

main:
    sub $sp,$sp,8    # allocate stack space to store $a0
    sw $a0,0($sp)    # store $a0 onto the stack ($sp is the stack pointer)
         # ^^^^^^ indirect addressing --- pointer arithmetic *(sp+0)=a0
    sw $s0,4($sp)    # store $s0 onto the stack ($sp is the stack pointer)
         # ^^^^^^ indirect addressing --- pointer arithmetic *(sp+4)=s0

    la $a0,welcome   # load the address of welcome into register $a0
    li $v0,4         # set up $v0 to print the string pointed to by $a0
    syscall          # print_string

# TO DO: extend this to use a loop to print all 16 values of array

    la $s0,array     # load the address of array into register $s0
    lw $t0,32($s0)   # load the address of array + 32 into register $t0
                     # note that this is equivalent to array[8] or *(array+8)

    # print the value, then print a newline
    move $a0,$t0     # copy register value $t0 into register $a0
    li $v0,1         # set up $v0 to print the int in register $a0
    syscall          # print_int

    la $a0,newline
    li $v0,4
    syscall

    lw $s0,4($sp)    # restore $s0 to its initial value
    lw $a0,0($sp)
    add $sp,$sp,8
    jr $ra
