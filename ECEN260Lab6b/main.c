#include <msp430.h> 

/*
 * main.c
 */
extern char readP1Data (void); // returns value of P1IN register
extern void writeP1Data(char data); // passes the char value to the P1OUT register
extern char readP1Direction (void); // returns value of P1DIR register
extern void writeP1Direction(char data); // passes the char value to the P1DIR register
extern char readP2Data (void); // returns value of P2IN register
extern void writeP2Data(char data); // passes the char value to the P2OUT register
extern char readP2Direction (void); // returns value of P2DIR register
extern void writeP2Direction(char data); // passes the char value to the P2DIR register

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    writeP2Data(0);      //Clear the output data
    writeP2Direction(7); //Bits 0-2 will be output
    writeP1Direction(6); //Bits 1 and 2 always mess up P1IN
    					 //I'm clearing them to make everything easier
    					 //For the IF statements

    int i = 0; //Initialize counter

    while(1) //Infinite loop
    {
    	if (readP1Data() == 7) //Enter loop if pressed
    	{
    		i++;  //Increment output for P2
    		writeP2Data(i); //Send newly incremented number to P2OUT
    		while (readP1Data() == 7)
    			; //Keep in loop until depressed
    	}
    }

	return 0;
}
