# check 1
# Load "check1.s"
# print_all_regs - to look at registery table
# < 91 lowercase , use branch blt (less than ) bgt (greater than)

# check1.s
# Service Calls 
# 1 print integer
# 4 print string
# 8 read string
# 10 exit program

.data
prompt: .asciiz "Enter a string (DONE to exit): "
string: .space 32                                  #reserve for input
exitc:  .asciiz "DONE"                             # Done for exit value

.text
.globl main
main:
READ:                          #loop to ask for anothe string
      li $v0,4                 #print call for print teh address of $v0
      la $a0,prompt           
      syscall

      li $v0,8                 # read call put address of input in $v0
      la $a0,string
      li $a1,32
      syscall                  # Read input into string

      la $t0,string
      lbu $t2,0($t0)          # load the unsigned byte of string[0]
      beq $t2,$zero,EXIT      # End if there was no input
      la $t1,exitc

CHECK_DONE:
      lbu $t2,0($t0)           # Current input byte
      lbu $t3,0($t1)           # Current byte of "DONE"
      beq $t3,$zero,END_DONE  # All four letters matched
      bne $t2,$t3,CONVERT     # A letter did not match
      addi $t0,$t0,1          # increment through the word to check for match
      addi $t1,$t1,1          # increment through the string to check for match
      j CHECK_DONE

END_DONE:
      li $t4,10               # ASCII newline from pressing Enter
      beq $t2,$t4,EXIT        # Input was "DONE\n"
      beq $t2,$zero,EXIT      # Input was "DONE" without newline
      j CONVERT               # example "DONE123" is ordinary input

CONVERT:
      la $a0,string
      jal tolower

PRINT:
      li $v0,4
      la $a0,string
      syscall                  # Print the changed string
      j READ                   # Ask for another string

EXIT:
      li $v0,10
      syscall                  # Exit the program

# $a0 holds the address of one byte.
# $v0 returns that byte, changed or unchanged.
tolowerb:
      lbu $v0,0($a0)
      li $t0,65                # 'A'
      slt $t1,$v0,$t0         # Byte < 'A'?
      bne $t1,$zero,BYTE_END

      li $t0,90                # 'Z'
      slt $t1,$t0,$v0         # Byte > 'Z'?
      bne $t1,$zero,BYTE_END

      addi $v0,$v0,32          # Uppercase to lowercase
      sb $v0,0($a0)            # Store the changed byte

BYTE_END:
      jr $ra

# $a0 holds the address of the string.
tolower:
      addi $sp,$sp,-4
      sw $ra,0($sp)            # Save return address before jal

NEXT_BYTE:
      lbu $t0,0($a0)
      beq $t0,$zero,STRING_END # Stop at the ending zero byte
      jal tolowerb             # Convert the byte at $a0
      addi $a0,$a0,1           # Advance one byte
      j NEXT_BYTE

STRING_END:
      lw $ra,0($sp)
      addi $sp,$sp,4
      jr $ra