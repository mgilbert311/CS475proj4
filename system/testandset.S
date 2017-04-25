	.text
	.globl	test_and_set

/**
 * test_and_set: assume it's called with test_and_set(&lock);
 * @param lock pointer to a mutex_t variable
 *
 */
test_and_set:
	pushl	%ebx		# preserve contents of the %ebx register, which is about to be modified
	movl	8(%esp), %ebx	# Move the arguments from the stack pointer register to the register ebx
	movl	$1, %eax	# Set register eax to be 1  (lock is held)
	xchg	%eax, (%ebx)	# Exchange data in register eax with data in register ebx
	popl	%ebx		# Pop the data from register ebx off the stack
	ret			# Return from method
