# loop.s

.data
message: .asciiz "AAAAAAAAAAAAAAAA\n"
#                   ^
#                  $t0

.text
.globl main

main:   la $a0,message
        la $t0,message    # $t0 will be my loop iterator...
        li $t1,0          # $t1 will be my int loop iterator (stop at 16)
        li $t2,66   # 'B'

L0:     sb $t2,1($t0)    # store 1 byte instead of a 4-byte word

        li $v0,4    # print_str prints $a0 as a string
        syscall

        addi $t0,$t0,1   # move my pointer one byte forward in message
        addi $t1,$t1,1   # increment int loop iterator by 1   (i++)
        addi $t2,$t2,1   # 'B' ==> 'C', 'C' ==> 'D', etc.

        slti $t3,$t1,15  # $t3 = ( $t1 < 15 );          [$t3 is 0 or 1]
        bne $t3,$zero,L0 # if ( $t3 != 0 ) goto L0;

#        li $t5,15   # loop bound
#        slt $t3,$t1,$t5
#        bne $t3,$zero,L0

# TO DO: change the above to eliminate $t1 and use some other logic
#         based on $t0 (or $t2) to end the loop

        jr $ra

