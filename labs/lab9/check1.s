# check 1 
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
         lbu $t2,0($t0)       # string[0]
         addi $t0,$t0,1       # string[ 0 + 1] = string[1] = string[2]
         addi $t1, $t0        # t1 has the count of how many bytes are in th string which is its length
         #slt $t0, $t0, $$t0. # keep counting until string[0] !< string[0]
         bne $t0, $zero, nl   # break if the counting hits a \n
         j strlen


# Use same features from lab 8 to trace through a words input 

strcpy:
       lw $ra, string         loads the word in its current state for example BRACADBR
       jr $ra
         

wordtri:    
         lbu $a0, $zero, string. # loads (word ex:) ABRACADABRA
         subi $t2, $t2, 1 # subract the first index -1 = BRACADBRA
         subi $t2, $t1, 1 # subtract the last element that was counted -1 = BRACADBR
         lw $a0, strcpy 
         slt $a0, 2     # if(len($a0 (the string)) < 2) 
         bne $ao, $zero, end

         j wordtri        # Loop through until


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
