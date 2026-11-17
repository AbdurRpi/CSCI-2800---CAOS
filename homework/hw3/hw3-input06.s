jal G   # level two
G:addi $t1,$a1,-50 ###
VVVVV:beq $t1,$a2,LAKERS
srl $t8,$a3,3
or $v0,$s3,$t8
add $t1,$t1,$k0
j VVVVV#VVVVVVVVVVVVVVVVVVVVVVVVVVV noise
LAKERS:or $v1,$t8,$zero#gravity!
jr $a0
