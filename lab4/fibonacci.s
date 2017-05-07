# Question 3 -  Create a recursive function that computes the Fibonacci value

.data
	sep_string: .asciiz " - "
.text
main:	li $a0, 0
	jal fibonacci
	move $a0, $v0
	li $v0, 1
	syscall
	
exit:	li $v0, 10
	syscall
	
fibonacci:
	addiu $sp, $sp, -4 # push ra
	sw $ra, 0($sp)
	
	addiu $sp, $sp, -4 # push s0
	sw $s0, 0($sp)
	
	beqz $a0, fibonacci_base_0 # test for first base case
	addi $t0, $a0, -1
	beqz $t0, fibonacci_base_1 # test for second base case
	
	addi $a0, $a0, -1
	jal fibonacci
	move $s0, $v0
	
	addi $a0, $a0, -1
	jal fibonacci
	add $s0, $s0, $v0
	move $v0, $s0
	j fibonacci_return
	
fibonacci_base_0:
	li $v0, 0
	j fibonacci
fibonacci_base_1:
	li $v0, 1
fibonacci_return:
	lw $s0, 0($sp)
	addiu $sp, $sp, 4
	
	lw $ra, 0($sp) # pop ra
	addiu $sp, $sp, 4
	
	jr $ra
