# check 1
# Load "check1.s"
# print_all_regs - to look at registery table
# < 91 lowercase - use branch blt (less than ) bgt (greater than)

.data

string: .asciiz "string\n"
exitc:  .word "DONE"

.text
.global main

main: sub $sp,$sp, 4     # allocate stack space of 4 for size of one character in memory being read
      sw $s0, 0($sp)     # indirect addressing
      li $v0,4           # setting up $v0 so that It can print the output


tolowerb: li $t1,90           # value 90 represents capital Z the end of the uperscase ascii value set 
          ble $a0,$t1         # if $a0 (input) <= 90 (which means its upper case )
          #beq $t1,$zero,PRINT # if condition doesnt hold and the value is already lower case then just print the value
          addi $v0,32         # assuimng the value is capital add 32 beacuse 32 + (insert char) is the equivalent to the lowercase ascii case

tolower: 


PRINT:   li $a0,1             # Load 1 into $v0 sys call so that it prints an integer
         add $v0,$a0,$zero    # Copy q2 from $s0 into $a0. Adding $zero changes nothing; $a0 now contains the number to print
         syscall              # Print the character
                              
         addi $sp,$sp,4      # Move $sp back up by four bytes, releasing that space
         jr $ra       
