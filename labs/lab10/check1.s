#check 1
.data
times:   .asciiz "times"
is:      .asciiz "is"
space:   .asciiz " "
newline: .asciiz "\n"
numbers: .word 0x00000001, 0x00000002, 0x00000003, 0x00000004,
         .word 0x00000005, 0x00000006, 0x00000007, 0x00000008,
         .word 0x00000009, 0x0000000a, 0x0000000b, 0x0000000c,
         .word 0x0000000d, 0x0000000e, 0x0000000f,
         .word 0x00000010, 0x00000011, 0x00000012, 0x00000013,
         .word 0x00000014, 0x00000015, 0x00000016, 0x00000017,
         .word 0x00000018, 0x00000019, 0x0000001a, 0x0000001b,
         .word 0x0000001c, 0x0000001d, 0x0000001e, 0x0000001f,
         .word 0x00000020, 0x00000021, 0x00000022, 0x00000000
.text
.globl main
   brunson: move $t0, $a0
            li $t3,1
            li $t4,4

            #move $a0,$t0
            li $v0,1       #print_int 
            syscall        # 1

            #move $t6,$a0
            la $a0,space
            li $v0,4       #print_str
            syscall        # " " 

            #move $t2,$a0
            la $a0,times
            li $v0,4       #print_str
            syscall        # times 

            #move $t6,$a0
            la $a0,space
            li $v0,4       #print_str
            syscall        # " "

            move $a0,$t4
            li $v0,1       # print_int
            syscall        # 4

            #move $t6,$a0
            la $a0,space
            li $v0,4       #print_str
            syscall        # " "

            #move $t0,$a0
            la $a0,is
            li $v0,4 # print_str
            syscall
            
            #move $t6,$a0
            la $a0,space
            li $v0,4       #print_str
            syscall        # " "
            
            sll $t1,$t0,2
            move $a0,$t1
            li $v0,1 # print_int
            syscall        # 12, 20 .....

            la $a0,newline
            li $v0,4
            syscall

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
   gravity: ori $a0,$zero,10
            ori $v0,$zero,11 # print_str
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

