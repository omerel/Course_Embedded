#include "p32xxxx.h"
#include <string.h>

// ================================================================================================
//											Math Quiz Game
// ================================================================================================
//
// A fun and interactive game for teaching pre-schoolers solvnig basic math problems with single
// digits. The system presents a simple math question and the user replies using the keypad. The
// system then reacts with sounds and pictures.
//
// Thia projects is a Final Project in Embedded Menachem Epstein's course Computing Course in
// Afeka College Of Engineering.
// 
// The project uses Pic32 microchip with Real Ice
// 
// Authors: Omer Elgrably 021590807 and Barr Inbar 039173232
// Mentor: Menachem Epstein
// Date: May 2016
// 
// ================================================================================================

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

// Const definition
#define TRIS_MAIN_ADDR_DECODER {unsigned int portfMap = TRISF;portfMap &= 0xFFFFFEF8;TRISF=portfMap;}
#define MAIN_ADDR_DECODER(CBA) PORTF = CBA
#define MAIN_DECODER_CS _RF8
#define TRIS_DATA_OUT {unsigned int porteMap = TRISE; porteMap &= 0xFFFFFF00; TRISE = porteMap;}
#define TRIS_LED_CLK {unsigned int portdMap = TRISD; portdMap &= 0xFFFFFFEF; TRISD = portdMap;}
#define LED_CLK  _RD4
#define LED_EN  0x04
#define DISABLE 0x01
#define NUM_OF_QUESTIONS 4
#define HAPPY 0
#define SAD 1
#define ONN 1
#define OFF 0

// Procedures declaration
char readChar();
void dispTopMsg(char msg[]);
void dispBottomMsg(char msg[]);
void falseAnswer(char answer);
void correctAnswer(char answer);
void keyPadDelay();
void delayWithTimer(void);
void delayWithLoop(int d);
void displayLeds();
void showLCD(int mode);
void playSound(int mode);
void itoa(int n, char s[]);
void reverse(char s[]);
void enable();
void busy();
void initPortsForLCD();

// Globals
char questions[NUM_OF_QUESTIONS][3] = {"5+2", "3+1", "6-5", "4+4"};
char answers[NUM_OF_QUESTIONS] = {7, 4, 1, 8};

/*
* Initializes ports for the textual LCD
*/
void initPortsForLCD()
{
	unsigned int portMap;

	// Port B
	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	portMap |= 0xF;
	TRISB = portMap;
	
	AD1PCFG |= 0x800f; //Select PORTB to be digital port input
	CNCONbits.ON = 0; //Change Notice Module On bit CN module is disabled
	CNPUE |=0x3C;  	//Set RB0 - RB3 as inputs with weak pull-up
	CNCONbits.ON = 1;// 1 = CN module is enabled

	// Port D
	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;

	// Port E
	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;

	// Port F
	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
}

/*
* Initializes ports for the keypad
*/
void initPortsForKeypad()
{
	unsigned int portMap;

	// Port B
	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	portMap |= 0xF;
	TRISB = portMap;
	
	AD1PCFG |= 0x800f; //Select PORTB to be digital port input
	CNCONbits.ON = 0; //Change Notice Module On bit CN module is disabled
	CNPUE |=0x3C;  	//Set RB0 - RB3 as inputs with weak pull-up
	CNCONbits.ON = 1;// 1 = CN module is enabled

	// Port D
	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;

	// Port E
	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;

	// Port F
	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;

	// Port G
	portMap = TRISG;
	portMap &= 0xFFFFFFFC;
	TRISG = portMap;
	PORTG = 0x00;
}

/*
* Sends a signal to the card
*/
void enable()
{
	PORTDbits.RD4 = 1;
	PORTDbits.RD4 = 0;
}

/*
* Check if the textual LCD is ready for the next input or not
*/
void busy()
{
	unsigned int portMap;
	char RD,RS;
	int STATUS_TRISE;
	RD=PORTDbits.RD5;
	RS=PORTBbits.RB15;
	STATUS_TRISE=TRISE;
	PORTDbits.RD5 = 1;//w/r
	PORTBbits.RB15 = 0;//rs 
	portMap = TRISE;
	portMap |= 0x80;
	TRISE = portMap;
	do
	{
		PORTDbits.RD4=1;//enable=1
		PORTDbits.RD4=0;//enable=0
	}
	while(PORTEbits.RE7);// BF
	PORTDbits.RD5=RD; 
	PORTBbits.RB15=RS;
	TRISE=STATUS_TRISE;  
}

/*
* Plays a tune according to the given mode
*/
void playSound(int mode)
{
//	while (1)
	{
		if (mode == HAPPY)
		{
			TRISG = 0;
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(200000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(200000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(200000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(200000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(300000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(50000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(300000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(600000);
		}
		else
		{
			TRISG = 0;
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(200000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(200000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(250000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(300000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(300000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(100000);
			PORTGbits.RG15 = ONN;
			enable();
			delayWithLoop(1000000);
			PORTGbits.RG15 = OFF;
			enable();
			delayWithLoop(1000000);
		}
	}
}

/*
* Read the char of the number pressed on the key pad
*/
char readChar()
{
	// Variable definition
	int nKeyPressed = -1, keyVal;
	int RUN_ZERO[4] = {0xee,0xdd,0xbb,0x77};
	int column = 0;
	char flag=0;
	int time = 30;
	char ascii[2];

	// Code Section

	initPortsForKeypad();

	while (time > 0)
	{
		while(1)
		{
			PORTG = 0x00;
	        PORTF = 0x07;
			PORTE = RUN_ZERO[column];
	
	      	keyPadDelay();
			keyVal = PORTB & 0x0F;
			if(keyVal != 0x0f)
			{
				flag=1;
				break;
			}
	        column++;
	
			// Loop when reaches the last column
			if(column==4)
				column = 0;  
		}
		delayWithTimer();
		itoa(time, ascii);
		dispBottomMsg(ascii);
		time--;
		if (flag)
			break;
	}

	return(translateKey((RUN_ZERO[column]&0xf0)|keyVal));
}

/*
* Retrieves the numeric value of the key pressed. If not a number returns -1
*/
int translateKey(int nKey)
{
	int nResult = -1;
	// Check what key pad was pressed and match with char val
	switch (nKey)
	{
		case(0xd7):
			nResult = 0;
			break;
		case(0xee):
			nResult = 1;
			break;
		case(0xde ):
			nResult = 2;
			break;
		case(0xbe):
			nResult = 3;
			break;
		case(0xed):
			nResult = 4;
			break;
		case(0xdd):
			nResult = 5;
			break;
		case(0xbd):
			nResult = 6;
			break;
		case(0xeb):
			nResult = 7;
			break;
		case(0xdb):
			nResult = 8;
			break;
		case(0x77):
			nResult = 9;
			break;
		default:
			nResult = -1;
			break;
	}
	return(nResult);
}

/*
* Delay for use of key pad
*/
void keyPadDelay()
{
	unsigned int i;
	for(i=0;i<6400;i++);
}

/*
* Display the top message on the text LCD
*/
void dispTopMsg(char msg[])
{
	char controlTop[6]={0x38,0x38,0x38,0xe,0x6,0x1};
	int i = 0;
	unsigned int portMap;
	
	initPortsForLCD();

	// Display top message - control mode
	PORTBbits.RB15 = 0; // Control mode
	PORTDbits.RD5 = 0;	// Write mode
	PORTF = 0x00;
	for (i = 0; i < 6; i++)
	{
		PORTE = controlTop[i];
		enable();
		busy();
	}

	PORTBbits.RB15 = 1; // Data mode
	PORTFbits.RF8 = 1;
	PORTDbits.RD5 = 0;//w/r
	PORTF = 0x00;
	for (i = 0; i < strlen(msg); i++)
	{
		PORTE = msg[i];
		enable();
		busy();
	}
}

/*
* Display the bottom message on the text LCD
*/
void dispBottomMsg(char msg[])
{
	char controlBottom[1]={0xC0}; // Move to beginning of bottom line, align center
	int i = 0;
	
	initPortsForLCD();

	// Display bottom message
	PORTBbits.RB15 = 0; // Control mode
	PORTDbits.RD5 = 0;	// Write mode
	PORTF = 0x00;
	PORTE = controlBottom[0];
	enable();
	busy();

	PORTBbits.RB15 = 1; // Data mode
	PORTFbits.RF8 = 1;
	PORTDbits.RD5 = 0;//w/r
	PORTF = 0x00;
	for (i = 0; i < strlen(msg); i++)
	{
		PORTE = msg[i];
		enable();
		busy();
	}
}

void showLCD(int mode)
{
	mode =1;
}

/*
* Simple delay using nop loop
*/ 
void delayWithLoop (int d)
{
	int i;
	for(i = 0; i< d;i++);
}

void delayWithTimer (void)
{
	T1CONbits.ON=0;
	T1CONbits.TGATE=0;
	T1CONbits.TCS=0;//in clock
	T1CONbits.TCKPS0=1;
	T1CONbits.TCKPS1=1;
	T1CONbits.TSYNC=1;
	TMR1=0;
	PR1=0X9FFE;		// 1 sec
	T1CONbits.ON=1;
	IFS0bits.T1IF=0;
	while(!IFS0bits.T1IF);
}

/*
* Show blinging leds
*/
void displayLeds()
{
	TRIS_DATA_OUT
	TRIS_LED_CLK
	LED_CLK = 0x0;
	MAIN_ADDR_DECODER(LED_EN);
	 while(1)
	{
		PORTE = 0x55;
		LED_CLK = 0x01;
		LED_CLK = 0x00;
 		delayWithLoop(120000);
		PORTE = 0xAA;
		LED_CLK = 0x01;
		LED_CLK = 0x00;
		delayWithLoop(120000);
	}
}

/*
* Displays the given question and checks the user's answer
*/
void showQuestion(int i)
{
	// Variable definition
	int time = 30, correct = 0;
	char cGuess[2], ascii[2];
	int nGuess = -1;
	char currQuestion[3];

	// Code Section

	strcpy(currQuestion, questions[i]);
	dispTopMsg(currQuestion);
	delayWithTimer();
	itoa(time, ascii);
	dispBottomMsg(ascii);

	nGuess = readChar();// only one char!
	itoa(nGuess, cGuess);
	if (nGuess == answers[i])
		correctAnswer(cGuess);
	else
		falseAnswer(cGuess);
}

/*
* Reacts when answer is correct
*/
void correctAnswer(char answer)
{	
	displayLeds();
	showLCD(HAPPY);
	dispTopMsg( answer );
	dispBottomMsg("Correct Answer!");
	playSound(HAPPY);
}

/*
* Reacts when answer is incorrect
*/
void falseAnswer(char answer)
{	
	showLCD(SAD);
	dispTopMsg(answer);
	dispBottomMsg("False Answer!");
	playSound(SAD);
}

/*
* Checks if switch 8 is on
*/
int checkSwitch()
{
	int x = 0;
	unsigned int portMap1, portMap2, portMap3;

	portMap1 = TRISE;
	portMap2 = PORTF;
	portMap3 = PORTE;

	// Receive switches input
	TRISE=0xFFFFFFFF;
    PORTF=3;
    PORTDbits.RD4=1;
    PORTDbits.RD4=0; 
    x = PORTE;
	TRISE = portMap1;
	PORTF = portMap2;
	PORTE = portMap3;
	return(x&128);
}


/* 
* Convert n to characters in s
*/
void itoa(int n, char s[])
 {
	int i, sign;
	sign = n;
	if (sign < 0)		/* record sign */
		n = -n;         /* make n positive */
	i = 0;
	do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';	/* get next digit */
	} while ((n /= 10) > 0);	/* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

/*
* For use of itoa
*/
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

int main(void)
{
	// Variable definition
	int i;

	// Code Section

	// Initialize
/*	TRIS_MAIN_ADDR_DECODER;
	MAIN_DECODER_CS = DISABLE;

	while ( 1 )
	{
		for (i =0 ; i < NUM_OF_QUESTIONS; i++)
		{
			if (checkSwitch())
				showQuestion(i);
			else
				break;
		}
	}
*/
//	displayLeds();
	showQuestion(2);
//	playSound(SAD);
//	dispTopMsg("dasdasdasd");
//	dispBottomMsg("Hello");

	return (0);
}
