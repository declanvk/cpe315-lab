# Question 5 - Perform a Double Arithmetic Right Shift ($a0, $a1) by N bits ($a2)

.data

.text
main:

addi $a0 $a0 0x0ABC
mul $a0 $a0 0x04
addi $a1 $a1 0xDEF5
mul $a1 $a1 0x04
addi $a2 $a2 0x02
jal DOUBLESHIFT
j exit

exit: 
      li $v0 10
      syscall


DOUBLESHIFT:

and $t0 $t0 0x00
and $t1 $t1 0x00
addi $t1 $t1 0x20
sub $t1 $t1 $a2
sllv $t0 $a0 $t1
srlv $v1 $v1 $a2
and $v1 $v1 $t0
srlv $v0 $v0 $a2
jr $ra
