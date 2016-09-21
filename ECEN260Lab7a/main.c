#include <msp430.h> 

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR = 0x00; // All input
    P2DIR = 0x07; // Bits 0-2 output
    P2OUT = 0x00; // Clear bits

    P2IE  = 0x08;   // P2.3 interrupt enabled
    P2IES = 0x08;   // P2.3 Hi/lo edge
    P2IFG &= ~0x08; // P2.3 IFG cleared

    P1IE  = 0x01;   // P1.0 interrupt enabled
    P1IES = 0x01;   // P1.0 Hi/lo edge
    P1IFG &= ~0x01; // P1.0 IFG cleared
    _BIS_SR(GIE);   // enable Global Interrupt Flag

	while(1)
		;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	P2OUT += 1;     //Increment Output

	P2IFG &= ~0x08; // P2.3 Int Flag cleared
	P1IFG &= ~0x01; // P1.0 Int Flag cleared
	return;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	P2OUT -= 1;     //Decrement Output

	P1IFG &= ~0x01; // P1.0 Int Flag cleared
	P2IFG &= ~0x08; // P2.3 Int Flag cleared
	return;
}
