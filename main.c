#include <stdio.h>
#include <stdlib.h>


// ================================================================================================
//											Math Quiz Game
//											==============
//
// Authors: Omer Elgrably and Barr Inbar
// Mentor: Menachem Epstein
// Date: May 2016
// 
// ================================================================================================

// Procedures Declaration
void initialize_ports();
void initQuestions();
void setTimer(int time);
char readChar();
void dispTopMsg(char msg[]);
void dispBottomMsg(char msg[]);
void falseAnswer();
void trueAnswer();

// Globals
const int NUM_OF_QUESTIONS = 4;
char questions[NUM_OF_QUESTIONS][3];
char answers[NUM_OF_QUESTIONS];

void main(int argc, char const *argv[])
{
	int i;
	while ( true )
	{
		for (i =0 ; i < NUM_OFQUESTIONS; i++)
		{
			initialize_ports();
			showQuestion(questions[i]);
		}
	}
	return 0;
}

void initQuestions()
{
	questions =  {"5+2", "3+1", "6-5", "4+4"};
	answers = {'7', '4', '1', '8'};
}

void showQuestion(char[] question)
{
	int time = 30;
	char cGuess = '';
	int correct = 0;
	dispTopMsg(question);
	setTimer(time);

	while (time > 0)
	{
		cGuess = readChar();// only one char!
		if (cGuess == answers[i]
		{
			trueAnswer(cGuess);
			correct = 1;
		}
		else
			break;

		dispBottomMsg(itoa(time));
		time--;
	}
	if (!correct)
		falseAnswer();
}

void trueAnswer(char answer)
{	
	int time = 10;
	setTimer(time);
	while (time > 0)
	{
		startLed()
		showLCD(HAPPY_SMILE)
		dispTopMsg( answer );
		dispBottomMsg("True Answer!");
		start_buzzer(HAPPY);
	}
}

void falseAnswer(char answer)
{	
	Long time = 0;
	initialaize Timer(time);
	while (time>0)
	{
		showLCD(SAD_SMILE)
		dispTopMsg(answer);
		dispBottomMsg("False Answer!");
	start_buzzer(SAD);
	}
}