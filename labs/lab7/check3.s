# check 2
# Load "check3.s"
# print_all_regs - to look at registery table


# C implamentation
#int iterations = 0;

#while (q2 > -13) {
#    q2 -= 10;
#    iterations++;
#}

#printf("==> the answer is %d\n", q2);
#printf("==> loop iterations: %d\n", iterations);



.data
msg:        .asciiz "==> the answer is "
msg2:       .asciiz "==> loop iterations: "
newline:    .asciiz "\n"

.text
main: li $t0,4             # temp sotoring 4 bytes Load Imaedtaley(li)
      sub $sp,$sp,$t0     # make four bytes of the stack space
      sw $s0,0($sp)        # Save original $s0 - 0($sp) means the memory address in $sp plus 0
      
      li $s0,13            # Load Imediately(li) q2 = 13
      li $t0,15            # 15
      add $t1,$t0,$s0      # q2 = 15 + q2
      li $t0, 11           # 11
      sub $s0,$t1,$t0      # q2 = 15 + q2 - 11 

      li $t2, 0            # Loop count start at 0
      
LOOP: li $t0, -13          # 13
      slt $t1,$t0,$s0      # if (q2 > -13), $t1 = 1 and if condition doesnt hold its 0
      beq $t1,$zero,PRINT  # if $t1 is 1 then skip the subtraction

      li $t0,10             # 10
      sub $s0,$s0,$t0      #  q2 -= 10

      li $t0,1
      add $t2,$t2,$t0       # incrementing the loop count by adding 1 to itself

      j LOOP               # loops back to check condition of q2        
PRINT:
      li $v0,4             # stores 4 in empty value and will print 4 in sys call
      la $a0,msg           # Put the adress of message in $a0 for sys call

      syscall              # print msg from above

      li $v0,1             # Put 1 in $v0: syscall 1 means print an integer
      add $a0,$s0,$zero    # Copy q2 from $s0 into $a0. Adding $zero changes
                           # nothing; $a0 now contains the number to print
      syscall              # Print that q2

      li $v0,4             # Select the print-string syscall again
      la $a0,newline       # Put the address of the "\n" string in $a0
      syscall              # Print the newline, moving output to the next line

      li $v0,4
      la $a0,msg2
      syscall               # prints the loop count 

      li $v0,1
      add $a0,$t2,$zero    # Copy the iteration count into $a0
      syscall              # Print the count as an integer

      li $v0,4             # Select the print-string syscall again
      la $a0,newline       # Put the address of the "\n" string in $a0
      syscall              # Print the newline, moving output to the next line

      lw $s0,0($sp)        # Load the original $s0 value saved on the stack.
                           # 0($sp) means the memory address in $sp plus 0.
      
      li $t0,4             # Put 4 in $t0: we reserved four stack bytes
      add $sp,$sp,$t0      # Move $sp back up by four bytes, releasing that space
      jr $ra               # Jump to the return address; main is finished


      





