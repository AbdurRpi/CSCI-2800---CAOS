# Lab 08, Checkpoint 3
# 
#  Width in $a2 must be from 1 through 32.

.data 0x10010000
newline: .asciiz "\n"               # Address 0x10010000
zerotxt: .asciiz "-"                # Address 0x10010002
onetxt:  .asciiz "$"                # Address 0x10010004
counttxt:.asciiz " --> '1' bits: "   # Address 0x10010006

.data 0x10010100
secret: .word 0x00020000, 0x40070000
        .word 0x60048000, 0x90044000
        .word 0x88044000, 0x84043f00
        .word 0x820400c0, 0x8189807e
        .word 0x8233c003, 0x4461803e
        .word 0x248823d0, 0x150003d0
        .word 0xe600f020, 0x89006120
        .word 0x45000040, 0x250000c0
        .word 0x16000030, 0x0c000188
        .word 0x08000244, 0x1000043c
        .word 0x20000200, 0x20000100
        .word 0x20ff0100, 0x1300c200
        .word 0x12002180, 0x3c001fe0
        .word 0x00000000

# A separate fixed address makes selecting secret2 straightforward.
.data 0x10010200
secret2:.word 0x7808301c, 0x30363063
        .word 0x337f7f63, 0x8000737f
        .word 0x36303c3f, 0x36303333
        .word 0x737f8000, 0x7e3e3763
        .word 0x3c60363e, 0x3303737e
        .word 0x00000000

# The three test words shown on the Checkpoint 3 sheet.
.data 0x10010300
example:.word 0x0ff03c3c, 0xf00ff00f, 0x3c3c0ff0
        .word 0x00000000

.text
.globl printb
.globl bcount
.globl main

# Unchanged from Checkpoint 2:
# print zerotxt when $a1 is zero; otherwise print onetxt.
printb:
        add  $t3,$a0,$zero       # Save the word being processed
        lui  $a0,0x1001
        addi $a0,$a0,4           # Start with the address of onetxt
        bne  $a1,$zero,print_bit # Exactly one conditional branch
        lui  $a0,0x1001
        addi $a0,$a0,2           # Zero bit: select zerotxt instead
print_bit:
        addi $v0,$zero,4         # Select print-string syscall
        syscall
        add  $a0,$t3,$zero       # Restore the current word
        jr   $ra

# Input: $a0 = one 32-bit word; $a2 = row width, from 1 to 32.
# Always process all 32 bits, most significant bit first.
# Each row prints at most $a2 bits and its own count.
# Return the total number of 1 bits in the word in $v0.
bcount:
        addi $sp,$sp,-8          # Reserve space for two saved values
        sw   $a0,0($sp)          # Save the original word
        sw   $ra,4($sp)          # Save return address before jal printb

        addi $t0,$zero,0         # Number of 1 bits in the current row
        addi $t1,$zero,32        # Total bits still to process
        addi $t5,$zero,0         # Number of bits printed in this row
        addi $t6,$zero,0         # Total 1 bits in the entire word

loop:
        lui  $t2,0x8000          # Mask: 0x80000000
        and  $a1,$a0,$t2         # Isolate the current highest bit
        jal  printb              # Print '-' or '$'

        beq  $a1,$zero,skip      # A zero bit does not increase either count
        addi $t0,$t0,1           # Count a 1 bit in this row
        addi $t6,$t6,1           # Count a 1 bit in the whole word

skip:
        sll  $a0,$a0,1           # Move the next bit to the highest position
        addi $t1,$t1,-1          # One fewer bit remains in this word
        addi $t5,$t5,1           # One more bit was printed in this row

        beq  $t5,$a2,print_count # Full row: display its count
        beq  $t1,$zero,print_count # End of word: display even a short row
        j    loop               # Otherwise continue the current row

# This is a label inside bcount, not another function.
print_count:
        add  $t4,$a0,$zero       # Save the shifted word before printing text

        lui  $a0,0x1001
        addi $a0,$a0,6           # Address of counttxt
        addi $v0,$zero,4         # Print " --> '1' bits: "
        syscall

        add  $a0,$t0,$zero       # Argument: this row's count
        addi $v0,$zero,1         # Print the integer count
        syscall

        lui  $a0,0x1001          # Address of newline
        addi $v0,$zero,4         # Print the newline
        syscall

        add  $a0,$t4,$zero       # Restore the remaining shifted word
        addi $t0,$zero,0         # Start the next row's count at zero
        addi $t5,$zero,0         # Start the next row's length at zero

        bne  $t1,$zero,loop      # Continue if this word still has bits left

        add  $v0,$t6,$zero       # Return total 1 bits for the entire word
        lw   $ra,4($sp)          # Restore the caller's return address
        lw   $a0,0($sp)          # Restore the original input word
        addi $sp,$sp,8           # Release the reserved stack space
        jr   $ra

main:
        addi $sp,$sp,-16         # Reserve space for four saved registers
        sw   $a0,0($sp)
        sw   $a1,4($sp)
        sw   $ra,8($sp)
        sw   $s1,12($sp)

        addi $a2,$zero,16        # WIDTH: change to any number from 1 to 32

        lui  $s1,0x1001
        addi $s1,$s1,0x0200      # DATA: secret2 at 0x10010200
        # Use 0x0100 above for secret, or 0x0300 for the sheet's example.

repeat:
        lw   $a0,0($s1)          # Load one complete 32-bit word
        beq  $a0,$zero,done      # A full zero word marks the end of the array
        jal  bcount              # Print all 32 bits using the chosen width
        addi $s1,$s1,4           # Move to the next four-byte word
        j    repeat

done:
        lw   $s1,12($sp)         # Restore the saved registers
        lw   $ra,8($sp)
        lw   $a1,4($sp)
        lw   $a0,0($sp)
        addi $sp,$sp,16          # Release all 16 reserved bytes
        jr   $ra

