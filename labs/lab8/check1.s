# check 1
# Load "check1.s"
# print_all_regs - to look at registery table
# < 91 lowercase - use branch blt (less than ) bgt (greater than)

.data

string: .asciiz "HELOWORLD\n"
exitc:  .asciiz "DONE\n"
newline: .asciiz "\n"

.text
.globl main

main: sub $sp,$sp, 4     # allocate stack space of 4 for size of one character in memory being read
      sw $s0, 0($sp)     # indirect addressing
      li $v0,4           # setting up $v0 so that It can print the output


tolowerb: li $t1,90            # value 90 represents capital Z the end of the uperscase ascii value set 
          ble $a0,$t1          # if $a0 (input) <= 90 (which means its upper case )
          #beq $t1,$zero,PRINT # if condition doesnt hold and the value is already lower case then just print the value
          addi $v0,32          # assuimng the value is capital add 32 beacuse 32 + (insert char) is the equivalent to the lowercase ascii case

tolower: la $s0,string
         lw $t0,0($s0)        # $a0 is the charcter string of the first element that can be lowered

         
PRINT:   move $a0,$t0
         li $v0,1             # Load 1 into $v0 sys call so that it prints an integer
         syscall              # Print the character

         la $a0,newline
         li $v0,4
         syscall

         lw $s0,4($sp)
         lw $a0,0($sp)                     
         add $sp,$sp,4      # Move $sp back up by four bytes, releasing that space
         jr $ra       
