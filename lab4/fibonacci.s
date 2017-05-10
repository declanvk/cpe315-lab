# Question 3 -  Create a recursive function that computes the Fibonacci value

.data
	pre_string: .asciiz "f("
	mid_string: .asciiz ") = "
	max_fibonacci_num: .word 20
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
