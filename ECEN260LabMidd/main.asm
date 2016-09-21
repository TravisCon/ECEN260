;----------------------------------------------------------------------
;   ECEN260 Midterm Part 4
;----------------------------------------------------------------------
        .cdecls C,LIST,"msp430g2553.h"  ; cdecls tells assembler to
                                            ; allow the c header file

        .text                           ; program start
        .global RESET		    		; define entry point

;----------------------------------------------------------------------
;  Allocate Global Memory
;----------------------------------------------------------------------
		.bss	data1, 1
		.bss	data2, 1
		.bss	total, 2
;----------------------------------------------------------------------
;   Main Code
;----------------------------------------------------------------------
RESET		mov.w   #0280h,SP
        	mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; stop watchdog timer

mainLoop
			mov.b	#0x9B, &data1
			mov.b	#0x99, &data2
			mov.b	&data1, R6
			mov.b	&data2, R7

			call 	#multiply	;Call multiply function

			mov		R8, &total	;Move answer to RAM

endProgram	jmp	endProgram

multiply	clr 	R8			;Clear Product
			clr 	R10 		;Clear negative flag
			tst.b 	r6			;Check multiplicand if negative
			jn		invert1		;invert if so
res1		tst.b 	r7			;Check multiplier if negative
			jn		invert2		;invert if negative

			;Do While(R7 != 0) loop below
res2		mov		R7, R9		;Make copy for AND
			and		#01, R9		;AND to see if Even or Odd
			jnz		addProd		;If odd, go to ADD function
repeat		rla		R6			;Multiply by 2
			rra.b	R7			;Divide by 2 (forget remainder)
			mov.b	R7, R9		;Make copy for AND
			and.b	#0x01, R9	;AND to see if Even or Odd
			jnz		addProd		;If odd, go to ADD function
			tst		R7			;Check Multiplier
			jnz		repeat		;If its zero, we've finished
			tst		R10			;Test custom "negative flag"
			jnz		invert3		;If there's an odd number of negatives,
res3		ret					; then invert the product

addProd		add		R6, R8		;Add R6 to product sum
			jmp 	repeat		;return to loop

invert1		dec		R10			;Subtract 1 from custom flag
			inv.b	R6			;Flip bits of Multiplicand
			inc.b	R6			;Add 1 to finish 2's compliment
			jmp		res1		;Return to function

invert2		inc 	R10			;Add 1 to custom flag
			inv.b	R7			;Flip bits of Multiplier
			inc.b	R7			;add 1 for 2's compliment
			jmp		res2		;Return to function

invert3		inv 	R8			;Invert product
			inc		R8			;Add 1 to product's inversion
			jmp		res3		;return
;----------------------------------------------------------------------
;   Interrupt Vectors
;----------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end
