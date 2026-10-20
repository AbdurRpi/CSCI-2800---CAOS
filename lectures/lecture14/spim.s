# spim example
# use load "spim.s" to run a program then use step to see it running in parts
# when program hangs for parser label use reinitilaize to reset spim to compile the program

main: li $t1,7             # Load Imediate (li) g = 7
      li $t2,6             # h = 6
      li $t3, 5            # i = 5
      li $t4, 2            # j = 2

      add $t5,$t1,$t2      # t5 = g + h
      sub $t6,$t5,$t3       # t6 = t5 - i
      sub $a0,$t6,$t4       # f = t6 - j

      li $v0,1              #syscall 1 (print_int)
      syscall               # print $a0 register... ( ao - a3 are argument registers for calling a funciton) 4 different values can be called and passed into a funciton so it can be printed to termianl

      jr $ra.                # return to the calling function
























