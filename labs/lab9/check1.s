#check 1 
# load "check1.s"
# print_all_regs

# Service Calls 
# 1 print integer
# 4 print string
# 8 read string
# 10 exit program

.data
string:  .space 32
nl: .asciiz "\n"

.text
.globalmain
main:
            addi $sp,$sp, -8. #saving regustery
            sw $a0,0($sp)
            sw $ra,4($sp)

            li $v0, 8     #read call put address of input into $v0
            la $a0, string
            li $a1, 32
            syscall        #Read input into string

            la $t0, string
            lbu $t2,0($t0)  #load unsigned bite string[0]
            beq $t2, $zero, End # if not input end program


strlen:     
         lbu $t2,0($t0)
         addi $t0,$t0,1
         slt $t0, $t0, $v0
         bne $t1, $zero, nl
         j strlen:


# Use same features from lab 8 to trace through a words input 


strcpy:



wordtri:



End:
   lw $ra,0($sp)
   addi $sp,$sp, 4
   jr $ra


   Psuedo Code
main:
Start by allocating space and saving registery's
save $a0 as a constant string
strlen:
then for string length we need to find out how manby letters are in the given string
then with return the value of leters in the given string in $a0
save that value as $a0
strcpy:
      copy the string in its current state so the same core letter get printed repeatedly

wordtri:
         using sbrk (malloc) we can create an array so that w can repeadedly remove 2 values 0 and -1(the end most)
         must only remove 2 values for each itteration
         then call strcpy to print the string again 





use the wordlen function to get every character and then print -1 on 
both sides of the word so each word looses -2 each iteration down
loop through the printing and call the strlen function
