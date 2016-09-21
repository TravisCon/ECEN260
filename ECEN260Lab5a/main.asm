;----------------------------------------------------------------------
;   ECEN260 Lab 5B
;----------------------------------------------------------------------
            .cdecls C,LIST,"msp430g2553.h"  ; cdecls tells assembler to
                                            ; allow the c header file

            .text                           ; program start
            .global RESET		    		; define entry point

;----------------------------------------------------------------------
;  Allocate Global Memory
;----------------------------------------------------------------------
			.bss	data1, 2
;----------------------------------------------------------------------
;   Main Code
;----------------------------------------------------------------------
RESET       mov.w   #0280h,SP
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; stop watchdog timer

mainLoop	mov #0xAAAA, R7
			clr R9
			mov #0x0010, R8

shift		rla R7
			adc R9
			dec R8
			jnz shift

endProgram	jmp	endProgram

;----------------------------------------------------------------------
;   Interrupt Vectors
;----------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end
