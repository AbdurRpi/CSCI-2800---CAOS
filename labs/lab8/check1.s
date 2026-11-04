# check 1
# Load "check1.s"
# print_all_regs - to look at registery table
# < 91 lowercase - use branch blt (less than ) bgt (greater than)

# check1.s

.data
string:  .asciiz "HELOWORLD\n"
exitc:   .asciiz "DONE\n"       #
newline: .asciiz "\n"           # 

.text
.globl main

main:
      sub $sp,$sp,8           # Space for two saved registers
      sw   $s0,0($sp)          # Save original $s0
      sw   $ra,4($sp)          # Save return address before jal

      la   $a0,string          # $a0 must contain the string's address
      jal  tolower             # Convert the entire string

PRINT:
      la   $a0,string          # Address of the now-lowercase string
      li   $v0,4               # Syscall 4 prints a string
      syscall                  # Prints "heloworld\n"

      lw   $s0,0($sp)          # Restore original $s0
      lw   $ra,4($sp)          # Restore original return address
      addi $sp,$sp,8           # Release the eight stack bytes
      jr   $ra

tolowerb:
      lbu  $v0,0($a0)          # Read ONE byte at the address in $a0
      li   $t1,65              # ASCII 'A'
      blt  $v0,$t1,EOB    # Below 'A': leave it unchanged

      li   $t1,90              # ASCII 'Z': your 90 boundary
      bgt  $v0,$t1,EOB    # Above 'Z': leave it unchanged

      addi $v0,$v0,32          # A–Z becomes a–z
      sb   $v0,0($a0)          # Write the changed byte to memory

EOB:                           # (EOB) - End of Byte
      jr   $ra                 # Return the byte in $v0

tolower:
      addi $sp,$sp,-8
      sw   $s0,0($sp)          # Save $s0 because this function uses it
      sw   $ra,4($sp)          # Save $ra because jal will change it
      move $s0,$a0             # $s0 points to the current character

LOOP:
      lbu  $t0,0($s0)          # Read current character
      beq  $t0,$zero,END_STRING # Zero byte means end of string

      move $a0,$s0             # Pass this character's ADDRESS to tolowerb
      jal  tolowerb
      addi $s0,$s0,1           # Advance to the next byte
      j    LOOP

END_STRING:
      lw   $s0,0($sp)
      lw   $ra,4($sp)
      addi $sp,$sp,8
      jr   $ra