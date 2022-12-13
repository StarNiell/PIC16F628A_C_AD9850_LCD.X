/*
 * File:   main.c
 * Author: Aniello
 *
 * Created on 1 novembre 2022, 19.03
 */

#define _XTAL_FREQ 4000000

#define CLK_595 RB0    //595 > SH_CP > PIN 11
#define DATA_595 RB1   //595 > DS    > PIN 14
#define LATCH_595 RB2  //595 > ST_CP > PIN 12

#define DATA_DDS RB3    // DATA_DDS  = xxxx1xxx
#define W_CLK_DDS RB4   // W_CLK_DDS = xxx1xxxx
#define FQ_UD_DDS RB5   // FQ_UD_DDS = xx1xxxxx
#define RESET_DDS RB6   // RESET_DDS = x1xxxxxx

//#define PORT 0x378

#include <xc.h>
#include "lcd_595.h"
#include "stdlib.h"
#include <stdio.h>
#include <math.h>

// BEGIN CONFIG
// CONFIG
#pragma config FOSC = INTOSCIO // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

int counter = 0; // non serve
int i;
float clock_in;
float F_out;
unsigned int delta_phase;
    
void main() {
    TRISB = 0x00;
    lcdSetup();
    ddsSetup();
        
    while (1)
    {
        ddsLoop();
        lcdPrint();
    }
}

void lcdSetup()
{
    __delay_ms(100);
    LCD_595_init(20, 2);
    LCD_595_clear();
    
    //LCD_595_cmd(LCD_1LINE);
    LCD_595_setCursor(0, 0);
    LCD_595_print("ElettronicaMaker.it");
    //LCD_595_cmd(LCD_2LINE);
    LCD_595_setCursor(0, 1);
    LCD_595_print("IU8NQI ");    
    
    counter = 0;
}

void ddsSetup()
{
    //AD9850 XTAL Mhz
    clock_in = 30000000;
    F_out = 7000000;
}

void lcdPrint()
{
    counter++;
    RB0 = 1;

    char buffer [sizeof(int)*8+1];
    itoa(buffer,counter,10);
    LCD_595_setCursor(12, 1);
    LCD_595_print(buffer);       

    __delay_ms(1000);

    RB0 = 0;
    __delay_ms(1000);
}

void ddsLoop()
{
    /* calculate AD9850 32-bit tuning word */
    delta_phase = (F_out * pow(2, 32)) / clock_in;


    /* output bit stream to AD9850 */
    tuning_word_out(delta_phase);


    /* output control byte */
    byte_out(0x00);


    /* latch frequency data - pulse FQ_UD high */
    char lcd_part = PORTB & 0b00000111;
    //FQ_UD_DDS = 1;
    PORTB = 00100000 + lcd_part;
    //FQ_UD_DDS = 0;
    PORTB = 00000000 + lcd_part;
}


/* output byte, MSB first */
byte_out(byte)
unsigned char byte;
{
	int i;


	for (i = 0; i < 8; i++)
	{
		if ((byte & 1) == 1)
			one();
		else
			zero();
		byte = byte >> 1;
	}
}


/* output 32-bit tuning word, MSB first */
tuning_word_out(tuning_word)
unsigned int tuning_word;
{
	int i;


	for (i = 0; i < 32; i++)
	{
		if ((tuning_word & 1) == 1)
			one();
		else
			zero();
		tuning_word = tuning_word >> 1;
	}
}


/* output a '1' to AD9850 */
one()
{
    // DATA_DDS  = xxxx1xxx
    // W_CLK_DDS = xxx1xxxx
    // FQ_UD_DDS = xx1xxxxx
    // RESET_DDS = x1xxxxxx

    char lcd_part = PORTB & 0b00000111;
	//outportb(PORT, 1);/* CLOCK low and DATA high - 01 */
    PORTB = 0b00001000 + lcd_part; // xxx01xxx
	//outportb(PORT, 3);/* CLOCK and DATA high - 11 */
    PORTB = 00011000 + lcd_part; // xxx11xxx
	//outportb(PORT, 0);/* CLOCK low and DATA low - 00 */
    PORTB = 00000000 + lcd_part;
}


/* output a '0' to AD9850 */
zero()
{
    char lcd_part = PORTB & 0b00000111;
	//outportb(PORT, 0);/* CLOCK low and DATA low - 00 */
    PORTB = 00000000 + lcd_part;
    //outportb(PORT, 2);/* CLOCK high and DATA low - 10 */
    PORTB = 00010000 + lcd_part; // xxx10xxx
	//outportb(PORT, 0);/* CLOCK low and DATA low - 00 */
    PORTB = 00010000 + lcd_part; // xxx10xxx
}