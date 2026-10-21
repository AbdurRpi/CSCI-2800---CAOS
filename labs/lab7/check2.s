# check 2
#Lab07-q2.s
# Load "check2.s"
# print_all_regs - to look at registery table

.data
msg:        .asciiz "==> the answer is"
newline:    .asciiz "\n"

.text
main: li $t0,4             # temp sotoring 4 bytes Load Imaedtaley(li)
      sub $sp,$sp,$t0     # make four bytes of the stack space
      sw $s0,0($sp)        # Save original $s0 - 0($sp) means the memory address in $sp plus 0
      
      li $s0,13            # Load Imediately(li) q2 = 13
      li $t0,15            # 15
      add $t1,$t0,$s0      #  q2 = 15 + q2
      li $t0, 11           # 11
      sub $t0,$t1,$t0      # q2 = 15 + q2 - 11 
      # answer should be -7 im off by 4 somewhere in my aritmetic find erorr for this and check 3 aswell
      li $t0, -13          # 13
      slt $t1,$t0,$s0      # if (q2 > -13), $t1 = 1 and if condition doesnt hold its 0
      beq $t1,$zero,PRINT  # if $t1 is 1 then skip the subtraction

      li $t0,10             # 10
      sub $s0,$s0,$t0      #  q2 -= 10
PRINT:
      li $v0,4             # stores 4 in empty value and will print 4 in sys call - v0 was used to choose what to print 
      la $a0,msg           # Put the adress of message in $a0 for sys call

      syscall              # print

      li $v0,1             # Put 1 in $v0: syscall 1 means print an integer
      add $a0,$s0,$zero    # Copy q2 from $s0 into $a0. Adding $zero changes
                           # nothing; $a0 now contains the number to print
      syscall              # Print that integer

      li $v0,4             # Select the print string syscall again
      la $a0,newline       # Put the address of the "\n" string in $a0
      syscall              # Print the newline, moving output to the next line

      lw $s0,0($sp)        # Load the original $s0 value saved on the stack.
                        
      li $t0,4             # Put 4 in $t0 reserved four stack bytes
      add $sp,$sp,$t0      # Move $sp back up by four bytes, releasing that space
      jr $ra               # Jump to the return address; main is finished


      





