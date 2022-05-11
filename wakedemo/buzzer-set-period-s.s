.text
	.arch msp430g2553
	.p2align 2
	
	.global buzzer_set_period

buzzer_set_period:
	mov r12, CCR0
	mov r12, CCR1
	rrc r12
	pop r0
