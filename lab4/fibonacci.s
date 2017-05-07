# Question 3 -  Create a recursive function that computes the Fibonacci value

.data
	pre_string: .asciiz "f("
	mid_string: .asciiz ") = "
	max_fibonacci_num: .word 10
.text
main:	lw $t0, max_fibonacci_num # Init print limit
	li $t1, 0 # init counter
	
main_loop:
	li $v0, 4
	la $a0, pre_string
	syscall # Print initial string
	
	li $v0, 1
	move $a0, $t1
	syscall # print argumemt to fib function
	
	li $v0, 4
	la $a0, mid_string
	syscall # Print middle string part
	
	move $a0, $t1 # set argument
	jal fibonacci # call fibonacci function
	
	move $a0, $v0 # move returned value to print
	li $v0, 1
	syscall # print returned value 
	
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	addiu $t1, $t1, 1 # increment counter
	ble $t1, $t0, main_loop # check if still in desirable range
	
exit:	li $v0, 10
	syscall
	
fibonacci:
	addiu $sp, $sp, -4 # push ra
	sw $ra, 0($sp)
	
	addiu $sp, $sp, -4 # push s0
	sw $s0, 0($sp)
	
	addiu $sp, $sp, -4 # push a0
	sw $a0, 0($sp)
	
	beqz $a0, fibonacci_base_0 # test for first base case
	beq $a0, 1, fibonacci_base_1 # test for second base case
	
	lw $a0, 0($sp) # f(n - 1) referenced from stack
	addi $a0, $a0, -1 
	jal fibonacci # call fucntion recursively
	move $s0, $v0 # load accumulator with inital value
	
	lw $a0, 0($sp) # f(n - 2) referenced from stack
	addi $a0, $a0, -2
	jal fibonacci # call function recursively
	add $s0, $s0, $v0 # add to accumulator
	
	move $v0, $s0 # load return value and jump to return sequence
	j fibonacci_return # move to return sequence
	
fibonacci_base_0:
	li $v0, 0
	j fibonacci_return
fibonacci_base_1:
	li $v0, 1
fibonacci_return:
	lw $a0, 0($sp) # pop a0
	addiu $sp, $sp, 4

	lw $s0, 0($sp) # pop s0
	addiu $sp, $sp, 4
	
	lw $ra, 0($sp) # pop ra
	addiu $sp, $sp, 4
	
	jr $ra # return from function
