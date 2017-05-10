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
	lw $a0, ($s2) # 
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
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -12 # clear space for arguments and return
	sw $ra, -4($fp)
	sw $a0, -8($fp)
	sw $a1, -12($fp)
	
	li $t0, 8 # loop counter over number of bytes
	move $t1, $a0 # move args into temporaries for mangling
	move $t2, $a1
	
bintohex_main_loop:
	and $t3, $t1, 0x0F # Get lowest 4 bits
	and $t4, $t4, $zero # Clear register
	
	blt $t3, 0xA, bintohex_num_digit # Check if the digit is numeric
	j bintohex_alpha_digit # Else is it alpha
	
bintohex_num_digit:
	add $t4, $t4, 0x30 # Add numeric character baseline
	j bintohex_main_loop_rest
bintohex_alpha_digit:
	add $t4, $t4, 0x41 # Add alpha character baseline
	sub $t3, $t3, 0x0A # Subtract 10 to get correct offset

bintohex_main_loop_rest:	
	add $t4, $t4, $t3 # Get total character code from sum
	sb $t4, ($t2) # Store the byte
	
	srl $t1, $t1, 4 # Shift down by 4 bits to examine those
	addiu $t2, $t2, 1 # Inc storage buffer address
	subiu $t0, $t0, 1 # Dec loop counter 
	
	bnez $t0, bintohex_main_loop # GO back to top
	
bintohex_return:
	sb $zero, 8($a1)

	lw $a1, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 16
	
	jr $ra
