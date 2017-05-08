# Question 2 - Create a function ‘bintohex’ to Convert a Binary number to a Hexadecimal null-terminated string
.data
	equal_text: .asciiz " = 0x"
	return_buffer: .space 12
	a0_arguments: .word 0x000000AA, 0x0000AA00, 0x00AA0000, 0xAA000000
	
.text
main:
	li $s0, 4 # loop limit
	li $s1, 0 # counter\
	la $s2, a0_arguments # arg pointer
	
main_loop:
	lw $a0, ($s2)
	li $v0, 1
	syscall
	
	la $a0, equal_text # print middle text
	li $v0, 4
	syscall 
	
	lw $a0, ($s2)
	la $a1, return_buffer
	jal bintohex
	
	la $a0, return_buffer
	li $v0, 4
	syscall
	
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	addiu $s1, $s1, 1
	addiu $s2, $s2, 4
	blt $s1, $s0, main_loop
	
exit:	li $v0, 10
	syscall

bintohex:
	addiu $sp, $sp, -4 # push ra
	sw $ra, 0($sp)
	
	li $t0, 8 # loop counter
	move $t1, $a0
	move $t2, $a1
	
bintohex_main_loop:
	and $t3, $t1, 0x0F
	and $t4, $t4, $zero
	
	blt $t3, 0xA, bintohex_num_digit
	j bintohex_alpha_digit
	
bintohex_num_digit:
	add $t4, $t4, 0x30
	j bintohex_main_loop_rest
bintohex_alpha_digit:
	add $t4, $t4, 0x41
	sub $t3, $t3, 0x0A

bintohex_main_loop_rest:	
	add $t4, $t4, $t3
	sb $t4, ($t2)
	
	srl $t1, $t1, 1
	addiu $t2, $t2, 1
	subiu $t0, $t0, 1
	
	bnez $t0, bintohex_main_loop
	
bintohex_return:
	sb $zero, 8($a1)

	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	
	jr $ra