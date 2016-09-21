;----------------------------------------------------------------------
;   ECEN260 Midterm Part 1
;----------------------------------------------------------------------
            .cdecls C,LIST,"msp430g2553.h"  ; cdecls tells assembler to
                                            ; allow the c header file

            .text                           ; program start
            .global RESET		    	  ; define entry point

;---------------------------------------------------------------------
;  Table 1 - Stored in Program Memory
;----------------------------------------------------------------------
			.label 		TABLE1_LOC_START
TABLE1		.byte		0xff,0xfe,0xfd,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,0xf6,0xf5,0xf4,0xf3,0xf2,0xf1,0xf0
TABLE1_ST	.word		TABLE1_LOC_START

;----------------------------------------------------------------------
;  Allocate Global Memory
;----------------------------------------------------------------------
			.bss	f2Address, 2

;----------------------------------------------------------------------
;   Main Code
;----------------------------------------------------------------------
RESET       mov.w   #0280h,SP
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; stop watchdog timer

			mov		&TABLE1_ST, R10
			mov.b		#0xf2, R11		;Load the number we're looking for
			mov.b		#0x10, R12		;Counts to 16 instead of 8
again		cmp.b		0(R10), R11		;Compare 0xf2 with address pointer.
										;If same, set Z flag.
			jz			found           ;If Z flag set, its found!
			inc			R10				;Next item in array
			dec			R12				;Decrement counter
			jnz			again           ;Jump if loop not finished
found		mov			R10, &f2Address 	;Move pointer address to 0x0200
			cmp			#0xC010, &f2Address ;Outside of table boundaries?
			jz			nfound				;If at 0x10, jump to nfound

endProgram	jmp			endProgram

nfound 		mov.b		#0x00, &f2Address ;Clear if not found
			jmp 		endProgram        ;End program once done

;----------------------------------------------------------------------
;   Interrupt Vectors
;----------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end
