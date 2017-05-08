# Question 5 - Perform a Double Arithmetic Right Shift ($a0, $a1) by N bits ($a2)

.data

.text
main:

addi $a0 $a0 0x0ABC
mul $a0 $a0 0x04
addi $a1 $a1 0xDEF5
mul $a1 $a1 0x04
addi $a2 $a2 0x08
jal DOUBLESHIFT
j exit

exit: 
      li $v0 10
      syscall


DOUBLESHIFT:

and $t0 $t0 0x00              #clear $t0
and $t1 $t1 0x00              #clear $t1
addi $t1 $t1 0x20             #put value 32 in $t1
sub $t1 $t1 $a2               #make $t1 32-N
sllv $t0 $a0 $t1              #put the left bits needed for $v1 in $t0
srlv $v1 $a1 $a2              #do the low order bit shift
or $v1 $v1 $t0                #or in the high order bit shift
srlv $v0 $a0 $a2              #do the high order biy shift
jr $ra                        #return
