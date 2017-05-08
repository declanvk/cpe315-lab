# Question 6 - Use of Shift instructions to Obtain Selected Bit Fields

EXTRACT:

andi $t0 $t0 0x00
addi $t0 $t0 0xF000
and $v0 $t0 $a0
sub $t0 $t0 $t0
addi $t0 $t0 0x0100
srl $a0 $a0 0x0B #shift right 11
andi $t1 $t1 0x00
and $t1 $t0 $a0
add $v0 $v0 $t1
sub $t0 $t0 $t0
addi $t0 $t0 0x0073
srl $a0 $a0 0x0E #shift right 14
and $t1 $t0 $a0
add $v0 $v0 $t1
jr $ra
