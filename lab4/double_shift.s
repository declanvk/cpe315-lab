# Question 5 - Perform a Double Arithmetic Right Shift ($a0, $a1) by N bits ($a2)

.data
	bintohex_return_buffer: .space 16
	
	double_shift_ahi_args: .word 0xFEDCBA98, 0xAAAAAAAA, 0x0FFFFFFF, 0x00000000
	double_shift_alo_args: .word 0x76543210, 0xAAAAAAAA, 0xFFFFFFFF, 0x00000008
	double_shift_shamt_args: .word 4, 1, 8, 3
	double_shift_test_cases: .word 4
	double_shift_op_string: .asciiz " >> "
	double_shift_eq_string: .asciiz " = "
.text
double_shift_main:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -20 # clear space for arguments and return
	sw $ra, -4($fp) # return address
	sw $a0, -8($fp) # ahi
	sw $a1, -12($fp) # alo
	sw $a2, -16($fp) # shamt
	sw $s0, -20($fp) # arg array offset
	sw $s1, -24($fp) # test counter
	
	li $s0, 0
	lw $s1, double_shift_test_cases
	
double_shift_main_loop:
	lw $a0, double_shift_ahi_args + 0($s0)
	lw $a1, double_shift_alo_args + 0($s0)
	jal print_double
	
	li $v0, 4
	la $a0, double_shift_op_string
	syscall
	
	li $v0, 1
	lw $a0, double_shift_shamt_args + 0($s0)
	syscall
	
	lw $a0, double_shift_ahi_args + 0($s0)
	lw $a1, double_shift_alo_args + 0($s0)
	lw $a2, double_shift_shamt_args + 0($s0)
	jal double_shift
	
	move $t0, $v0
	move $t1, $v1
	
	li $v0, 4
	la $a0, double_shift_eq_string
	syscall
	
	move $a0, $t0
	move $a1, $t1
	jal print_double
	
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	addiu $s0, $s0, 4
	subiu $s1, $s1, 1
	bnez $s1, double_shift_main_loop
	
double_shift_exit:
	lw $s1, -24($fp)
	lw $s0, -20($fp)
	lw $a2, -16($fp)
	lw $a1, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 24

	li $v0 10
	syscall


double_shift:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -16 # clear space for arguments and return
	sw $ra, -4($fp) # return address
	sw $a0, -8($fp) # ahi
	sw $a1, -12($fp) # alo
	sw $a2, -16($fp) # shamt
	
	and $t0 $t0 0x00              #clear $t0
	and $t1 $t1 0x00              #clear $t1
	addi $t1 $t1 0x20             #put value 32 in $t1
	sub $t1 $t1 $a2               #make $t1 32-N
	sllv $t0 $a0 $t1              #put the left bits needed for $v1 in $t0
	srlv $v1 $a1 $a2              #do the low order bit shift
	or $v1 $v1 $t0                #or in the high order bit shift
	srav $v0 $a0 $a2              #do the high order bit shift
	
	lw $a2, -16($fp)
	lw $a1, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 20
	jr $ra                        #return

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

