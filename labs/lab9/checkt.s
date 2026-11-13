# check 2

.data
string: .space 32
nl: .asciiz "\n"
arrow: .asciiz "==>"

.text
globl main

main:   
        addi $sp,$sp,-8
        sw $a0,0($sp)
        sw $ra,4($sp)

        li   $v0,8            # Syscall 8: read a string
        la   $a0,string       # Address of the input buffer
        li   $a1,32           # Buffer capacity, including '\0'
        syscall

        la   $t0,string       # Start checking from the first character

        sb $t1,8($a0)
        li $t1, newline
        


checksum:
        la $a0,string
        la $t0,strlen
        li $t1, 0
        
checksum_loop:       #Wrong $t0 use?           # $t1 is the byte at pionter | $t2 is the byte at pointer + 1(second) | $t3 is the 
        slt $t0,$t0,2           # If $t0 < 2 | if bytes less than 2 
        bne $t0,$0,last_byte    # if less than 2 then break to last byte (for chuncking)

        addi $t0,$t0,1          # byte at pointer 
        addi $t1,$t1,1          # byte at pointer + 1 

        sll $t0,$t0,3           # devide each position by 8 because you are spliting the 16bit word into chuncks
        addi $t2,$t0,$t1        # pair which is the pair + second position
        addi $t3,$t2,$t1        # Sum which is the chuncked 2 bytes 

        srl $t5,$t3,4           # shift sum right by 16 bits
        li $t3, $t5
        addi $t3, $t3,$t5       # add sum to the carry 
        addi $t1,$t1,2          # advance the pointer by 2 
        addi $t1,$t1,-2

        j checksum_loop

checksum_end:
        jr $ra

remove_nl:
        lbu  $t2,0($t0)       # Read the current character 
        beq  $t2,$zero,line   # Stop at the terminating zero

        li   $t3,10           # ASCII 10 represents newline
        beq  $t2,$t3,replace_nl

        addi $t0,$t0,1        # Advance to the next character
        j    remove_nl

replace_nl:
        sb   $zero,0($t0)     # Replace newline with '\0'

strlen:
        move $t0,$a0          # Pointer to the current character
        li   $v0,0            # Start the length at zero

last_byte:
        slt $t0,$t0,0         # If $t0 < 0 
        bne $t1,$0, checksum_finish

        lbu  $t0,$a0
        andi $s2,$t0,$t1

        srl $t5,$t3,4           # shift sum right by 16 bits
        and  $t3,$t3 $t5
        addi $t3, $t3,$t5       # add sum to the carry 
        addi $t1,$t1,2          # advance the pointer by 2 
        addi $t1,$t1,-2

checksum_finish:
        and $t3,$t3,$t5
        li $v0,$t3
        jr $ra


#Psuedo

main

make allocation on the stack 

read in the data (string) at the memory of $a0 so sw $a0,0($sp0)
 call strlen to have length calculated and saved in $a1
 call Checksum to calculate and save into $v0

 main:
    Reserve stack space.
    Save the original $a0 and $ra.

    Read a string into the input buffer.

    Replace its newline with a terminating zero.
        This prevents Enter from becoming part of the checksum.

    Put the string address into $a0.
    Call strlen.
        Length comes back in $v0.

    Copy the length from $v0 into $a1.
    Put the string address into $a0.

    Call checksum.
        The checksum comes back in $v0.

    Restore the original $a0 and $ra.
    Release the reserved stack space.
    Return.

