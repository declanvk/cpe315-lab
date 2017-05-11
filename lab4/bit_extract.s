# Question 6 - Use of Shift instructions to Obtain Selected Bit Fields
.data
	bintohex_return_buffer: .space 12

	bit_extract_eq_str: .asciiz " => "
	bit_extract_a0_arguments: .word 0x6608C000, 0xC2008000, 0x19F70FFF, 0xC2008001
	bit_extract_num_tests: .word 4
.text

bit_extract_main:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -16 # clear space for arguments and return address
	sw $ra, -4($fp)
	sw $a0, -8($fp)
	sw $s0, -12($fp) 
	sw $s1, -16($fp)
	
	lw $s0, bit_extract_num_tests
	la $s1, bit_extract_a0_arguments
bit_extract_main_loop:
	lw $a0, 0($s1)
	la $a1, bintohex_return_buffer
	jal bintohex
	
	li $v0, 4
	la $a0, bintohex_return_buffer
	syscall
	
	li $v0, 4
	la $a0, bit_extract_eq_str
	syscall
	
	lw $a0, 0($s1)
	jal bit_extract
	
	move $a0, $v0
	la $a1, bintohex_return_buffer
	jal bintohex
	
	li $v0, 4
	la $a0, bintohex_return_buffer
	syscall
		
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	subiu $s0, $s0, 1
	addiu $s1, $s1, 4
	bnez $s0, bit_extract_main_loop
	
bit_extract_exit:
	lw $s1, -16($fp)
	lw $s0, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addiu $sp, $sp, 20
	
	li $v0, 10
	syscall

bit_extract:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -8 # clear space for arguments and return address
	sw $ra, -4($fp)
	sw $a0, -8($fp)
	
	andi $t0, $a0, 0x19F70FFF
	bnez $t0, bit_extract_invalid
	
	# Field formats            fff0 0nn0 0000 x000 yyyy 0000 0000 0000
	andi $t0, $a0, 0xE0000000 # fff0 0000 0000 0000 0000 0000 0000 0000
	andi $t1, $a0, 0x06000000 # 0000 0nn0 0000 0000 0000 0000 0000 0000
	andi $t2, $a0, 0x00080000 # 0000 0000 0000 x000 0000 0000 0000 0000
	andi $t3, $a0, 0x0000F000 # 0000 0000 0000 0000 yyyy 0000 0000 0000
	
	srl $t0, $t0, 25
	srl $t1, $t1, 25
	srl $t2, $t2, 11
	srl $t3, $t3, 0
	
	or $t4, $t0, $t1
	or $t5, $t2, $t3
	or $v0, $t4, $t5
	j bit_extract_return
	
bit_extract_invalid:
	li $v0, -1 # invalid return code

bit_extract_return:
	lw $a0 ,-8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addiu $sp, $sp, 12
	jr $ra

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
	
bintohex_byte_loop:
	and $t3, $t1, 0x0F # Get lowest 4 bits
	and $t4, $t4, $zero # Clear register
	
	blt $t3, 0xA, bintohex_num_digit # Check if the digit is numeric
	j bintohex_alpha_digit # Else is it alpha
	
bintohex_num_digit:
	add $t4, $t4, 0x30 # Add numeric character baseline
	j bintohex_byte_loop_rest
bintohex_alpha_digit:
	add $t4, $t4, 0x41 # Add alpha character baseline
	sub $t3, $t3, 0x0A # Subtract 10 to get correct offset

bintohex_byte_loop_rest:	
	add $t4, $t4, $t3 # Get total character code from sum
	
	add $t5, $t0, $t2 
	sub $t5, $t5, 1
	sb $t4, ($t5) # Store the byte
	
	srl $t1, $t1, 4 # Shift down by 4 bits to examine those
	subiu $t0, $t0, 1 # Dec loop counter 
	
	bnez $t0, bintohex_byte_loop # GO back to top
	
bintohex_return:
	sb $zero, 8($a1)

	lw $a1, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 16
	
	jr $ra
