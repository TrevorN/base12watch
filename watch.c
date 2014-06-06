#include <avr/io.h> //standard include for ATMega16
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#define sbi(x,y) x |= _BV(y) //set bit - using bitwise OR operator 
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator
#define is_high(x,y) (x & _BV(y) == _BV(y)) //check if the y'th bit of register 'x' is high ... test if its AND with 1 is 1

#include "display.h"
#include "easytwi.h"

int main(void)
{
	TWIStart(); //Create a start condition.
	TWIWrite(0b11010000); //Slave address plus write.
	TWIWrite(0x00); //Set the pointer
	TWIWrite(0x00);	//Seconds
	TWIWrite(0x00); //Minutes
	TWIWrite(0x52); //Hours
	//TWIWrite(0x01); //Day
	//TWIWrite(0x15); //Date
	//TWIWrite(0x12);	//Month
	//TWIWrite(0x13); //Year
	TWIStop(); //Send a stop bit.

	uint8_t seconds = 0;
	uint8_t minutes = 0;
	uint8_t hours = 0;
	uint8_t day = 0;
	while(true)
	{
		TWIStart();
		TWIWrite(0b11010000); //Slave address plus write.
		TWIWrite(0x01);	//Location of minutes.
		TWIStart();
		TWIWrite(0b11010001); //Slave address plus read.
		//seconds = TWIRead();
		//seconds = (seconds>>4)*10 + (seconds & 0x0F);
		minutes = TWIReadACK();
		minutes = (minutes>>4)*10 + (minutes & 0x0F);
		hours = TWIRead();
		hours = ((hours & 0x10)>>4)*10 + (hours & 0x0F);
        TWIStop();

		//printNum(day, UCHAR_MAX);
		//printNum(day, UCHAR_MAX);
		//printNum(hours, UCHAR_MAX);
		//printNum(hours, UCHAR_MAX);
		//printNum(minutes, UCHAR_MAX);
		//printNum(minutes, UCHAR_MAX);
		for(int i = 0; i < UCHAR_MAX; i++)
        {
           showDigit(0, hours);
           showDigit(1, minutes / 5);
        }
	}
}
