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

mainLoop	bis.b	#06, &P1DIR ;P1IN bits 1 and 2 are buggy
								;so we force them to be outputs
			clr.b	&P1IN		;Clear input pins
			mov		#29h, R10 	;Initialize P2OUT
			mov		#0,	0(R10)	;Set P2Out to binary 0
			mov		#20h, R11	;Move address of P1IN to R11
			bis.b	#07, &P2DIR	;Set P2DIR to 0000 0111
			jmp		loop2		;Start the infinite loop

loop1		inc.b	0(R10)		;Increment P2OUT
loop2		tst.b	0(R11)		;Which flags does it trigger?
			jnz		wait		;Jump to "wait" loop
			jmp		loop2		;Keep in loop2 until P1IN is depressed

wait		tst.b 	0(R11)		;Is the trigger pressed?
			jz		loop1		;If pressed, go to increment
			jmp		wait		;Otherwise, stay in this loop

endProgram	jmp	endProgram

;----------------------------------------------------------------------
;   Interrupt Vectors
;----------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end
