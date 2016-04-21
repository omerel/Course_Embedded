#include <p32xxxx.h>
#include <math.h>
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
void initPortB(void);
void print_led(int x);
void initPortD(void);
void initPortE(void);
void initPortF(void);
void initPortG(void);
void showAction(int x);
void performAction(int x);
void displayMsg(char strBottomMsg[]);
void busy(void);
float getDiff();
float getExternal();
float getSignal(O);
int checkSwitch();
void initPortsForDigital(void);
void enable();
int getKey();

void delay(void);

void main(void)
{
	int key_pressed;
	char strBottomMsg[16];
  	unsigned int portMap;
  	initPortB();
	initPortD();
	initPortE();
	initPortF();
	initPortG();
    while(1)
    {
		key_pressed = getKey();
	}         
}

int getKey()
{
	int key_pressed,i,keyVal,column, s8 = 0;
	char flag=0;
	int RUN_ZERO[4] = {0xee,0xdd,0xbb,0x77};

	column = 0;
	while(1)
	{
		PORTG = 0x00;
		PORTF = 0x07;
		PORTE = RUN_ZERO[column];
		delay();
		keyVal = PORTB & 0x0F;
		if(keyVal != 0x0f)
		{
			flag=1;
			break;
		}
		column++;
		if(column==4)
		column = 0;  
		
		// Check if switch 8 is ON
		if (checkSwitch()!=0)
		{
			performAction(key_pressed);
		}
	}							
	
	return((RUN_ZERO[column]&0xf0)|(keyVal));
}

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

void performAction(int x)
{
	unsigned int portMapD, portMapE, portMapF;
	int mode;
	float current;
	char	topmsg[32] = "", bottommsg[32] = "";

	portMapD = PORTD;
	portMapE = PORTE;
	portMapF = PORTF;

	initPortsForDigital();
	if ( x == 0xee ) // 1
	{
		current = getDiff();
		strcpy(bottommsg, "Analog 1 - ");
		strcat(bottommsg, ftoa(current));
		strcat(bottommsg, "V");
	}
	if ( x == 0xde ) // 2
	{
		current = getExternal();
		strcpy(bottommsg, "Analog 2 - ");
		strcat(bottommsg, ftoa(current));
		strcat(bottommsg, "V");
	}
	if ( x == 0xbe ) // 3
	{
		current = getSignals();
		mode = 3;
	}
	displayMsg(topmsg, bottommsg);

  	initPortB();
	initPortD();
	initPortE();
	initPortF();
	initPortG();

	PORTD = portMapD;
	PORTE = portMapE;
	PORTF = portMapF;
}

float getDiff()
{
}

float getExternal()
{
}

float getSignals()
{
}

void enable()
{
	PORTDbits.RD4 = 1;
	PORTDbits.RD4 = 0;
}
void delay(void)
{
	unsigned int i;
	for(i=0;i<6400;i++);
}

void initPortB(void)
{
	unsigned int portMap;

	// Handle LCD display
	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	portMap |= 0xF;
	TRISB = portMap;
	
	AD1PCFG |= 0x800f; //Select PORTB to be digital port input
	CNCONbits.ON = 0; //Change Notice Module On bit CN module is disabled
	CNPUE |=0x3C;  	//Set RB0 - RB3 as inputs with weak pull-up
	CNCONbits.ON = 1;// 1 = CN module is enabled
}

void initPortD(void)
{
	unsigned int portMap;
	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;
}

void initPortE(void)
{
	unsigned int portMap;
	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;
}

void initPortF(void)
{
	unsigned int portMap;
	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
}

void initPortG(void)
{
	unsigned int portMap;
	portMap = TRISG;
	portMap &= 0xFFFFFFFC;
	TRISG = portMap;
	PORTG = 0x00;
}

void displayMsg(char strBottomMsg[])
{
	char controlTop[6]={0x38,0x38,0x38,0xe,0x6,0x1};
	char controlBottom[1]={0xC0}; // Move to beginning of bottom line, align center
	int i = 0;

	// Display top message - control mode
	PORTBbits.RB15 = 0; // Control mode
	PORTDbits.RD5 = 0;	// Write mode
	PORTF = 0x00;
	for (i = 0; i < 6; i++)
	{
		PORTE = controlTop[i];
		PORTDbits.RD4=1;
		PORTDbits.RD4=0;
		busy();
	}

	// Display bottom message
	PORTBbits.RB15 = 0; // Control mode
	PORTDbits.RD5 = 0;	// Write mode
	PORTF = 0x00;
	PORTE = controlBottom[0];
	PORTDbits.RD4=1;
	PORTDbits.RD4=0;
	busy();

	PORTBbits.RB15 = 1; // Data mode
	PORTFbits.RF8 = 1;
	PORTDbits.RD5 = 0;//w/r
	PORTF = 0x00;
	for (i = 0; i < strlen(strBottomMsg); i++)
	{
		PORTE = strBottomMsg[i];
		PORTDbits.RD4=1;
		PORTDbits.RD4=0;
		busy();
	}
}

void busy(void)
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
		PORTDbits.RD4=1;//enable=1
		PORTDbits.RD4=0;//enable=0
	}
	while(PORTEbits.RE7);// BF
	PORTDbits.RD5=RD; 
	PORTBbits.RB15=RS;
	TRISE=STATUS_TRISE;  
}

void initPortsForDigital(void)
{
	unsigned int portMap;

	portMap = TRISD;
	portMap &= 0xFFFFFF4F;
	TRISD = portMap;
	PORTDbits.RD4 = 0;
	PORTDbits.RD5 = 0;

	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;

	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
}
