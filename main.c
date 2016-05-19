#include "p32xxxx.h"
#include <string.h>

// ================================================================================================
//											Math Quiz Game
// ================================================================================================
//
// Authors: Omer Elgrably 021590807 and Barr Inbar 039173232
// Mentor: Menachem Epstein
// Date: May 2016
// 
// ================================================================================================

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF

#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

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

// Procedures Declaration
void initialize_ports();
char readChar();
void dispTopMsg(char msg[]);
void dispBottomMsg(char msg[]);
void falseAnswer(char answer);
void trueAnswer(char answer);
void delayWithTimer(void);
void delayWithLoop(int d);
void displayLeds();
void showLCD(int mode);
void start_buzzer(int mode);
void itoa(int n, char s[]);
void reverse(char s[]);
/*
* COPY
*/
void enable();

// Globals

char questions[NUM_OF_QUESTIONS][3] = {"5+2", "3+1", "6-5", "4+4"};
char answers[NUM_OF_QUESTIONS] = {'7', '4', '1', '8'};

/*
* COPY
*/
void enable()
{
	PORTDbits.RD4=1;
	PORTDbits.RD4=0;
}
void start_buzzer(int mode)
{
	if (mode == HAPPY)
	{
	}
	else
	{
	}


}
void initialize_ports()
{
}

char readChar()
{
	return '1';
}


/*
* COPY
*/
void busy()
{
	char RD,RS;
	unsigned int portMap;
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
		enable();
	}
	while(PORTEbits.RE7);// BF
	PORTDbits.RD5=RD; 
	PORTBbits.RB15=RS;
	TRISE=STATUS_TRISE;  
}


/*
* COPY
*/
void dispTopMsg(char msg[])
{
	char controlTop[6]={0x38,0x38,0x38,0xe,0x6,0x1};
	int i = 0;

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
* COPY
*/
void dispBottomMsg(char msg[])
{
	char controlBottom[1]={0xC0}; // Move to beginning of bottom line, align center
	int i = 0;

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
	PR1=0X9FFE;
	T1CONbits.ON=1;
	IFS0bits.T1IF=0;
	while(!IFS0bits.T1IF);
}

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
* COPY
*/
void showQuestion(char question[])
{
	// Variable definition
	int time = 30, i=0, correct = 0;
	char cGuess = ' ';
	char ascii[2];

	// Code Section

	dispTopMsg(question);
	delayWithTimer();
	itoa(time, ascii);
	dispBottomMsg(ascii);

	while (time > 0)
	{
		cGuess = readChar();// only one char!
		if (cGuess == answers[i])
		{
			trueAnswer(cGuess);
			correct = 1;
		}
		else
			break;
		delayWithTimer();
		itoa(time, ascii);
		dispBottomMsg(ascii);
		time--;
	}
	if (!correct)
		falseAnswer(cGuess);
}

void trueAnswer(char answer)
{	
	int time = 10;
	while (time > 0)
	{
		displayLeds();
		showLCD(HAPPY);
		dispTopMsg( answer );
		dispBottomMsg("True Answer!");
		start_buzzer(HAPPY);
	}
}

void falseAnswer(char answer)
{	
	int time = 0;
	while (time>0)
	{
		showLCD(SAD);
		dispTopMsg(answer);
		dispBottomMsg("False Answer!");
		start_buzzer(SAD);
	}
}

/* itoa:  convert n to characters in s */
 void itoa(int n, char s[])
 {
	int i, sign;
	sign = n;
	if (sign < 0)		/* record sign */
		n = -n;         /* make n positive */
	i = 0;
	do {						/* generate digits in reverse order */
         s[i++] = n % 10 + '0';	/* get next digit */
	} while ((n /= 10) > 0);	/* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

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
	TRIS_MAIN_ADDR_DECODER;
	MAIN_DECODER_CS = DISABLE;
	int i;
	displayLeds();

	while (1)
	{
		for (i =0; i < NUM_OF_QUESTIONS; i++)
		{
			initialize_ports();
			showQuestion(questions[i]);
		}
	}
	return (0);
}
