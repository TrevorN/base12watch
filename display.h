#include <avr/io.h> //standard include for ATMega16
#include <avr/cpufunc.h> 
#define sbi(x,y) x |= _BV(y) //set bit - using bitwise OR operator 
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator
#define is_high(x,y) (x & _BV(y) == _BV(y)) //check if the y'th bit of register 'x' is high ... test if its AND with 1 is 1

//The patterns associated with displaying certain digits.
uint_8t digits[] = {0b01111110, //zero
                    0b01100000, //one
                    0b01101101, //two
                    0b01111001, //three
                    0b00110011, //four
                    0b01011011, //five
                    0b01011111, //six
                    0b01110000, //seven
                    0b01111111, //eight
                    0b01111011};//nine

//Takes the platform agnostic version of the seven segment data, and transforms it so that it matches the pinout.
uint_8t transformDataToDisplay(uint_8t input)
{
    return ((0b01100000 & input) << 1) | (0b00011111 & input)
}

void showDigit(char segment, char num)
{
    DDRD &= 0b00100000;
    if(segment == 0)
    {
        cbi(DDRC, PC0);
        cbi(PORTC, PC0);
        sbi(DDRC, PC1);
        sbi(PORTC, PC1);
    } else {
        cbi(DDRC, PC1);
        cbi(PORTC, PC1);
        sbi(DDRC, PC0);
        sbi(PORTC, PC0);
    }
    
    DDRD = transformDataToDisplay(digits[num]);
}

void showNumber(char number)
{
    if(number < 100 && number => 0)
    {
        showDigit(0, number / 10);
        showDigit(1, number % 10);
    }
}

void printNum(unsigned char number, unsigned char duration)
{
	for(char j = 0; j < duration; j++)
	{
		showNumber(number);
	}
}

/*
 *void turnOnLed(char displayNumber, char index)
 *{
 *    displayPowerOff();
 *    
 *    if(displayNumber == 0)
 *    {
 *        sbi(DDRC, PC1);
 *        sbi(PORTC, PC1);
 *    } else {
 *        sbi(DDRC, PC0);
 *        sbi(PORTC, PC0);
 *    }
 *    
 *    switch (index)
 *    {
 *        case 0:
 *            sbi(DDRD, PD0); 
 *            break;
 *        case 1:
 *            sbi(DDRD, PD1);
 *            break;
 *        case 2:
 *            sbi(DDRD, PD3);
 *            break;
 *        case 3:
 *            sbi(DDRD, PD4);
 *            break;
 *        case 4:
 *            sbi(DDRD, PD5);
 *            break;
 *        case 5:
 *            sbi(DDRD, PD6);
 *            break;
 *        case 6:
 *            sbi(DDRD, PD7);
 *            break;
 *    }
 *    
 *}
 *
 *void turnOnLeds(char segment, char toON[], char length)
 *{
 *    for(unsigned char i = 0; i < length; i++)
 *    {
 *        turnOnLed(segment, toON[i]);
 *    }
 *}
 *
 *I seriously love the pre-processor.
 *#define turnOnLeds(segment, num) turnOnLeds(segment, num, sizeof num / sizeof *num)
 *
 *
 *void showDigit(char segment, char num)
 *{
 *    switch(num)
 *    {
 *        case 0:
 *        {
 *            char zero[] = {0, 1, 2, 3, 4, 5};
 *            turnOnLeds(segment, zero);
 *            break;
 *        }
 *        case 1:
 *        {
 *            char one[] = {1, 2};
 *            turnOnLeds(segment, one);
 *            break;
 *        }
 *        case 2:
 *        {
 *            char two[] = {0, 1, 3, 4, 6};
 *            turnOnLeds(segment, two);
 *            break;
 *        }
 *        case 3:
 *        {
 *            char three[] = {0, 1, 2, 3, 6};
 *            turnOnLeds(segment, three);
 *            break;
 *        }
 *        case 4:
 *        {
 *            char four[] = {1, 2, 5, 6};
 *            turnOnLeds(segment, four);
 *            break;
 *        }
 *        case 5:
 *        {
 *            char five[] = {0, 2, 3, 5, 6};
 *            turnOnLeds(segment, five);
 *            break;
 *        }
 *        case 6:
 *        {
 *            char six[] = {0, 2, 3, 4, 5, 6};
 *            turnOnLeds(segment, six);
 *            break;
 *        }
 *        case 7:
 *        {
 *            char seven[] = {0, 1, 2};
 *            turnOnLeds(segment, seven);
 *            break;
 *        }
 *        case 8:
 *        {
 *            char eight[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
 *            turnOnLeds(segment, eight);
 *            break;
 *        }
 *        case 9:
 *        {
 *            char nine[] = {0, 1, 2, 3, 5, 6, 7, 8};
 *            turnOnLeds(segment, nine);
 *            break;
 *        }
 *        case 10:
 *        {
 *            char a[] = {0, 1, 2, 4, 5, 6};
 *            turnOnLeds(segment, a);
 *            break;
 *        }
 *        case 11:
 *        {
 *            char b[] = {2, 3, 4, 5, 6};
 *            turnOnLeds(segment, b);
 *            break;
 *        }
 *        case 12:
 *        {
 *            char bigC[] = {0, 3, 4, 5};
 *            turnOnLeds(segment, bigC);
 *            break;
 *        }
 *        case 13:
 *        {
 *            char littleC[] = {3, 4, 6};
 *            turnOnLeds(segment, littleC);
 *            break;
 *        }
 *    }
 *}
 *
 *void showNum(uint8_t number)
 *{
 *    showDigit(0, number / 10);
 *    showDigit(1, number % 10);
 *}
 *
 */
