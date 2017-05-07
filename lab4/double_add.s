# Question 4 -  Double fixed-point arithmetic add double_add(ahi, alo, bhi, blo)

main:

addi $a1 $a1 0x1234
mul $a1 $a1 0x8000
mul $a1 $a1 0x0002
addi $a3 $a3 0xFFFF
mul $a3 $a3 0x8000
mul $a3 $a3 0x0002
addi $a0 $a0 0x1234
addi $a2 $a2 0xFFFF
jal DOUBLEADD

exit: li $v0 10
syscall

DOUBLEADD:

addu $v1 $a1 $a3
sltu $t0 $v1 $a1
add $v0 $a0 $a2
add $v0 $v0 $t0
#function for bintohex here
jr $ra
