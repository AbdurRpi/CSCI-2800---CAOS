# Checkpoint 3: Calculate and verify an Internet checksum

#
# checksum inputs:
# $a0 = starting address of data
# $a1 = number of bytes to process
# $a2 = 0 to calculate; nonzero to verify
#
# checksum return:
# Calculate mode: $v0 = 16-bit checksum
# Verify mode:    $v0 = 1 if valid, 0 otherwise

.data
string: .space 36             # Input plus room for padding and checksum
nl:     .asciiz "\n"
check_msg: .asciiz "Checksum (decimal): "
valid_msg: .asciiz "Verification (1 = valid, 0 = invalid): "

.text
.globl main
.globl result

main:
        addi $sp,$sp,-16      # Reserve space for four saved registers
        sw   $a0,0($sp)
        sw   $s0,4($sp)       # $s0 will hold the data length
        sw   $s1,8($sp)       # $s1 will hold the calculated checksum
        sw   $ra,12($sp)

        li   $v0,8            # Syscall 8: read a string
        la   $a0,string       # Address of input buffer
        li   $a1,32           # Read at most 31 characters plus '\0'
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
        la   $a0,string       # Pass input address to strlen
        jal  strlen           # Length is returned in $v0
        move $s0,$v0          # Save length across function calls

        la   $a0,string       # Address of data to calculate
        move $a1,$s0          # Number of input bytes
        li   $a2,0            # Mode 0: calculate the checksum
        jal  checksum

        move $s1,$v0          # Save checksum before printing syscalls

        li   $v0,4            # Print the checksum label
        la   $a0,check_msg
        syscall

        li   $v0,1            # Print the checksum as a decimal integer
        move $a0,$s1
        syscall

        li   $v0,4
        la   $a0,nl
        syscall

        # Prepare data for verification.
        # For odd lengths, the lab treats the last byte as 0x00XX.
        # Insert a zero BEFORE that last byte to preserve this grouping.

        andi $t1,$s0,1        # Low bit is 1 if the length is odd
        beq  $t1,$zero,add_checksum

        la   $t0,string
        add  $t0,$t0,$s0      # Address just past the original last byte

        lbu  $t2,-1($t0)      # Read the original final byte
        sb   $t2,0($t0)       # Move it one position to the right
        sb   $zero,-1($t0)    # Insert a zero before it

        addi $s0,$s0,1        # Include the inserted zero in the length

add_checksum:
        la   $t0,string
        add  $t0,$t0,$s0      # Address immediately after the prepared data

        srl  $t2,$s1,8        # Move checksum's upper byte into low position
        sb   $t2,0($t0)       # Store the checksum's upper byte first
        sb   $s1,1($t0)       # Store the checksum's lower byte second

        addi $s0,$s0,2        # Include both checksum bytes in the length

        la   $a0,string       # Address of data INCLUDING its checksum
        move $a1,$s0          # Total number of bytes to verify
        li   $a2,1            # Nonzero mode: verify
        jal  checksum

result:
        move $s1,$v0          # Save verification result before printing

        li   $v0,4
        la   $a0,valid_msg
        syscall

        li   $v0,1
        move $a0,$s1          # Print 1 for valid or 0 for invalid
        syscall

        li   $v0,4
        la   $a0,nl
        syscall

        move $v0,$s1          # Leave verification result in $v0

        lw   $a0,0($sp)       # Restore main's saved registers
        lw   $s0,4($sp)
        lw   $s1,8($sp)
        lw   $ra,12($sp)
        addi $sp,$sp,16       # Release all 16 reserved bytes
        jr   $ra



# strlen
# $a0 = string address
# Returns length in $v0, excluding '\0'


strlen:
        move $t0,$a0          # Pointer to current character
        li   $v0,0            # Start length at zero

strlen_loop:
        lbu  $t2,0($t0)
        beq  $t2,$zero,strlen_done

        addi $t0,$t0,1        # Advance the pointer
        addi $v0,$v0,1        # Increase the character count
        j    strlen_loop

strlen_done:
        jr   $ra



# checksum
# Process pairs: (AB), (CD), (EF), then a possible final byte.
#
# $t0 = current data pointer
# $t1 = bytes remaining
# $t2 = accumulated sum
# $t3 = first byte, then combined pair
# $t4 = second byte
# $t5 = carry
# $t6 = comparison result
# $t7 = constant 2
#

checksum:
        move $t0,$a0          # Start at the supplied data address
        move $t1,$a1          # Number of bytes to process
        li   $t2,0            # Start sum at zero
        li   $t7,2            # Two bytes form a 16-bit pair

checksum_loop:
        slt  $t6,$t1,$t7      # Check whether fewer than two bytes remain
        bne  $t6,$zero,last_byte

        lbu  $t3,0($t0)       # Read first byte
        lbu  $t4,1($t0)       # Read second byte

        sll  $t3,$t3,8        # First byte becomes upper eight bits
        add  $t3,$t3,$t4      # Combine the bytes

        add  $t2,$t2,$t3      # Add this pair to the sum

        srl  $t5,$t2,16       # Extract carry above the lower 16 bits
        andi $t2,$t2,0xFFFF   # Keep the lower 16 bits
        add  $t2,$t2,$t5      # Add the carry back into the sum

        addi $t0,$t0,2        # Advance past the two bytes
        addi $t1,$t1,-2       # Two fewer bytes remain
        j    checksum_loop

last_byte:
        beq  $t1,$zero,checksum_finish

        lbu  $t3,0($t0)       # Read the one remaining unsigned byte
        add  $t2,$t2,$t3      # Add directly: 0x49 becomes 0x0049

        srl  $t5,$t2,16       # Extract any carry
        andi $t2,$t2,0xFFFF   # Keep the lower 16 bits
        add  $t2,$t2,$t5      # Add carry back

checksum_finish:
        bne  $a2,$zero,verify_checksum # Nonzero mode means verification

        xori $v0,$t2,0xFFFF   # Calculate mode: flip the lower 16 bits
        jr   $ra             # Return calculated checksum

verify_checksum:
        li   $v0,0            # Default return value: invalid
        li   $t3,0xFFFF       # Expected sum BEFORE flipping
        bne  $t2,$t3,checksum_end

        li   $v0,1            # Sum equals 0xFFFF, so verification succeeds

checksum_end:
        jr   $ra