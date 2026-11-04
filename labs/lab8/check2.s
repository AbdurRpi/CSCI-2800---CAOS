#LCheckpoint 2
# 
#zerotxt: .asciiz "-"
#onetxt:  .asciiz "$"

# lab08-checkpoint2.s
# Use normal SPIM settings with delayed branches disabled.

.data 0x10010000
newline: .asciiz "\n"               # Address: 0x10010000
zerotxt: .asciiz "0"                # Address: 0x10010002
onetxt:  .asciiz "1"                # Address: 0x10010004
counttxt:.asciiz " --> '1' bits: "   # Address: 0x10010006

# Place the array at a known address.
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
        .word 0x00000000            # End of secret

secret2:.word 0x7808301c, 0x30363063
        .word 0x337f7f63, 0x8000737f
        .word 0x36303c3f, 0x36303333
        .word 0x737f8000, 0x7e3e3763
        .word 0x3c60363e, 0x3303737e
        .word 0x00000000        # indicates end of secret2 data

.text
.globl printb
.globl bcount
.globl main


# Print "0" if $a1 is zero, otherwise print "1".
# Preserve $a0 because bcount is using it for the current word.

printb:
        add  $t3,$a0,$zero       # Save the current word

        lui  $a0,0x1001         # $a0 = 0x10010000
        addi $a0,$a0,4          # Address of onetxt

        bne  $a1,$zero,print_bit # If nonzero, print onetxt

        lui  $a0,0x1001         # Otherwise, select zerotxt
        addi $a0,$a0,2          # Address of zerotxt

print_bit:
        addi $v0,$zero,4        # Select print-string syscall
        syscall

        add  $a0,$t3,$zero      # Restore the current word
        jr   $ra               # Return to bcount


# Input: $a0 contains a 32-bit word.
# Print its bits and the number of 1 bits.
# Return the count in $v0.

bcount:
        addi $sp,$sp,-8         # Reserve space for two registers
        sw   $a0,0($sp)        # Save the original input word
        sw   $ra,4($sp)        # Save the return address

        addi $t0,$zero,0        # Number of 1 bits counted
        addi $t1,$zero,32       # Number of bits remaining

loop:
        lui  $t2,0x8000         # Mask: 0x80000000
        and  $a1,$a0,$t2        # Isolate the highest bit

        jal  printb            # Print that bit

        beq  $a1,$zero,skip     # If the bit was zero, skip counting
        addi $t0,$t0,1          # Otherwise, count one more 1 bit

skip:
        sll  $a0,$a0,1          # Move the next bit to the highest position
        addi $t1,$t1,-1         # One fewer bit remains
        bne  $t1,$zero,loop     # Repeat until all 32 bits are processed

        lui  $a0,0x1001          #address of '1'
        addi $a0,$a0,6          # Address of counttxt
        addi $v0,$zero,4        # Print " --> '1' bits: "
        syscall

        add  $a0,$t0,$zero      # Put the count into $a0
        addi $v0,$zero,1        # Select print-integer syscall
        syscall

        lui  $a0,0x1001         # Address of newline
        addi $v0,$zero,4        # Select print-string syscall
        syscall

        add  $v0,$t0,$zero      # Return the count in $v0

        lw   $ra,4($sp)        # Restore the return address
        lw   $a0,0($sp)        # Restore the original input word
        addi $sp,$sp,8         # Release the eight reserved bytes
        jr   $ra              # Return to main


main:
        addi $sp,$sp,-16       # Reserve space for four registers of 4 bytes each
        sw   $a0,0($sp)        # stack location of original $a0
        sw   $a1,4($sp)         # stack location of original $a1
        sw   $ra,8($sp)         # stack location of return address $ra, needs to be saved before jal changes
        sw   $s1,12($sp)        # stack location of original $s1

        lui  $s1,0x1001        # $s1 = 0x10010000
        addi $s1,$s1,0x0100    # $s1 = 0x10010100: address of secret

repeat:
        lw   $a0,0($s1)        # Read the current word
        beq  $a0,$zero,done    # Stop at the zero end marker

        jal  bcount           # Print and count this word's bits

        addi $s1,$s1,4         # Advance to the next four-byte word
        j    repeat

done:
        lw   $s1,12($sp)       # Restore the saved registers
        lw   $ra,8($sp)
        lw   $a1,4($sp)
        lw   $a0,0($sp)
        addi $sp,$sp,16        # Release all 16 reserved bytes
        jr   $ra