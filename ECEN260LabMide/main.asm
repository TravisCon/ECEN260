;----------------------------------------------------------------------
; ECEN260 Midterm Part 5
;----------------------------------------------------------------------
 .cdecls C,LIST,"msp430g2553.h" ; cdecls tells assembler to
 ; allow the c header file
;---------------------------------------------------------------------
; Setup Main Section
;----------------------------------------------------------------------
 	.text ; program start
 	.global RESET ; define entry point
;---------------------------------------------------------------------
; Table 1 - Stored in Program Memory
;----------------------------------------------------------------------
			.label 	TABLE1_LOC_START
TABLE1 		.byte 	5,8,-29,25,33,1,-5,-32
TABLE1_ST 	.word 	TABLE1_LOC_START
;----------------------------------------------------------------------
; Table 2 - Stored in RAM memory (starting address = 0x0200)
;----------------------------------------------------------------------
	.bss TABLE2_ST, 8
;----------------------------------------------------------------------
; Main Code
;----------------------------------------------------------------------
RESET 	mov.w #0280h,SP
	  	mov.w #WDTPW+WDTHOLD,&WDTCTL ; stop watchdog timer

		mov &TABLE1_ST, R10
		mov	#0x0208, R7		;For comparison on line 46, 49
		mov #0x0200, R11	;Destination for copy
		mov #0x0008, R9 	;Count down from
							;R9 is the counter
		;First, Copy over to RAM
loop1	mov.b 	0(R10), 0(R11)
		inc		R10		;Increment R10 pointer
		inc 	R11		;Increment R11 pointer
		dec		R9		;Decrement Counter
		jnz		loop1	;If counter is zero, exit
;------Real Code Starts Here---------
		mov		#0x0200, R10 ;loop 2 starts at i = 0
loop2	mov 	#0x0200, R11 ;Loop 3 starts at j = 0
		mov		#0x0201, R12 ;This represents j+1
loop3	cmp.b	0(R11), 0(R12) ;Less than?
		jl		swap			;If less than, call SWAP
cont	inc		R11		;Increment j counter
		inc		R12 	;increment j++
		cmp		R7, R12 ;Are we beyond our bounds of 0x0208?
		jl		loop3	;If not finished, restart loop
		inc		R10		;Increment i counter
		cmp		R7, R10	;Has the i counter exceeded 0x0208?
		jl		loop2	;Exit loop if finished

endProgram jmp endProgram

swap	mov.b	0(R12), R13		;Copy to temporary
		mov.b	0(R11), 0(R12)	;Copy lower byte to new address
		mov.b	R13, 0(R11)		;Copy temp to lower byte
		jmp 	cont			;Bring it on home

;----------------------------------------------------------------------
; Interrupt Vectors
;----------------------------------------------------------------------
	.sect ".reset" ; MSP430 RESET Vector
 	.short RESET
 	.end
