data
greater: .asciiz ">"
newline: .asciiz "\n"
numbers: .word 0x00000001, 0x00000002, 0x00000003, 0x00000004,
.word 0x00000005, 0x00000006, 0x00000007, 0x00000008,
.word 0x00000009, 0x0000000a, 0x0000000b, 0x00000000
.word 0x00000000, 0x00000000, 0x00000000, 0x0000000f
.text
.globl main
brunson: move $t0,$a0
la $a0,greater
li $v0,4 # print_str
syscall
sll $t1,$t0,2
move $a0,$t1
li $v0,1 # print_int
syscall
jr $ra
apeno: add $sp,$sp,-8
sw $s0,0($sp)
sw $ra,4($sp)
move $s0,$a0
oker: lw $a0,0($s0)
beq $a0,$zero,gravity
jal brunson
addi $s0,$s0,8
j oker
gravity: la $a0,newline
li $v0,4 # print_str
lab10: syscall
lw $ra,4($sp)
lw $s0,0($sp)
add $sp,$sp,8
jr $ra
main: addi $sp,$sp,-8
sw $s0,0($sp)
sw $ra,4($sp)
la $a0,numbers
jal apeno
here: lw $ra,4($sp)
lw $s0,0($sp)
addi $sp,$sp,8
jr $ra