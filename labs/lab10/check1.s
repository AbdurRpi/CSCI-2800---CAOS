#check 1
.data
times:   .asciiz "times"
is:      .asciiz "is"
space:   .asciiz " "
newline: .asciiz "\n"
numbers: .word 0x00000001, 0x00000002, 0x00000003, 0x00000004,
         .word 0x00000005, 0x00000006, 0x00000007, 0x00000008,
         .word 0x00000009, 0x0000000a, 0x0000000b, 0x00000000
         .word 0x00000000, 0x00000000, 0x00000000, 0x0000000f
.text
.globl main
   brunson: li $t3,1
            li $t4,4

            move $a0,$t3
            li $v0,1       #print_int 
            syscall        # 1

            move $t6,$a0
            la $a0,space
            li $v0,4       #print_str
            syscall        # " " 

            move $t2,$a0
            la $a0,is
            li $v0,4       #print_str
            syscall        # is 

            move $t6,$a0
            la $a0,space
            li $v0,4       #print_str
            syscall        # " "

            move $a0,$t4
            li $v0,1       # print_int
            syscall        # 4

            move $t6,$a0
            la $a0,space
            li $v0,4       #print_str
            syscall        # " "

            move $t0,$a0
            la $a0,times
            li $v0,4 # print_str
            syscall
            
            move $t6,$a0
            la $a0,space
            li $v0,4       #print_str
            syscall        # " "

            sll $t1,$t0,2
            move $a0,$t1
            li $v0,1 # print_int
            syscall        # 12, 20 .....

            addi $t3,$t3,2
            jr $ra
            
   apeno:   add $sp,$sp,-8
            sw $s0,0($sp)
            sw $ra,4($sp)
            move $s0,$a0
   oker:    lbu $a0,0($s0)
            beq $a0,$zero,gravity
            jal brunson
            addi $s0,$s0,8
            j oker
   gravity: la $a0,newline
            li $v0,4 # print_str
   lab10:   syscall
            lw $ra,4($sp)
            lw $s0,0($sp)
            add $sp,$sp,8
            jr $ra
   main:    addi $sp,$sp,-8
            sw $s0,0($sp)
            sw $ra,4($sp)
            la $a0,numbers
            jal apeno
   here:    lw $ra,4($sp)
            lw $s0,0($sp)
            addi $sp,$sp,8
            jr $ra

