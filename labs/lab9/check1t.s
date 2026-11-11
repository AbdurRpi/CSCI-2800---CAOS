# check 1


.data
string: .space 32
nl: .asciiz "\n"

.text
.globl main
main:
      addi $sp,$sp,-8 #saving regustry
      sw $a0,0(#sp)
      sw $ra,4($sp)


remove_nl:
         lbu $t2,0($t0)     #read the current character 
         beq $t2,$zero,line #if string hits a \0 end
         li  $t3,10         #loading ASCII newline 10 value
         beq $t2,$t3,replace_nl     #break one newline (nl)

         addi $t0,$t0,1    #advance one next character
         j remove_nl

repalce_nl:
         sb $zero,0($t0)   #replace newline with \0

line:    #input from the string
         la $a0,string
         jal wordtri    # call wordtri because new \0 is in place for new line printed
strlen:
         move $t0,$a0
         li $v0,0

strlen_loop:
         lbu $t2,0
         beq $t2,$zero,strlen_done

         addi $t0,$t0,1
         addi $v0,$v0,1
         j strlen_loop

strlen_done:
         jr $ra

strcpy:
         move $t0,$a0 #destination pointer for string
         move $t1,$a1 # source pointer of byte in the string
         move $v0,$a0 #return the destination address

strcpy_loop:
         lbu $t2,0($t1)  # Read one source byte
         sb  $t2,0($t0)  # copy it into the destination
         
         beq $t2,$zero,strcpy_done # copies the back \0 to be move to the last byte in the list

         addi $t0,$t0,1  # advance 1 destination from incrementing the pointer
         addi $t1,$t1,1  # advances 1 in the source (original string)
         j strcpy_loop

strpy_done:
         jr $ra
wordtri:
         
         sw $s0,0($a0)     #saving the original string as it was inputed
         lw $a0,strlen #get the word then moe down
         sll $t0, strlen, 0
         beq $$t0, $zero, End
         sbrk 

Loop:
      li $v0,4
      la $a0, strlen

      syscal

      li $v0,4
      la $a0, wordtri
      syscall

      li $v0,4
      la $a0, nl
      syscall

      sll $t0, strlen, 0   #if strlen < 0
      addi $t0, $t0, 1
      addi, $a0,$a0,-2
        

End:
      li $t0,8
      add $sp,$sp,$t0
      jr $ra