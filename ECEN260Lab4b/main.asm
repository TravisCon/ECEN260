;----------------------------------------------------------------------
; ECEN260 Lab 4A
;----------------------------------------------------------------------
		.cdecls C,LIST,"msp430.h" ; Include device header file
;----------------------------------------------------------------------
		.def RESET
			; Export program entry-point
 			; to make it known to linker.
;----------------------------------------------------------------------
		.text
	; Assemble into program memory
		.retain ; Override ELF conditional linking
 ; and retain current section
		.retainrefs ; Additionally retain any sections
 ; that have references to current
; section
;----------------------------------------------------------------------
; Allocate Global Memory
;----------------------------------------------------------------------
			.bss data1, 1
			.bss data2, 1
;----------------------------------------------------------------------
RESET		mov.w #__STACK_END,SP ; Initialize stack pointer
StopWDT 	mov.w #WDTPW|WDTHOLD,&WDTCTL ; Stop watchdog timer
;----------------------------------------------------------------------
; Main loop
;----------------------------------------------------------------------
mainLoop
			mov	  #0xF000, &data1 ;Set a beginning value

			bis   #0x000F, &data1 ;Set the very last bit
			bic   #0xE000, &data1 ;Clear the first 3 bits
			xor   #0x01C0, &data1 ;use XOR to invert bits 7-9

endProgram 	jmp endProgram
;----------------------------------------------------------------------
; Stack Pointer definition
;----------------------------------------------------------------------
 .global __STACK_END
 .sect .stack
;----------------------------------------------------------------------
; Interrupt Vectors
;----------------------------------------------------------------------
 .sect ".reset" ; MSP430 RESET Vector
 .short RESET
