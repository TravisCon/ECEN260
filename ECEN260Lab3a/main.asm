;---------------------------------------------------------------------
; Digital I/O example for the LaunchPad
; Read the status of built in push button - P1.3
; (Note that P1.3 is "1" when the push button is open
; and "0" when the button is closed)
; Red light if the button is not pushed - P1.0
; Green light if the button is pushed - P1.6
; Build with Code Composer Studio
;---------------------------------------------------------------------
 .cdecls C,LIST,"msp430g2553.h" ; cdecls tells assembler to
 ; allow the c header file
;---------------------------------------------------------------------
; Main Code
;---------------------------------------------------------------------
 .text ; program start
 .global _main ; define entry point
_main		mov.w #0280h,SP
 			mov.w #WDTPW+WDTHOLD,&WDTCTL
 			bis.b #01000001b,&P1DIR
 			bis.b #00001000b,&P1REN
 			bis.b #00001000b,&P1OUT
Mainloop	bit.b #00001000b,&P1IN
 			jc Off
;Swapping the bis.b with the bis.c
;gave the desired outcome
On			bis.b #00000001b,&P1OUT
 			bic.b #01000000b,&P1OUT
 			jmp Wait
;The same swap also happened down here
;below the "Off" set of codes
Off			bic.b #00000001b,&P1OUT
 			bis.b #01000000b,&P1OUT
Wait		mov.w #1834,R15
L1			dec.w R15
 			jnz L1
 			jmp Mainloop
;---------------------------------------------------------------------
; Interrupt Vectors
;---------------------------------------------------------------------
 .sect ".reset" ; MSP430 RESET Vector
 .short _main
 .end
