# expr.s

# to install spim, try:  bash$ sudo apt-get install spim

# from calculate.c code:
#  int g = 7, h = 6, i = 5, j = 2;
#  int f = g + h - i - j;


main:   li $t1,7                # Load Immediate (li)   g = 7
        li $t2,6                # h = 6
        li $t3,5                # i = 5
        li $t4,2                # j = 2

        add $t5,$t1,$t2         # t5 = g + h
        sub $t6,$t5,$t3         # t6 = t5 - i
        sub $a0,$t6,$t4         # f = t6 - j

        li $v0,1                # syscall 1 (print_int)
        syscall                 # print $a0 register...

        jr $ra                  # return to the calling function

