/* port_test.c
 * The following program just  keeps toggling pin 0 of port B
 */
 
#include <avr/io.h> //standard include for ATMega16
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <limits.h>
#define sbi(x,y) x |= _BV(y) //set bit - using bitwise OR operator 
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator
#define is_high(x,y) (x & _BV(y) == _BV(y)) //check if the y'th bit of register 'x' is high ... test if its AND with 1 is 1

#include "display.h"

uint8_t timeSetter[4] = {0x00, 0x00, 0x00, 0b00010010};
uint8_t tsIndex = 0;

typedef enum{false, true} bool;

bool write = true;

uint8_t inBuf[2];
uint8_t ibIndex = 0;

/*
ISR(TWI_vect)
{
	switch(TWSR & 0xF8)
	{
		//THIS IS ALL FOR WRITING
		case(0x08):
			//Just sent a start condition.
			//So lets send the address and write.
			TWDR = 0xD0;
			TWCR = 0xC5;
			break;
		case(0x10):
			//Just sent a repeated start condition.
			//So lets send the address and read.
			TWDR = 0xD1;
			TWCR = 0xC5;
			break;
		case(0x18):
			//Just sent SLA+W, and we got an ACK.
			if(write)
			{
				TWDR = 0x00;
				TWCR = 0xC5;
			} else {
				TWDR = 0x00;
				TWCR = 0xC5;
			}
			break;
		case(0x20):
			//Just sent SLA+W, and we got a NACK.
			while(true)
			{
				showDigit(0, 11);
				showDigit(1, 10);
			}
			break;
		case(0x28):
			//Sent data, got an ACK
			if(write)
			{
				if(tsIndex < 4)
				{
					//Keep sending our buffer.
					TWDR = timeSetter[tsIndex];
					tsIndex++;
					TWCR = 0xC5;
				} else {
					//Send a STOP, we done, yo.
					tsIndex = 0;
					TWCR = 0xD5;
				}
			} else {
				//Send a repeated start.
				TWCR = 0xE5;
			}
			break;
		case(0x30):
			//Sent data, got a NACK
			while(true)
			{
				showDigit(0, 11);
				showDigit(1, 10);
			}
			break;
		case(0x38):
			//Lost arbitration. FAT CHANCE LOL
			while(true)
			{
				showDigit(0, 11);
				showDigit(1, 10);
			}
			break;
		//THIS IS ALL FOR READING
		case(0x40):
			//SLA+R has been sent, got an ACK.
			TWCR = 0xC5;
			break;
		case(0x48):
			//SLA+R has been sent, got a NACK.
			while(true)
			{
				showDigit(0, 11);
				showDigit(1, 10);
			}
			break;
		case(0x50):
			//Data byte has been received, we sent an ACK!
			if(ibIndex < 2)
			{
				inBuf[ibIndex] = TWDR;
				ibIndex++;
				TWCR = 0xC5;
			} else {
				ibIndex = 0;
				TWCR = 0xD5;
			}
			break;
		case(0x58):
			//Data byte has been received, we sent a NACK!
			while(true)
			{
				showDigit(0, 11);
				showDigit(1, 10);
			}
			break;
	}
}
*/

uint8_t status = 0;

inline void TWIStart(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

inline void TWIStop(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

inline void TWIWrite(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

inline uint8_t TWIRead()
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

inline uint8_t TWIReadACK()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

int main(void)
{
	TWIStart(); //Create a start condition.
	TWIWrite(0b11010000); //Slave address plus write.
	TWIWrite(0x00); //Set the pointer
	TWIWrite(0x00);	//Seconds
	TWIWrite(0x15); //Minutes
	TWIWrite(0x47); //Hours
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
