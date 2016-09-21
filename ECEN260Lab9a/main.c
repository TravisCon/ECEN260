//******************************************************************************
//  MSP430G2553 Demo - 	AdaFruit Display 2.8in 320x240pixels
//						Color Display Touchscreen Communications
// 						SPI Mode
//
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
#define TS_XM		0x0001		// X- at P1.0 (Analog In)
#define TS_YP		0x0002		// Y+ at P1.1 (Analog In)
#define TS_XP		0x0004		// X+ at P1.2
#define TS_YM		0x0008		// Y- at P1.3

typedef struct TSPoint
{
	int x;
	int y;
	int z;
} TSPoint;

// Color Definitions
#define RED			0xF800
#define GRN			0x07E0
#define BLU 		0x001F
#define BLK			0x0000
#define CYAN 		0x07FF
#define MAGENTA 	0xF81F
#define YELLOW 		0xFFE0
#define WHITE 		0xFFFF

// Button X and Y values
#define YBEGIN		250
#define YEND		310
#define B1BEGIN		10
#define B1END		70
#define B2BEGIN		90
#define B2END		150
#define B3BEGIN		170
#define B3END		230

// font Definitions
	const char font[91][8] = { {0x3c, 0x66, 0x6e, 0x6e, 0x60, 0x62, 0x3c, 0x00},
	{0x00, 0x00, 0x3c, 0x06, 0x3e, 0x66, 0x3e, 0x00},
	{0x00, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x7c, 0x00},
	{0x00, 0x00, 0x3c, 0x60, 0x60, 0x60, 0x3c, 0x00},
	{0x00, 0x06, 0x06, 0x3e, 0x66, 0x66, 0x3e, 0x00},
	{0x00, 0x00, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00},
	{0x00, 0x0e, 0x18, 0x3e, 0x18, 0x18, 0x18, 0x00},
	{0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x7c},
	{0x00, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x66, 0x00},
	{0x00, 0x18, 0x00, 0x38, 0x18, 0x18, 0x3c, 0x00},
	{0x00, 0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0x3c},
	{0x00, 0x60, 0x60, 0x6c, 0x78, 0x6c, 0x66, 0x00},
	{0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00},
	{0x00, 0x00, 0x66, 0x7f, 0x7f, 0x6b, 0x63, 0x00},
	{0x00, 0x00, 0x7c, 0x66, 0x66, 0x66, 0x66, 0x00},
	{0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00},
	{0x00, 0x00, 0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60},
	{0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x06},
	{0x00, 0x00, 0x7c, 0x66, 0x60, 0x60, 0x60, 0x00},
	{0x00, 0x00, 0x3e, 0x60, 0x3c, 0x06, 0x7c, 0x00},
	{0x00, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x0e, 0x00},
	{0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3e, 0x00},
	{0x00, 0x00, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00},
	{0x00, 0x00, 0x63, 0x6b, 0x7f, 0x3e, 0x36, 0x00},
	{0x00, 0x00, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x00},
	{0x00, 0x00, 0x66, 0x66, 0x66, 0x3e, 0x0c, 0x78},
	{0x00, 0x00, 0x7e, 0x0c, 0x18, 0x30, 0x7e, 0x00},
	{0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00},
	{0x0c, 0x12, 0x30, 0x7c, 0x30, 0x62, 0xfc, 0x00},
	{0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0x00},
	{0x00, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18},
	{0x00, 0x10, 0x30, 0x7f, 0x7f, 0x30, 0x10, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00},
	{0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x66, 0x66, 0xff, 0x66, 0xff, 0x66, 0x66, 0x00},
	{0x18, 0x3e, 0x60, 0x3c, 0x06, 0x7c, 0x18, 0x00},
	{0x62, 0x66, 0x0c, 0x18, 0x30, 0x66, 0x46, 0x00},
	{0x3c, 0x66, 0x3c, 0x38, 0x67, 0x66, 0x3f, 0x00},
	{0x06, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x0c, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00},
	{0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00},
	{0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00},
	{0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30},
	{0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00},
	{0x00, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00},
	{0x3c, 0x66, 0x6e, 0x76, 0x66, 0x66, 0x3c, 0x00},
	{0x18, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7e, 0x00},
	{0x3c, 0x66, 0x06, 0x0c, 0x30, 0x60, 0x7e, 0x00},
	{0x3c, 0x66, 0x06, 0x1c, 0x06, 0x66, 0x3c, 0x00},
	{0x06, 0x0e, 0x1e, 0x66, 0x7f, 0x06, 0x06, 0x00},
	{0x7e, 0x60, 0x7c, 0x06, 0x06, 0x66, 0x3c, 0x00},
	{0x3c, 0x66, 0x60, 0x7c, 0x66, 0x66, 0x3c, 0x00},
	{0x7e, 0x66, 0x0c, 0x18, 0x18, 0x18, 0x18, 0x00},
	{0x3c, 0x66, 0x66, 0x3c, 0x66, 0x66, 0x3c, 0x00},
	{0x3c, 0x66, 0x66, 0x3e, 0x06, 0x66, 0x3c, 0x00},
	{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00},
	{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30},
	{0x0e, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0e, 0x00},
	{0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00},
	{0x70, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x70, 0x00},
	{0x3c, 0x66, 0x06, 0x0c, 0x18, 0x00, 0x18, 0x00},
	{0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00},
	{0x18, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00},
	{0x7c, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x7c, 0x00},
	{0x3c, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3c, 0x00},
	{0x78, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x78, 0x00},
	{0x7e, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7e, 0x00},
	{0x7e, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00},
	{0x3c, 0x66, 0x60, 0x6e, 0x66, 0x66, 0x3c, 0x00},
	{0x66, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00},
	{0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00},
	{0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x6c, 0x38, 0x00},
	{0x66, 0x6c, 0x78, 0x70, 0x78, 0x6c, 0x66, 0x00},
	{0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x00},
	{0x63, 0x77, 0x7f, 0x6b, 0x63, 0x63, 0x63, 0x00},
	{0x66, 0x76, 0x7e, 0x7e, 0x6e, 0x66, 0x66, 0x00},
	{0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00},
	{0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60, 0x60, 0x00},
	{0x3c, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x0e, 0x00},
	{0x7c, 0x66, 0x66, 0x7c, 0x78, 0x6c, 0x66, 0x00},
	{0x3c, 0x66, 0x60, 0x3c, 0x06, 0x66, 0x3c, 0x00},
	{0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00},
	{0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00},
	{0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00},
	{0x63, 0x63, 0x63, 0x6b, 0x7f, 0x77, 0x63, 0x00},
	{0x66, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x66, 0x00},
	{0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x00},
	{0x7e, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x7e, 0x00}};


// Function Prototypes
void fillScreen(int);
void drawPixel(int, int, int);
void writeLCDData(char);
void writeLCDControl(char);
void waitMS(unsigned int);
void initClk(void);
void initPins(void);
void initLCD(void);
void initUSCI(void);
TSPoint getTSPoint(void);
void drawLetter(const char[], int, int, int);
void writeWords(char[], int, int, int);
void iToC(char [], int);
long map(long, long, long, long, long);
void fillRectangle(int, int, int, int, int);
void fillCircle(int , int, int, int);
void fillCircleHelper(int, int, int, unsigned char, int, int);
void drawRect(int, int, int, int, int);
void drawFastVLine(int, int, int, int);
void drawFastHLine(int, int, int, int);
void writeLine(char [], int, int, int, char);
int rotateX(int, int);
int rotateY(int, int);

// used for writing text to the screen
volatile int startX = 0x0004;
volatile int startY = 0x0004;

void main(void)
{
	char cNumber[5];
	TSPoint p;

	WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer

	initClk();							// Init clock to 16 MHz
	initPins();							// Init Pin Functionality
  	initUSCI();							// Init USCI (SPI)
  	P2OUT &= ~LCD_RST;
  	waitMS(500);
  	P2OUT = LCD_RST;
  	initLCD();							// Init LCD Controller

  	//Nice black background for buttons, and white space
  	// in the center for display.
	fillScreen(BLK);
	fillRectangle(10, 230, 25, YBEGIN - 10, WHITE);

	//First Button
	fillRectangle(B1BEGIN, B1END, YBEGIN, YEND, BLU);
	startX = B1BEGIN + 7;
	startY = YBEGIN + 20;
	writeWords("Button", WHITE, 1, BLU);
	startX = B1BEGIN + 10;
	startY = YBEGIN + 35;
	writeWords(" ONE  ", WHITE, 1, BLU);

	//Second button
	fillRectangle(B2BEGIN, B2END, YBEGIN, YEND, BLU);
	startX = B2BEGIN + 7;
	startY = YBEGIN + 20;
	writeWords("Button", WHITE, 1, BLU);
	startX = B2BEGIN + 10;
	startY = YBEGIN + 35;
	writeWords(" TWO  ", WHITE, 1, BLU);

	//Button three
	fillRectangle(B3BEGIN, B3END, YBEGIN, YEND, BLU);
	startX = B3BEGIN + 7;
	startY = YBEGIN + 20;
	writeWords("Button", WHITE, 1, BLU);
	startX = B3BEGIN + 10;
	startY = YBEGIN + 35;
	writeWords("THREE ", WHITE, 1, BLU);
	startX = 10;
	startY = 5;

	//Write my name at the top of the frame,
	// because it looks professional
	writeWords("Travis Confer", WHITE, 2, BLK);

	//Polling loop for button presses
	while (1)
	{
		//Amount of delay for each loop through
		waitMS(200);
		p = getTSPoint(); 	//Get p.x, p.y, and p.z
		 	 	 	 	  	//and assign to p

		if (p.z > 20)		//If screen is pressed
		{
			//Convert p.x to range between 0-240
			int Xp = (p.x - 120) * 240 / 740;
			//Convert p.y to range between 0-320
			int Yp = (p.y -  90) * 320 / 800;

			//If button press is below the YBEGIN and above YEND
			// then we are in range of our buttons
			if (Yp > YBEGIN && Yp < YEND)
			{
				//Between 3 buttons
				if (Xp > B1BEGIN && Xp < B1END)
				{
					//Clear screen and draw circle
					fillRectangle(10, 230, 25, YBEGIN - 10, WHITE);
					fillCircle(50, 100, 20, RED);
				}
				else if (Xp > B2BEGIN && Xp < B2END)
				{
					//Clear screen and draw rectangle
					fillRectangle(10, 230, 25, YBEGIN - 10, WHITE);
					fillRectangle(120, 200, 40, 70, GRN);
				}
				else if (Xp > B3BEGIN && Xp < B3END)
				{
					//clear screen and write message
					fillRectangle(10, 230, 25, YBEGIN - 10, WHITE);
					startX = 100;
					startY = 100;
					writeWords("Hello", BLU, 2, WHITE);
					startX = 110;
					startY = 140;
					writeWords("Bro", CYAN, 2, WHITE);
					startX = 105;
					startY = 180;
					writeWords("Karl", RED, 2, WHITE);
				}
			}
		}
	}
}

void initClk(void)
{
   BCSCTL1 = CALBC1_16MHZ;				// 16 MHz Operation
   DCOCTL = CALDCO_16MHZ;
}

void initPins(void)
{

	P1DIR = LCD_MOSI | LCD_SCK | TS_XP | TS_YM;	// set out put pins for P1
	P2DIR = LCD_RST | LCD_CS | LCD_DC;			// set out put pins for P2

	P1SEL |= LCD_MOSI + LCD_SCK;
	P1SEL2 |= LCD_MOSI + LCD_SCK;

	ADC10CTL0 = 0x1010; // Sample/Hold time and ADC on
	ADC10AE0 = TS_XM | TS_YP; // P1.0,P1.1 ADC option select

	P2OUT = LCD_RST | LCD_CS | LCD_DC;
}

void fillScreen(int color)
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
	for (i = 240*161; i > 0 ; i--)		// Loop through all memory locations 16 bit color
	{
		writeLCDData(color >> 8);		// Write data to LCD memory
		writeLCDData(color & 0xff);		// Write data to LCD memory
		writeLCDData(color >> 8);		// Write data to LCD memory
		writeLCDData(color & 0xff);		// Write data to LCD memory
	}
}

void fillRectangle(int xStart, int xEnd, int yStart, int yEnd, int color)
{
	unsigned int i;
	writeLCDControl(0x2A);				// Select Row Address
	writeLCDData(xStart >> 8);			// Starting x Address (Most Sig 8 bits of address)
	writeLCDData(xStart & 0xff);		// Starting x Address (Least Sig 8 bits of address)
	writeLCDData(xEnd >> 8);			// Ending x Address (Most Sig 8 bits of address)
	writeLCDData(xEnd & 0xff);			// Ending x Address (Least Sig 8 bits of address)

	writeLCDControl(0x2B);				// Select Column Address
	writeLCDData(yStart >> 8);			// Starting y Address (Most Sig 8 bits of address)
	writeLCDData(yStart & 0xff);		// Starting y Address (Least Sig 8 bits of address)
	writeLCDData((yEnd) >> 8);			// Ending y Address (Most Sig 8 bits of address)
	writeLCDData((yEnd) & 0xff);		// Ending y Address (Least Sig 8 bits of address)

	writeLCDControl(0x2C);				// Select Memory Write
	for (i = (xEnd - xStart + 1)*(yEnd - yStart + 1); i > 0 ; i--)		// Loop through all memory locations 16 bit color
	{
		writeLCDData(color >> 8);			// Send Most Significant 8 bits of 16 bit color
		writeLCDData(color & 0xff);			// Send Least Significant 8 bits of 16 bit color
	}
}

void fillCircle(int x0, int y0, int r, int color)
{
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

void fillCircleHelper(int x0, int y0, int r, unsigned char cornername, int delta, int color)
{
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

void drawRect(int x, int y, int w, int h, int color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void drawFastVLine(int x, int y, int h, int color)
{
	fillRectangle(x, x, y, y+h-1, color);
}


void drawFastHLine(int x, int y, int w, int color)
{
  fillRectangle(x, x+w-1, y, y, color);
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

void writeLCDControl(char data)
{
	P2OUT &= ~LCD_DC;					// DC low
	P2OUT &= ~LCD_CS;					// CS Low

  	while(IFG2&UCB0TXIFG);				// TX buffer ready?
  	UCB0TXBUF=data;						// start transmission

    return;
}

void writeLCDData(char data)
{
	P2OUT |= LCD_DC;					// DC high
	P2OUT &= ~LCD_CS;					// CS Low

	while(IFG2&UCB0TXIFG);				// TX buffer ready?
  	UCB0TXBUF=data;						// start transmission

    return;
}

void waitMS(unsigned int m_sec)
{
	while(m_sec--)
		__delay_cycles(16000);			// 1000 for 1MHz
}

void initLCD(void)
{
    P1OUT = 0x00;						// Set all outputs low for initialization
    P2OUT = LCD_RST | LCD_CS | LCD_DC;

	P2OUT |= LCD_RST;					// Reset LCD
	waitMS(10);
	P2OUT &= ~LCD_RST;
	waitMS(10);
	P2OUT |= LCD_RST;
	waitMS(100);

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
}

void initUSCI(void)
{
  	UCB0CTL1|=UCSWRST;					// USCI in reset state
  	UCB0CTL0|=UCMST+UCSYNC+UCCKPH+UCMSB;// SPI Master, 8bit, MSB first, synchronous mode
  	UCB0CTL1|=UCSSEL_2;					// USCI CLK-SRC=SMCLK=~8MHz
  	UCB0CTL1&=~UCSWRST;					// USCI released for operation
  	IE2|=UCB0TXIE;						// enable TX interrupt
  	IFG2&=~UCB0TXIFG;

  	_EINT();							// enable interrupts
}

TSPoint getTSPoint(void)
{
	int x=0, y=0, z=0;
	int sample;
 	TSPoint location;

	//
	// Code to read the X position on the touchscreen
	//
	P1DIR &= ~TS_YP;		// Y+ set to input
	P1DIR &= ~TS_YM;		// Y- set to input
	P1DIR |= TS_XP;			// X+ set to output
	P1DIR |= TS_XM;			// X- set to output

	P1OUT |= TS_XP;			// X+ set to high
	P1OUT &= ~TS_XM;		// X- set to low

	ADC10CTL0 = 0x1010; 	// Sampling and conversion start
	ADC10CTL1 = 0x1000; 	// Input A1 into ADC
	ADC10AE0 = TS_YP;		// P1.1 ADC option select
	ADC10CTL0 |= 0x0003; 	// Sampling and conversion start
	while (ADC10CTL1 & 0x0001); // Wait for conversion to complete - ADC10BUSY?
	sample = ADC10MEM; 		// Read ADC value
	x = (1023 - sample);	// Set top left corner = 0

	//
	// Code to read the Y position on the touchscreen
	//
	P1DIR &= ~TS_XP;		// X+ set to input
	P1DIR &= ~TS_XM;		// X- set to input
	P1DIR |= TS_YP;			// Y+ set to output
	P1DIR |= TS_YM;			// Y- set to output

	P1OUT |= TS_YP;			// Y+ out set high
	P1OUT &= ~TS_YM;		// Y- out set low

	ADC10CTL0 = 0x1010; 	// Sampling and conversion start
	ADC10CTL1 = 0x0000; 	// Input A0(X-) into ADC
	ADC10AE0 = TS_XM;		// P1.0 ADC option select
	ADC10CTL0 |= 0x0003; 	// Sampling and conversion start
	while (ADC10CTL1 & 0x0001); // Wait for conversion to complete - ADC10BUSY?
	sample = ADC10MEM; 		// Read ADC value
	y = (1023 - sample);	// Set top left corner = 0

	//
	// Code to read the Z position on the touchscreen
	//
	P1DIR |= TS_YM;			// Y- set to output
	P1DIR &= ~TS_YP;		// Y+ set to input
	P1DIR &= ~TS_XM;		// X- set to input
	P1DIR |= TS_XP;			// X+ set to output

 	P1OUT |= TS_YM;			// Set Y- high
	P1OUT &= ~TS_XP;		// Set X+ low

	ADC10CTL0 = 0x1010; 	// Sampling and conversion start
 	ADC10CTL1 = 0x0000; 	// Input A0(X-) into ADC
	ADC10AE0 = TS_XM;		// P1.0 ADC option select
	ADC10CTL0 |= 0x0003; 	// Sampling and conversion start
 	while (ADC10CTL1 & 0x0001); // Wait for conversion to complete - ADC10BUSY?
 	int z1= ADC10MEM; 		// Read ADC value

	ADC10CTL0 = 0x1010; 	// Sampling and conversion start
 	ADC10CTL1 = 0x1000; 	// Input A1(Y+) into ADC
	ADC10AE0 = TS_YP;		// P1.1 ADC option select
 	ADC10CTL0 |= 0x0003; 	// Sampling and conversion start
 	while (ADC10CTL1 & 0x0001); // Wait for conversion to complete - ADC10BUSY?
 	int z2= ADC10MEM; 		// Read ADC value

	z = (1023-(z2-z1));

 	location.x = x;
 	location.y = y;
 	location.z = z;

 	return location;
}

void drawLetter(const char letter[], int color, int fSize, int hColor)
{

	int repeatX;
	int repeatY;
	char mask;
	unsigned int i;
	unsigned int j;
	writeLCDControl(0x2A);				// Select Row Address
	writeLCDData(startX >> 8);			// Starting x Address (Most Sig 8 bits of address)
	writeLCDData(startX & 0xff);		// Starting x Address (Least Sig 8 bits of address)
	startX += ((8 * fSize)-1);
	writeLCDData(startX >> 8);			// Ending x Address (Most Sig 8 bits of address)
	writeLCDData(startX & 0xff);		// Ending x Address (Least Sig 8 bits of address)

	writeLCDControl(0x2B);				// Select Column Address
	writeLCDData(startY >> 8);			// Starting y Address (Most Sig 8 bits of address)
	writeLCDData(startY & 0xff);		// Starting y Address (Least Sig 8 bits of address)
	writeLCDData((startY + ((8*fSize)-1)) >> 8);	// Ending y Address (Most Sig 8 bits of address)
	writeLCDData((startY + ((8*fSize)-1)) & 0xff);	// Ending y Address (Least Sig 8 bits of address)

	startX++;

	writeLCDControl(0x2C);				// Select Memory Write
	for (i = 0; i < 8; i++)				// Loop through all characters in array
	{
		repeatY = fSize;
		while(repeatY > 0)
		{
			repeatY--;
			mask = 0x80;
			for (j = 0; j < 8; j++)			// loop through all bits in character
			{
				repeatX = fSize;
				while (repeatX > 0)
				{
					repeatX--;
					if (letter[i] & mask)
					{
						writeLCDData(color >> 8);			// Send Most Significant 8 bits of 16 bit color
						writeLCDData(color & 0xff);			// Send Least Significant 8 bits of 16 bit color
					}
					else
					{
						writeLCDData(hColor >> 8);			// Send Most Significant 8 bits of 16 bit White
						writeLCDData(hColor & 0xff);		// Send least Significant 8 bits of 16 bit White
					}
				}
				mask = mask >> 1;
			}
		}
	}
}

void writeLine(char words[], int color, int fSize, int hColor, char align)
{
	unsigned int numLetters = 0;
	unsigned int i = 0;
	unsigned int offset;
	while (words[i] != '\0')
	{
		i++;
		numLetters++;
	}
	if ((4 + (numLetters * 8 * fSize) - 1) > 235)
	{
		startX = 4;
		writeWords("String too Long!", color, 1, hColor);
	}
	else
	{
		offset = 239 - ((numLetters * 8 * fSize));
		fillRectangle(0, 239, startY, startY + 8 * fSize, hColor);
		switch (align)
		{
		default:
			startX = 4;
			break;
		case 'C':
		case 'c':
			startX = offset / 2;
			break;
		case 'R':
		case 'r':
			startX = offset;
			break;
		}
		writeWords(words, color, fSize, hColor);
	}
}

void writeWords(char words[], int color, int fSize, int hColor)
{
	unsigned int numLetters = 0;
	unsigned int i = 0;
	unsigned int j = 0;

	while(words[i] != '\0')
	{
		numLetters = 0;
		while(words[j] != ' ' && words[j] != '\0')
		{
			j++;
			numLetters++;
		}

		if ((startX + (numLetters * 8 * fSize) - 1) > 235)
		{
			startX = (8*fSize);
			startY += (fSize + (8 * fSize));
			if (words[i] == ' ')
			{
				i++;
			}
		}

		while(i < j)
		{
			switch (words[i])
			{
			case 'a':
				drawLetter(font[1], color, fSize, hColor);
				break;
			case 'b':
				drawLetter(font[2], color, fSize, hColor);
				break;
			case 'c':
				drawLetter(font[3], color, fSize, hColor);
				break;
			case 'd':
				drawLetter(font[4], color, fSize, hColor);
				break;
			case 'e':
				drawLetter(font[5], color, fSize, hColor);
				break;
			case 'f':
				drawLetter(font[6], color, fSize, hColor);
				break;
			case 'g':
				drawLetter(font[7], color, fSize, hColor);
				break;
			case 'h':
				drawLetter(font[8], color, fSize, hColor);
				break;
			case 'i':
				drawLetter(font[9], color, fSize, hColor);
				break;
			case 'j':
				drawLetter(font[10], color, fSize, hColor);
				break;
			case 'k':
				drawLetter(font[11], color, fSize, hColor);
				break;
			case 'l':
				drawLetter(font[12], color, fSize, hColor);
				break;
			case 'm':
				drawLetter(font[13], color, fSize, hColor);
				break;
			case 'n':
				drawLetter(font[14], color, fSize, hColor);
				break;
			case 'o':
				drawLetter(font[15], color, fSize, hColor);
				break;
			case 'p':
				drawLetter(font[16], color, fSize, hColor);
				break;
			case 'q':
				drawLetter(font[17], color, fSize, hColor);
				break;
			case 'r':
				drawLetter(font[18], color, fSize, hColor);
				break;
			case 's':
				drawLetter(font[19], color, fSize, hColor);
				break;
			case 't':
				drawLetter(font[20], color, fSize, hColor);
				break;
			case 'u':
				drawLetter(font[21], color, fSize, hColor);
				break;
			case 'v':
				drawLetter(font[22], color, fSize, hColor);
				break;
			case 'w':
				drawLetter(font[23], color, fSize, hColor);
				break;
			case 'x':
				drawLetter(font[24], color, fSize, hColor);
				break;
			case 'y':
				drawLetter(font[25], color, fSize, hColor);
				break;
			case 'z':
				drawLetter(font[26], color, fSize, hColor);
				break;
			case '0':
				drawLetter(font[48], color, fSize, hColor);
				break;
			case '1':
				drawLetter(font[49], color, fSize, hColor);
				break;
			case '2':
				drawLetter(font[50], color, fSize, hColor);
				break;
			case '3':
				drawLetter(font[51], color, fSize, hColor);
				break;
			case '4':
				drawLetter(font[52], color, fSize, hColor);
				break;
			case '5':
				drawLetter(font[53], color, fSize, hColor);
				break;
			case '6':
				drawLetter(font[54], color, fSize, hColor);
				break;
			case '7':
				drawLetter(font[55], color, fSize, hColor);
				break;
			case '8':
				drawLetter(font[56], color, fSize, hColor);
				break;
			case '9':
				drawLetter(font[57], color, fSize, hColor);
				break;
			case 'A':
				drawLetter(font[65], color, fSize, hColor);
				break;
			case 'B':
				drawLetter(font[66], color, fSize, hColor);
				break;
			case 'C':
				drawLetter(font[67], color, fSize, hColor);
				break;
			case 'D':
				drawLetter(font[68], color, fSize, hColor);
				break;
			case 'E':
				drawLetter(font[69], color, fSize, hColor);
				break;
			case 'F':
				drawLetter(font[70], color, fSize, hColor);
				break;
			case 'G':
				drawLetter(font[71], color, fSize, hColor);
				break;
			case 'H':
				drawLetter(font[72], color, fSize, hColor);
				break;
			case 'I':
				drawLetter(font[73], color, fSize, hColor);
				break;
			case 'J':
				drawLetter(font[74], color, fSize, hColor);
				break;
			case 'K':
				drawLetter(font[75], color, fSize, hColor);
				break;
			case 'L':
				drawLetter(font[76], color, fSize, hColor);
				break;
			case 'M':
				drawLetter(font[77], color, fSize, hColor);
				break;
			case 'N':
				drawLetter(font[78], color, fSize, hColor);
				break;
			case 'O':
				drawLetter(font[79], color, fSize, hColor);
				break;
			case 'P':
				drawLetter(font[80], color, fSize, hColor);
				break;
			case 'Q':
				drawLetter(font[81], color, fSize, hColor);
				break;
			case 'R':
				drawLetter(font[82], color, fSize, hColor);
				break;
			case 'S':
				drawLetter(font[83], color, fSize, hColor);
				break;
			case 'T':
				drawLetter(font[84], color, fSize, hColor);
				break;
			case 'U':
				drawLetter(font[85], color, fSize, hColor);
				break;
			case 'V':
				drawLetter(font[86], color, fSize, hColor);
				break;
			case 'W':
				drawLetter(font[87], color, fSize, hColor);
				break;
			case 'X':
				drawLetter(font[88], color, fSize, hColor);
				break;
			case 'Y':
				drawLetter(font[89], color, fSize, hColor);
				break;
			case 'Z':
				drawLetter(font[90], color, fSize, hColor);
				break;
			case '!':
				drawLetter(font[33], color, fSize, hColor);
				break;
			case '?':
				drawLetter(font[63], color, fSize, hColor);
				break;
			case ' ':
				drawLetter(font[32], color, fSize, hColor);
				break;
			case '.':
				drawLetter(font[46], color, fSize, hColor);
				break;
			case '\"':
				drawLetter(font[34], color, fSize, hColor);
				break;
			}
			i++;
		}
		j++;
	}
}

void iToC(char cNumber[], int value)
{
	int remainder;
	unsigned char i;
	for (i = 0; i < 4; i++)
	{
		if (value == 0)
			cNumber[3 - i] = '0';
		remainder = (value % 10);
		cNumber[3 - i] = (char) (/*((int) '0')*/0x30 + remainder);
		value = value / 10;
	}
	cNumber[4] = '\0';
}

int rotateX(int x, int y)
{
	int x1;
	x1 = (int) (x *.7071068 - y * .7071068);
	return x1;
}

int rotateY(int x, int y)
{
	int y1;
	y1 = (int) (x *.7071068 + y * .7071068);
	return y1;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#pragma INTERRUPT (USCI)
#pragma vector=USCIAB0TX_VECTOR
void USCI(void)
{
	P2OUT|=LCD_CS;						// transmission done
	IFG2&=~UCB0TXIFG;					// clear TXIFG
}
