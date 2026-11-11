# Checkpoint 1
# Use normal SPIM settings with delayed branches/loads disabled.

.data
string: .space 32             # Up to 31 input characters plus '\0'
nl:     .asciiz "\n"          # Newline printed after each row
space:  .asciiz " "           # One space for indentation

.text
.globl main

main:
        addi $sp,$sp,-8       # Reserve space for two saved registers
        sw   $a0,0($sp)       # Save original $a0
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
        la   $a0,string       # Pass the original string's address
        jal  wordtri          # Call the triangle function

        lw   $a0,0($sp)       # Restore main's saved registers
        lw   $ra,4($sp)
        addi $sp,$sp,8        # Release the eight reserved bytes
        jr   $ra             # Return instead of falling into strlen


# ----------------------------------------------------------
# strlen
# Input:  $a0 = string address
# Return: $v0 = number of characters, excluding '\0'
# Leaves $a0 unchanged.
# ----------------------------------------------------------

strlen:
        move $t0,$a0          # Pointer to the current character
        li   $v0,0            # Start the length at zero

strlen_loop:
        lbu  $t2,0($t0)       # Load one character
        beq  $t2,$zero,strlen_done

        addi $t0,$t0,1        # Advance the pointer by one byte
        addi $v0,$v0,1        # Increase the character count
        j    strlen_loop

strlen_done:
        jr   $ra             # Return the length in $v0


# ----------------------------------------------------------
# strcpy
# Input:  $a0 = destination address
#         $a1 = source address
# Return: $v0 = destination address
#
# Copy every character, including the terminating zero.
# ----------------------------------------------------------

strcpy:
        move $t0,$a0          # Destination pointer
        move $t1,$a1          # Source pointer
        move $v0,$a0          # Save destination as the return value

strcpy_loop:
        lbu  $t2,0($t1)       # Read one byte from the source
        sb   $t2,0($t0)       # Store that byte in the destination

        beq  $t2,$zero,strcpy_done # Stop after copying '\0'

        addi $t0,$t0,1        # Advance destination pointer
        addi $t1,$t1,1        # Advance source pointer
        j    strcpy_loop

strcpy_done:
        jr   $ra


# ----------------------------------------------------------
# wordtri
# Input: $a0 = original string address
#
# Allocate and modify a COPY of the original string.
# Print each row with one extra indentation space.
# Remove one character from each end after each row.
# ----------------------------------------------------------

wordtri:
        addi $sp,$sp,-24      # Reserve space for six saved registers
        sw   $s0,0($sp)       # Preserve the saved registers we use
        sw   $s1,4($sp)
        sw   $s2,8($sp)
        sw   $s3,12($sp)
        sw   $a0,16($sp)      # Save original string argument
        sw   $ra,20($sp)      # Save return address before function calls

        move $s0,$a0          # Keep the original string's address
        jal  strlen           # Find its length
        move $s1,$v0          # Keep the length in a saved register

        beq  $s1,$zero,wordtri_done # Empty string: nothing to print

        addi $a0,$s1,1        # Request length + 1 bytes for the copy
        li   $v0,9            # Syscall 9: sbrk, allocate memory
        syscall               # Allocated address is returned in $v0

        move $s3,$v0          # Save the allocated address

        move $a0,$s3          # Destination: allocated memory
        move $a1,$s0          # Source: original input string
        jal  strcpy           # Copy the string, including '\0'

        move $s0,$s3          # Current row starts at the copied string
        li   $s2,0            # First row has zero indentation spaces

Loop:
        li   $t0,0            # Spaces printed so far on this row

indent_loop:
        beq  $t0,$s2,print_word # Stop when enough spaces are printed

        li   $v0,4            # Print-string syscall
        la   $a0,space        # Address of one space
        syscall

        addi $t0,$t0,1        # Count the space just printed
        j    indent_loop

print_word:
        li   $v0,4            # Print the current string
        move $a0,$s0          # Current row's starting address
        syscall

        li   $v0,4            # Print a newline after the row
        la   $a0,nl
        syscall

        li   $t0,3
        slt  $t1,$s1,$t0      # $t1 = 1 if current length is less than 3
        bne  $t1,$zero,wordtri_done
                              # A row of length 1 or 2 is the final row

        addi $s0,$s0,1        # Skip the first character for the next row
        addi $s1,$s1,-2       # New length: remove one from each end

        add  $t0,$s0,$s1      # Address immediately after the new last character
        sb   $zero,0($t0)     # End the shortened COPY with '\0'

        addi $s2,$s2,1        # Next row gets one more indentation space
        j    Loop

wordtri_done:
        lw   $s0,0($sp)       # Restore all registers saved by wordtri
        lw   $s1,4($sp)
        lw   $s2,8($sp)
        lw   $s3,12($sp)
        lw   $a0,16($sp)
        lw   $ra,20($sp)

        addi $sp,$sp,24       # Release all 24 reserved bytes
        jr   $ra             # Return to main