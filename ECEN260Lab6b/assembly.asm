;AsmFile
; cdecls tells assembler to allow
; the c header file
	.cdecls C,LIST,"msp430.h"
; Setup Main Section
	.text ; program start
	.global readP1Data
	.global writeP1Data
	.global readP1Direction
	.global writeP1Direction
	.global readP2Data
	.global writeP2Data
	.global readP2Direction
	.global writeP2Direction

; Main Code
readP1Data:
	mov.b	&P1IN, R12	;Read P1 input pins
	ret

writeP1Data:
	mov.b #21h, R11 	;Address of P1DIR register
	mov.b R12, 0(R11)	; Move passed value to P1DIR register
	ret

readP1Direction:
	mov.b	&P1DIR, R12	;Read direction bits
	ret

writeP1Direction:
	mov.b #22h, R11		;Address of P1DIR register
	mov.b R12, 0(R11)	;Move passed value to P1DIR register
	ret

readP2Data:
	mov.b	&P2IN, R12	;Return P2 input
	ret

writeP2Data:
	mov.b #29h, R11		;Address of P2OUT register
	mov.b R12, 0(R11)	;Move passed value to P2OUT register
	ret

readP2Direction:
	mov.b	&P1DIR, R12	;read in P1DIR and return
	ret

writeP2Direction:
	mov.b #2Ah, R11		;Address of P2DIR register
	mov.b R12, 0(R11)	;Move passed value to P2DIR register
	ret

.end
