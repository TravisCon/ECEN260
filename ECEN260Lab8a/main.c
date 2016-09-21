//******************************************************************************
//  MSP430G2553 Demo - Color Display Communications - Bit Bang Mode - AdaFruit Display 2.8in 320x240pixels
//	LCD_RST  = P2.0 (RST on display board)
//	LCD_CS   = P2.1 (CS on display board)
//	LCD_DC	 = P2.2 (D/C on display board)
//	LCD_SCK  = P1.5 (SCK on display board)
//	LCD_MOSI = P1.7 (MOSI on display board)
//  VCC = Vin on display board
//	GND = GND on display board
//
//******************************************************************************
#include "msp430g2553.h"

// Pin Definitions
#define LCD_MOSI	0x0080		// data out at P1.7
#define LCD_SCK		0x0020		// serial clk at P1.5
#define LCD_RST 	0x0001		// reset at P2.0
#define LCD_CS		0x0002		// chip select at P2.1
#define LCD_DC		0x0004		// Data/Cmd at P2.2
#define BUTTON		0x0007		// Input buttons

#define TEXTX		175//Text - X location
// Color Definitions
#define RED			0xF800
#define GRN			0x07E0
#define BLU 		0x001F
#define BLK			0x0000
#define CYAN 		0x07FF
#define MAGENTA 	0xF81F
#define YELLOW 		0xFFE0
#define WHITE 		0xFFFF
#define DARKGREEN   0x7AAF

char H[8] = {0xCC, 0xCC, 0xCC, 0xFC,
			 0xFC, 0xCC, 0xCC, 0xCC};
char E[8] = {0xF8, 0xF8, 0xC0, 0xF8,
			 0xF8, 0xC0, 0xF8, 0xF8};
char L[8] = {0xC0, 0xC0, 0xC0, 0xC0,
			 0xC0, 0xC0, 0xF8, 0xF8};
char O[8] = {0x38, 0x6C, 0xC6, 0xC6,
			 0xC6, 0xC6, 0x6C, 0x38};
char B[8] = {0xF8, 0xCC, 0xCC, 0xF8,
			 0xF8, 0xCC, 0xCC, 0xF8};
char R[8] = {0xFC, 0xC6, 0xC6, 0xFC,
			 0xF0, 0xD8, 0xCC, 0xC6};
char K[8] = {0xC4, 0xCC, 0xD8, 0xF0,
			 0xF0, 0xD8, 0xCC, 0xC4};
char A[8] = {0x10, 0x38, 0x38, 0x28,
			 0x6C, 0x7C, 0xC6, 0xC6};

//space for function to still read in data
char space [8] = {0x00, 0x00, 0x00, 0x00,
				  0x00, 0x00, 0x00, 0x00};

void clearScreen(void);
void drawPixel(int, int, int);
void writeLCDData(char);
void writeLCDControl(char);
void initClk(void);
void rectangle(int x, int y, int color, int width, int height);
void circle(unsigned int x0, unsigned int y0,
        unsigned int radius, int color);
void drawLetter(int x, int y, char * letter,int color);

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer

	P1DIR = LCD_MOSI | LCD_SCK & ~BUTTON; 		// P1.2, P1.4 output, else input
	P2DIR = LCD_RST | LCD_CS | LCD_DC;

    // Initialize LCD
    P1OUT = 0x00;						// Set all outputs low for initialization
    P2OUT = LCD_RST | LCD_CS | LCD_DC;

	writeLCDControl(0xEF);				//
	writeLCDData(0x03);
	writeLCDData(0x80);
	writeLCDData(0x02);

	writeLCDControl(0xCF);				// Power Control B
	writeLCDData(0x00);
	writeLCDData(0xC1);
	writeLCDData(0x30);

	writeLCDControl(0xED);				// Power On Sequence Control
	writeLCDData(0x64);
	writeLCDData(0x03);
	writeLCDData(0x12);
	writeLCDData(0x81);

	writeLCDControl(0xE8);				// Driver Timing Control A
	writeLCDData(0x85);
	writeLCDData(0x00);
	writeLCDData(0x78);

	writeLCDControl(0xCB);				// Power Control A
	writeLCDData(0x39);
	writeLCDData(0x2C);
	writeLCDData(0x00);
	writeLCDData(0x34);
	writeLCDData(0x02);

	writeLCDControl(0xF7);				// Pump Ration Control
	writeLCDData(0x20);

	writeLCDControl(0xEA);				// Driver Timing Control A
	writeLCDData(0x00);
	writeLCDData(0x00);

	writeLCDControl(0xC0); 				// Power Control 1
	writeLCDData(0x23);

	writeLCDControl(0xC1);    			// Power Control 2
	writeLCDData(0x10);

	writeLCDControl(0xC5);    			// VCOM Control
	writeLCDData(0x3e);
	writeLCDData(0x28);

	writeLCDControl(0xC7);    			// VCOM Control2
	writeLCDData(0x86);

	writeLCDControl(0x36);    			// Memory Access Control
	writeLCDData(0x48);

	writeLCDControl(0x3A);				// COLMOD Pixel Format Set
	writeLCDData(0x55);

	writeLCDControl(0xB1);				// Frame Rate Control
	writeLCDData(0x00);
	writeLCDData(0x18);

	writeLCDControl(0xB6);    			// Display Function Control
	writeLCDData(0x08);
	writeLCDData(0x82);
	writeLCDData(0x27);

	writeLCDControl(0xF2);    			// 3Gamma Control
	writeLCDData(0x00);

	writeLCDControl(0x26);    			// Gamma curve selected
	writeLCDData(0x01);

	writeLCDControl(0xE0);   			// Positive Gamma Correction
	writeLCDData(0x0F);
	writeLCDData(0x31);
	writeLCDData(0x2B);
	writeLCDData(0x0C);
	writeLCDData(0x0E);
	writeLCDData(0x08);
	writeLCDData(0x4E);
	writeLCDData(0xF1);
	writeLCDData(0x37);
	writeLCDData(0x07);
	writeLCDData(0x10);
	writeLCDData(0x03);
	writeLCDData(0x0E);
	writeLCDData(0x09);
	writeLCDData(0x00);

	writeLCDControl(0xE1);    			// Negative Gamma Correction
	writeLCDData(0x00);
	writeLCDData(0x0E);
	writeLCDData(0x14);
	writeLCDData(0x03);
	writeLCDData(0x11);
	writeLCDData(0x07);
	writeLCDData(0x31);
	writeLCDData(0xC1);
	writeLCDData(0x48);
	writeLCDData(0x08);
	writeLCDData(0x0F);
	writeLCDData(0x0C);
	writeLCDData(0x31);
	writeLCDData(0x36);
	writeLCDData(0x0F);

	writeLCDControl(0x11);    			// Exit Sleep
	writeLCDControl(0x29);    			// Display on

	initClk();
	clearScreen();

	P1IE  =   BUTTON;   // interrupt enabled for buttons
	P1IES &= ~BUTTON;   // Low edge button trigger
	P1IFG &= ~BUTTON;   // IFG cleared for buttons
	_BIS_SR(GIE);   // enable Global Interrupt Flag

	while (1);
}

void drawLetter(int x, int y, char * letter, int color)
{
	//Countdown from 7 to 0
	int i = 7;
	for (i; i >= 0; i--)
	{
		//Assign value to bt
		char bt = letter[i];
		int j = 7;
		//Countdown from 7 to 0
		for (j; j >= 0; j--)
		{
			//Check MSB for match
			if (bt & 0x80)
				drawPixel(x+i, y+j, color);
			//Shift left
			bt = bt << 1;
		}
	}
}

void circle(unsigned int x0, unsigned int y0,
        unsigned int radius, int color)
{
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	drawPixel(x0, y0 + radius, color);
	drawPixel(x0, y0 - radius, color);
	drawPixel(x0 + radius, y0, color);
	drawPixel(x0 - radius, y0, color);

	while(x < y)
	{
	    if(f >= 0)
	    {
	       y--;
	       ddF_y += 2;
	       f += ddF_y;
	    }
	    x++;
	    ddF_x += 2;
	    f += ddF_x + 1;
	    drawPixel(x0 + x, y0 + y, color);
	    drawPixel(x0 - x, y0 + y, color);
	    drawPixel(x0 + x, y0 - y, color);
	    drawPixel(x0 - x, y0 - y, color);
	    drawPixel(x0 + y, y0 + x, color);
	    drawPixel(x0 - y, y0 + x, color);
	    drawPixel(x0 + y, y0 - x, color);
	    drawPixel(x0 - y, y0 - x, color);
	}
}

void rectangle(int x, int y, int color, int width, int height)
{
	unsigned int i;
	unsigned int j;
	//Nested FOR loop
	for (i = x + width; i > x ; i--)		// Loop through memory locations 2 pixels at a time - 16 bit color
	{
		//Draw pixel for every single row and column
		for (j = y + height; j > y; j--)
			drawPixel(i, j, color);
	}
}

void writeLCDControl(char data)
{
	P2OUT &= ~LCD_CS; //Clear Chip Select
	P2OUT &= ~LCD_DC; //Clear D/C

	//Loop through bits 0-7
	int i = 0;
	for (i = 7; i >= 0; i--)
	{
		//If bit 7 matches, output
		if (data & 0x80)
			P1OUT |= LCD_MOSI;
		else
			P1OUT &= ~LCD_MOSI;

		//Shift data
		data = data << 1;
		//Clock Pulse
		P1OUT |=  LCD_SCK;
		P1OUT &= ~LCD_SCK;
	}

	P1OUT &= ~LCD_MOSI;
	P2OUT |=  LCD_CS;
	P2OUT |=  LCD_DC;
}

void writeLCDData(char data)
{
	P2OUT &= ~LCD_CS;
	P2OUT |=  LCD_DC;

	int i = 0;
	for (i = 7; i >= 0; i--)
	{
		if (data & 0x80)
			P1OUT |= LCD_MOSI;
		else
			P1OUT &= ~LCD_MOSI;

		data = data << 1;
		P1OUT |=  LCD_SCK;
		P1OUT &= ~LCD_SCK;
	}

	P2OUT |=    LCD_CS;
	P1OUT &= ~LCD_MOSI;
}

void initClk(void)
{
   BCSCTL1 = CALBC1_16MHZ;
   DCOCTL = CALDCO_16MHZ;
}

void clearScreen(void)
{
	unsigned int i;

	writeLCDControl(0x2A);				// Select Column Address
	writeLCDData(0);					// Setup beginning column address
	writeLCDData(0);					// Setup beginning column address
	writeLCDData(0);					// Setup ending column address
	writeLCDData(239);					// Setup ending column address

	writeLCDControl(0x2B);				// Select Row Address
	writeLCDData(0);					// Setup beginning row address
	writeLCDData(0);					// Setup beginning row address
	writeLCDData(0x01);					// Setup ending row address
	writeLCDData(0x3F);					// Setup ending row address

	writeLCDControl(0x2C);				// Select Memory Write
	for (i = 240*161; i > 0 ; i--)		// Loop through memory locations 2 pixels at a time - 16 bit color
	{
		writeLCDData(0xff);				// Write pixel WHITE
		writeLCDData(0xff);
		writeLCDData(0xff);				// Write next pixel WHITE
		writeLCDData(0xff);
	}
}

void drawPixel(int x, int y, int color)
{
	writeLCDControl(0x2A);				// Select Column Address
	writeLCDData(x >> 8);				// Starting x Address (Most Sig 8 bits of address)
	writeLCDData(x & 0xff);				// Starting x Address (Least Sig 8 bits of address)
	writeLCDData(x >> 8);				// Ending x Address (Most Sig 8 bits of address)
	writeLCDData(x & 0xff);				// Ending x Address (Least Sig 8 bits of address)

	writeLCDControl(0x2B);				// Select Row Address
	writeLCDData(y >> 8);				// Starting y Address (Most Sig 8 bits of address)
	writeLCDData(y & 0xff);				// Starting y Address (Least Sig 8 bits of address)
	writeLCDData(y >> 8);				// Ending y Address (Most Sig 8 bits of address)
	writeLCDData(y & 0xff);				// Ending y Address (Least Sig 8 bits of address)

	writeLCDControl(0x2C);				// Select Color to write to the pixel
	writeLCDData(color >> 8);			// Send Most Significant 8 bits of 16 bit color
	writeLCDData(color & 0xff);			// Send Least Significant 8 bits of 16 bit color
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	//Reset Screen
	clearScreen();

	//If button 0 pressed, draw rectangle
	if (P1IFG & 0x01)
		rectangle(20, 25, RED, 50, 75);
	//If button 1 pressed, draw circle
	if (P1IFG & 0x02)
		circle(150, 200, 50, BLU);
	//If button 2 pressed, write "Hello Bro Karl"
	if (P1IFG & 0x04)
	{
		drawLetter(TEXTX, 200, H, BLK);
		drawLetter(TEXTX, 192, E, RED);
		drawLetter(TEXTX, 184, L, GRN);
		drawLetter(TEXTX, 176, L, BLU);
		drawLetter(TEXTX, 168, O, MAGENTA);
		drawLetter(TEXTX, 160, space, BLK);
		drawLetter(TEXTX, 152, B, CYAN);
		drawLetter(TEXTX, 144, R, BLK);
		drawLetter(TEXTX, 136, O, RED);
		drawLetter(TEXTX, 128, space, BLK);
		drawLetter(TEXTX, 120, K, GRN);
		drawLetter(TEXTX, 112, A, BLU);
		drawLetter(TEXTX, 104, R, MAGENTA);
		drawLetter(TEXTX,  96, L, DARKGREEN);
	}

	P1IFG &= ~BUTTON; //Flag cleared for buttons
	return;
}
