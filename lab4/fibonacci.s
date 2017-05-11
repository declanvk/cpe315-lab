# Question 3 -  Create a recursive function that computes the Fibonacci value

.data
	fibonacci_pre_string: .asciiz "f("
	fibonacci_mid_string: .asciiz ") = "
	fibonacci_a0_arguments: .word 0, 1, 5, 10, 20
	fibonacci_num_test: .word 5
.text
fibonacci_main:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -8 # clear space for arguments and return address
	sw $ra, -4($fp)
	sw $a0, -8($fp)
	
	lw $t0, fibonacci_num_test # Init print limit
	la $t1, fibonacci_a0_arguments # init argument pointer
	
fibonacci_main_loop:
	li $v0, 4
	la $a0, fibonacci_pre_string
	syscall # Print initial string
	
	li $v0, 1
	lw $a0, ($t1)
	syscall # print argumemt to fib function
	
	li $v0, 4
	la $a0, fibonacci_mid_string
	syscall # Print middle string part
	
	lw $a0, ($t1) # set argument
	jal fibonacci # call fibonacci function
	
	move $a0, $v0 # move returned value to print
	li $v0, 1
	syscall # print returned value 
	
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	addiu $t1, $t1, 4 # increment argument pointer
	subiu $t0, $t0, 1 # decrement counter
	bnez $t0, fibonacci_main_loop # check if still in desirable range
	
fibonacci_main_exit:
	lw $a0, -8($fp) # Restore s0, a0, return address, and frame pointer
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 12 # Remove extra space from stack

	li $v0, 10
	syscall
	
fibonacci:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -12 # clear space for arguments and return address
	sw $ra, -4($fp)
	sw $a0, -8($fp)
	sw $s0, -12($fp)
	
	beqz $a0, fibonacci_base_0 # test for first base case
	beq $a0, 1, fibonacci_base_1 # test for second base case
	
	lw $a0, -8($fp) # f(n - 1) referenced from frame
	addi $a0, $a0, -1 
	jal fibonacci # call fucntion recursively
	move $s0, $v0 # load accumulator with inital value
	
	lw $a0, -8($fp) # f(n - 2) referenced from frame
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
	lw $s0, -12($fp) # Restore s0, a0, return address, and frame pointer
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 16 # Remove extra space from stack
	
	jr $ra
