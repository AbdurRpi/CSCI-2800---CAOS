# check 2


main: li $s0,13            # Load Imediately(li) q2 = 13
      add $15,$s0          #  q2 = 15 + q2 - 11
      sub $11
      slt $s0 > 13         # if (q2 > 13) 
      sub $a0 - 10         #  q2 -= 10

      syscall              # printf

      jr $ra.

      