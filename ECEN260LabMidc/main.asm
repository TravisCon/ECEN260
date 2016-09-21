;----------------------------------------------------------------------
;   ECEN260 Midterm Part 3
;----------------------------------------------------------------------
            .cdecls C,LIST,"msp430g2553.h"  ; cdecls tells assembler to
                                            ; allow the c header file

            .text                           ; program start
            .global RESET		    		; define entry point

;----------------------------------------------------------------------
;  Allocate Global Memory
;----------------------------------------------------------------------
			.bss	data1, 2
			.bss	data2, 2
			.bss	quotient, 2
			.bss	remainder, 2
;----------------------------------------------------------------------
;   Main Code
;----------------------------------------------------------------------
RESET       mov.w   #0280h,SP
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; stop watchdog timer

mainLoop	mov		#0xFC18, &data1	;Move dividend to RAM
			mov.b	#0x0B, &data2	;Move divisor to RAM
			mov		&data1, R6		;Move dividend to Reg6
			mov.b	&data2, R7		;Move dividend to Reg7
			clr		R9				;Clear Remainder

			call 	#division		;Call our function

			mov		R8, &quotient
			mov		R9, &remainder

endProgram	jmp		endProgram

division 	clr 	R8			;Count subtractions made
			clr 	R10 		;Counts negative operands
			cmp		#0x0000, R6 ;If negative, invert function
			jl		invert1		;Call invert1 if negative
res1		cmp.b 	#0x00, R7	;If negative, invert function
			jl		invert2		;Call invert2 if negative

repeat		sub		R7, R6		;Subtract contents from R6
			inc		R8			;Increment quotient counter
			cmp		R7, R6		;If R6 remains are more than R7 repeat
			jge		repeat		;repeat loop
			mov		R6, R9		;Once finished, move remainder into R9
			tst		R10			;Test custom "negative flag"
			jnz		invert3		;If there's an odd number of negatives,
res2		ret					; then invert the quotient

invert1		dec		R10			;Subtract 1 from custom flag
			inv		R6			;Flip bits of Dividend
			inc		R6			;Add 1 to finish 2's compliment
			jmp		res1		;Return to function

invert2		inc 	R10			;Add 1 to custom flag
			inv.b	R7			;Flip bits of dividend
			inc.b	R7			;add 1 for 2's compliment
			jmp		repeat		;Return to function

invert3		inv 	R8			;Invert quotient
			inc		R8			;Add 1 to quotient's inversion
			jmp		res2		;return
;----------------------------------------------------------------------
;   Interrupt Vectors
;----------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end
