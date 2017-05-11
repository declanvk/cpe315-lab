# Question 4 -  Double fixed-point arithmetic add double_add(ahi, alo, bhi, blo)
.data
	bintohex_return_buffer: .space 16

	double_add_ahi_args: .word 0xFEDCBA98, 0x00000000, 0xFFFFFFFF, 0x0000000E
	double_add_alo_args: .word 0x76543210, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	double_add_bhi_args: .word 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000
	double_add_blo_args: .word 0x00000000, 0x00000000, 0x00000001, 0x00000001
	double_add_num_test: .word 4
	double_add_sum_stri: .asciiz " + "
	double_add_equal_st: .asciiz " = "
.text
double_add_main:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -32 # clear space for arguments and return
	sw $ra, -4($fp) # return address
	sw $a0, -8($fp) # ahi
	sw $a1, -12($fp) # alo
	sw $a2, -16($fp) # bhi
	sw $a3, -24($fp) # blo
	sw $s0, -28($fp) # arg array offset
	sw $s1, -32($fp) # test counter
	
	li $s0, 0
	lw $s1, double_add_num_test
double_add_main_loop:
	
	lw $a0, double_add_ahi_args + 0($s0)
	lw $a1, double_add_alo_args + 0($s0)
	jal print_double
	
	li $v0, 4
	la $a0, double_add_sum_stri
	syscall
	
	lw $a0, double_add_bhi_args + 0($s0)
	lw $a1, double_add_blo_args + 0($s0)
	jal print_double
	
	lw $a0, double_add_ahi_args + 0($s0)
	lw $a1, double_add_alo_args + 0($s0)
	lw $a2, double_add_bhi_args + 0($s0)
	lw $a3, double_add_blo_args + 0($s0)
	jal double_add
	
	move $t0, $v0
	move $t1, $v1
	
	li $v0, 4
	la $a0 double_add_equal_st
	syscall
	
	move $a0, $t0
	move $a1, $t1
	jal print_double
	
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	addiu $s0, $s0, 4
	subiu $s1, $s1, 1
	bnez $s1, double_add_main_loop

double_add_exit:
	lw $s1, -32($fp)
	lw $s0, -28($fp)
	lw $a3, -24($fp)
	lw $a2, -16($fp)
	lw $a1, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 36

	li $v0 10
	syscall

print_double:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -20 # clear space for arguments and return
	sw $ra, -4($fp) # return address
	sw $a0, -8($fp) # ahi
	sw $a1, -12($fp) # alo
	sw $s0, -16($fp)
	sw $s1, -20($fp)
	
	move $s0, $a0
	move $s1, $a1
	
	move $a0, $s0
	la $a1, bintohex_return_buffer
	jal bintohex
	
	li $v0, 4
	la $a0, bintohex_return_buffer
	syscall
	
	move $a0, $s1
	la $a1, bintohex_return_buffer
	jal bintohex
	
	li $v0, 4
	la $a0, bintohex_return_buffer
	syscall
	
	lw $s1, -20($fp)
	lw $s0, -16($fp)
	lw $a1, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 24
	
	jr $ra

double_add:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -20 # clear space for arguments and return
	sw $ra, -4($fp)
	sw $a0, -8($fp)
	sw $a1, -12($fp)
	sw $a2, -16($fp)
	sw $a3, -20($fp)
	
	addu $v1 $a1 $a3
	sltu $t0 $v1 $a1
	add $v0 $a0 $a2
	add $v0 $v0 $t0
	
	lw $a3, -20($fp)
	lw $a2, -16($fp)
	lw $a1, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 24
	
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
