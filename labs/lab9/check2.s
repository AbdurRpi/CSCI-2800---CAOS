# Check 2: Internet checksum
# 
#
# checksum input:
# $a0 = starting address of the data
# $a1 = number of bytes to process
#
# checksum return:
# $v0 = calculated 16-bit checksum

.data
string: .space 32             # Up to 31 input characters plus '\0'
nl:     .asciiz "\n"          # Newline printed after each row
space:  .asciiz " "           # One space for indentation

.text
.globl main
.globl result
main:
        addi $sp,$sp,-8       # Reserve space for two saved registers
        sw   $a0,0($sp)       # Save the original argument register
        sw   $ra,4($sp)       # Save main's return address

        li   $v0,8            # Syscall 8: read a string
        la   $a0,string       # Address of the input buffer
        li   $a1,32           # Buffer capacity, including '\0'
        syscall

        la   $t0,string       # Start checking from the first character

remove_nl:
        lbu  $t2,0($t0)       # Read the current character
        beq  $t2,$zero,line   # Stop at the terminating zero

        li   $t3,10           # ASCII 10 represents newline
        beq  $t2,$t3,replace_nl

        addi $t0,$t0,1        # Advance to the next character
        j    remove_nl

replace_nl:
        sb   $zero,0($t0)     # Replace newline with '\0'

line:
        la   $a0,string       # Pass the string address to strlen
        jal  strlen           # Returns the length in $v0

        move $a1,$v0          # Pass the byte count to checksum
        la   $a0,string       # Pass the data address to checksum
        jal  checksum         # Returns the checksum in $v0

result:
        # Inspects $v0 
        lw   $a0,0($sp)       # Restore the original argument register
        lw   $ra,4($sp)       # Restore main's return address
        addi $sp,$sp,8        # Release the eight reserved bytes
        jr   $ra

strlen:
         # Input:  $a0 is string address
         # Return: $v0 = length, excluding the terminating zero
        move $t0,$a0          # Pointer to the current character
        li   $v0,0            # Start the length at zero

strlen_loop:
        lbu  $t2,0($t0)       # Read one character
        beq  $t2,$zero,strlen_done

        addi $t0,$t0,1        # Advance the pointer by one byte
        addi $v0,$v0,1        # Increase the character count
        j    strlen_loop

strlen_done:
        jr   $ra             # Return the length in $v0




# Chunck pairs by 2 bytes (AB) (BC) (DE), then a possible final byte.
# $t0 = pointer to the current byte
# $t1 = number of bytes remaining
# $t2 = accumulated sum
# $t3 = first byte, then the combined pair
# $t4 = second byte
# $t5 = carry above the lower 16 bits
# $t6 = comparison result
# $t7 = constant 2

# Only temporary registers are changed.
# No other functions are called, so no stack frame is needed.

checksum:
        move $t0,$a0          # Start at the supplied data address
        move $t1,$a1          # Number of bytes supplied by the caller
        li   $t2,0            # Start the sum at zero
        li   $t7,2            # Two bytes make one 16-bit pair

checksum_loop:
        slt  $t6,$t1,$t7      # $t6 = 1 if fewer than two bytes remain
        bne  $t6,$zero,last_byte

        lbu  $t3,0($t0)       # Read the first byte
        lbu  $t4,1($t0)       # Read the second byte

        sll  $t3,$t3,8        # Move the first byte into the upper 8 bits
        add  $t3,$t3,$t4      # Combine the two bytes into a 16-bit value

        add  $t2,$t2,$t3      # Add this pair to the accumulated sum

        srl  $t5,$t2,16       # Extract the carry above bit 15
        andi $t2,$t2,0xFFFF   # Keep only the lower 16 bits
        add  $t2,$t2,$t5      # Add the carry back into the sum

        addi $t0,$t0,2        # Advance the pointer past both bytes
        addi $t1,$t1,-2       # Two fewer bytes remain
        j    checksum_loop

last_byte:
        beq  $t1,$zero,checksum_finish # No leftover byte: finish

        lbu  $t3,0($t0)       # Read the one remaining unsigned byte
        add  $t2,$t2,$t3      # Add it directly, without shifting
                              # Example: 0x49 becomes 0x0049

        srl  $t5,$t2,16       # Extract any carry from this addition
        andi $t2,$t2,0xFFFF   # Keep only the lower 16 bits
        add  $t2,$t2,$t5      # Add the carry back into the sum

checksum_finish:
        xori $v0,$t2,0xFFFF   # Flip the lower 16 bits; return in $v0
        jr   $ra             # Return to the caller