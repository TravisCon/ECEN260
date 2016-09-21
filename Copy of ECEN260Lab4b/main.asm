;----------------------------------------------------------------------
; ECEN260 Lab 4A
;----------------------------------------------------------------------
		.cdecls C,LIST,"msp430.h"	; Include device header file
;----------------------------------------------------------------------
		.def    RESET		; Export program entry-point
                             		; to make it known to linker.
;----------------------------------------------------------------------
		.text                   ; Assemble into program memory
		.retain                 ; Override ELF conditional linking
                                    ; and retain current section
		.retainrefs             ; Additionally retain any sections
                                    ; that have references to current
                                    ; section
;---------------------------------------------------------------------
;  Table 1 - Stored in Program Memory (i.e. Flash Memory Addr = 0xC000)
;----------------------------------------------------------------------
		.label 	TABLE1_LOC_START
TABLE1	.byte		0xff,0xfe,0xfd,0xfc,0xfb,0xfa,0xf9,0xf8
TABLE1_ST	.word		TABLE1_LOC_START

;----------------------------------------------------------------------
;  Table 2 - Stored in RAM memory (starting address = 0x0200)
;----------------------------------------------------------------------
			.bss		TABLE2_ST, 8

;----------------------------------------------------------------------
RESET		mov.w #__STACK_END,SP		; Initialize stack pointer
StopWDT     mov.w  #WDTPW|WDTHOLD,&WDTCTL	; Stop watchdog timer

;----------------------------------------------------------------------
; Main loop
;----------------------------------------------------------------------mainLoop	mov	&TABLE1_ST, R10
			mov	#0x0200, R11 ;Set write location to Reg11

            mov.b	0(R10), 0(R11) ;Write 0 to 0x0200
            mov.b	1(R10), 1(R11) ;Write 1 to 0x0201
            mov.b	2(R10), 2(R11) ;Write 2 to 0x0202
            mov.b	3(R10), 3(R11) ;Write 3 to 0x0203
            mov.b	4(R10), 4(R11) ;Write 4 to 0x0204
            mov.b	5(R10), 5(R11) ;Write 5 to 0x0205
            mov.b	6(R10), 6(R11) ;Write 6 to 0x0206
            mov.b	7(R10), 7(R11) ;Write 7 to 0x0207

mainLoop	jmp	mainLoop
;----------------------------------------------------------------------
;           Stack Pointer definition
;----------------------------------------------------------------------
            .global __STACK_END
            .sect 	.stack

;----------------------------------------------------------------------
;           Interrupt Vectors
;----------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
