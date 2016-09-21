;----------------------------------------------------------------------
;   ECEN260 Midterm Part 2
;----------------------------------------------------------------------
            .cdecls C,LIST,"msp430g2553.h"  ; cdecls tells assembler to
                                            ; allow the c header file

;---------------------------------------------------------------------
;   Setup Main Section
;----------------------------------------------------------------------
            .text                           ; program start
            .global RESET		    	  ; define entry point

;---------------------------------------------------------------------
;  Table 1 - Stored in Program Memory
;----------------------------------------------------------------------
			.label 	TABLE1_LOC_START
TABLE1		.byte		0xff,0xfe,0xfd,0xfc,0xfb,0xfa,0xf9,0xf8
TABLE1_ST	.word		TABLE1_LOC_START

;----------------------------------------------------------------------
;  Table 2 - Stored in RAM memory (starting address = 0x0200)
;----------------------------------------------------------------------
			.bss		TABLE2_ST, 8

;----------------------------------------------------------------------
;   Main Code
;----------------------------------------------------------------------
RESET		mov.w	#0280h,SP
            mov.w	#WDTPW+WDTHOLD,&WDTCTL  ; stop watchdog timer

			mov	&TABLE1_ST, R10	;Move table1 pointer to R10
			mov	#0x0200, R11	;Move table2 pointer to R11
			mov #0x0008, R9 	;Count down from 8
								;R9 is the counter

loop		mov.b 	0(R10), 0(R11)
			inc		R10		;Increment R10 pointer
			inc 	R11		;Increment R11 pointer
			dec		R9		;Decrement Counter
			jnz		loop	;If counter is zero, exit

endProgram	jmp	endProgram
;----------------------------------------------------------------------
;   Interrupt Vectors
;----------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end
