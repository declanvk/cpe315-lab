.data
	main_bintohex_test_str: .asciiz "Bin to hex test:\n"
	main_fibonacci_test_str: .asciiz "Fibonacci test:\n"
	main_double_add_test_str: .asciiz "Double add test:\n"
	main_double_shift_test_str: .asciiz "Double arithmetic right shift test:\n"
	main_bit_extract_test_str: .asciiz "Bit extraction test:\n"
	
	bintohex_equal_text: .asciiz " = 0x"
	bintohex_return_buffer: .space 16
	bintohex_a0_arguments: .word 0x000000AA, 0x01234567, 0xFFFFFFFF, 0xFEDCBA98, 0xAA000000
	
	fibonacci_pre_string: .asciiz "f("
	fibonacci_mid_string: .asciiz ") = "
	fibonacci_a0_arguments: .word 0, 1, 5, 10, 20
	fibonacci_num_test: .word 5
	
	double_add_ahi_args: .word 0xFEDCBA98, 0x00000000, 0xFFFFFFFF, 0x0000000E
	double_add_alo_args: .word 0x76543210, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	double_add_bhi_args: .word 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000
	double_add_blo_args: .word 0x00000000, 0x00000000, 0x00000001, 0x00000001
	double_add_num_test: .word 4
	double_add_sum_stri: .asciiz " + "
	double_add_equal_st: .asciiz " = "
	
	double_shift_ahi_args: .word 0xFEDCBA98, 0xAAAAAAAA, 0x0FFFFFFF, 0x00000000
	double_shift_alo_args: .word 0x76543210, 0xAAAAAAAA, 0xFFFFFFFF, 0x00000008
	double_shift_shamt_args: .word 4, 1, 8, 3
	double_shift_test_cases: .word 4
	double_shift_op_string: .asciiz " >> "
	double_shift_eq_string: .asciiz " = "

	bit_extract_eq_str: .asciiz " => "
	bit_extract_a0_arguments: .word 0x6608C000, 0xC2008000, 0x19F70FFF, 0xC2008001
	bit_extract_num_tests: .word 4
.text

main:
	li $v0, 4
	la $a0, main_bintohex_test_str
	syscall
	
	jal bintohex_main
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	li $v0, 4
	la $a0, main_fibonacci_test_str
	syscall
	
	jal fibonacci_main
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	li $v0, 4
	la $a0, main_double_add_test_str
	syscall
	
	jal double_add_main
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	li $v0, 4
	la $a0, main_double_shift_test_str
	syscall
	
	jal double_shift_main
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	li $v0, 4
	la $a0, main_bit_extract_test_str
	syscall
	
	jal bit_extract_main
	
exit:	li $v0, 10
	syscall
	

bintohex_main:
	addiu $sp, $sp, -4 # clear space for frame pointer
	sw $fp, 0($sp) # save frame pointer
	move $fp, $sp # update frame pointer to new frame
	addiu $sp, $sp, -24 # clear space for arguments and return 
	sw $ra, -4($fp) 
	sw $a0, -8($fp) 
	sw $a1, -12($fp)
	sw $s0, -16($fp) 
	sw $s1, -20($fp) 
	sw $s2, -24($fp) 

	li $s0, 5 # loop limit
	li $s1, 0 # counter\
	la $s2, bintohex_a0_arguments # arg pointer
	
bintohex_main_loop:
	lw $a0, ($s2)
	li $v0, 1
	syscall
	
	la $a0, bintohex_equal_text # print middle text
	li $v0, 4
	syscall 
	
	lw $a0, ($s2)
	la $a1, bintohex_return_buffer
	jal bintohex
	
	la $a0, bintohex_return_buffer
	li $v0, 4
	syscall
	
	addi $a0, $0, 0xA # set newline
	li $v0, 11
	syscall # print newline character
	
	addiu $s1, $s1, 1
	addiu $s2, $s2, 4
	blt $s1, $s0, bintohex_main_loop
	
bintohex_main_exit:
	lw $s2, -24($fp)
	lw $s1, -20($fp)
	lw $s0, -16($fp)
	lw $a1, -12($fp)
	lw $a0, -8($fp)
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addi $sp $sp 28

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

	jr $ra
	
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

	jr $ra

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

	jr $ra


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
	
	jr $ra

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
