.text
	.arch msp430g2553
	.p2align 2

	.global switch_interrupt_handler
	.extern switch_update_interrupt_sense
	.extern switches

switch_interrupt_handler:
	call #switch_update_interrupt_sense
	xor.b #255, r12 
	and.b #15, r12
	mov.b r12, &switches
	pop r0

