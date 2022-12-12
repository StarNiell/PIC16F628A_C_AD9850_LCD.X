/*
 * File:   main.c
 * Author: Aniello
 *
 * Created on 12 dicembre 2022, 13.24
 */

#define _XTAL_FREQ 4000000

#define CLK_595 RB1    //595 > SH_CP > PIN 11
#define DATA_595 RB2   //595 > DS    > PIN 14
#define LATCH_595 RB3  //595 > ST_CP > PIN 12

#include <xc.h>
#include "lcd_595.h"
#include "stdlib.h"

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



void main(void) {
    return;
}
