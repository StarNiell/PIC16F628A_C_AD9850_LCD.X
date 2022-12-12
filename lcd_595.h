
#include <xc.h>

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

uint8_t _cols;
uint8_t _rows;
uint8_t _displayControl;
uint8_t _displayMode;
uint8_t charsize = LCD_5x8DOTS;

void SHIFT595_write(uint8_t dato)
{   
    for(uint8_t i=7; i<255; i--)
    {
        CLK_595 = 0;
        LATCH_595 = 0;
        NOP();

        if((dato&(1<<i))==0)   
            DATA_595 = 0; 
        else
            DATA_595 = 1;
        
        NOP();
        CLK_595 = 1;    
        NOP();
    }
    CLK_595 = 0;
    LATCH_595 = 1;
    NOP();
}

/* Envia comando por LCD ------------------------------------------*/

void LCD_595_cmd(char cmd)
{
  	char data_u, data_l;
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
    
//  SHIFT595_write(data_u|0b0010);  //en=1, rs=0  1100
//	SHIFT595_write(data_u|0b0000);  //en=0, rs=0  1000
//	SHIFT595_write(data_l|0b0010);  //en=1, rs=0
//	SHIFT595_write(data_l|0b0000);  //en=0, rs=0
    
    // rs = Q1 -> 0010
    // en = Q3 -> 1000
    SHIFT595_write(data_u|0b1000);  //en=1, rs=0  1000
	SHIFT595_write(data_u|0b0000);  //en=0, rs=0  0000
	SHIFT595_write(data_l|0b1000);  //en=1, rs=0  1000
	SHIFT595_write(data_l|0b0000);  //en=0, rs=0  0000

    __delay_ms(2);
    
}

/* Envia dato por LCD ------------------------------------------*/

void LCD_595_char(char data)
{
	char data_u, data_l;
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
  
//  SHIFT595_write(data_u|0b0011);  //en=1, rs=1  1101
//	SHIFT595_write(data_u|0b0001);  //en=0, rs=1  1001
//	SHIFT595_write(data_l|0b0011);  //en=1, rs=1  1101
//	SHIFT595_write(data_l|0b0001);  //en=0, rs=1  1001
    
    // rs = Q1 -> 0010
    // en = Q3 -> 1000
    SHIFT595_write(data_u|0b1010);  //en=1, rs=1  1010
	SHIFT595_write(data_u|0b0010);  //en=0, rs=1  0010
	SHIFT595_write(data_l|0b1010);  //en=1, rs=1  1010
	SHIFT595_write(data_l|0b0010);  //en=0, rs=1  0010

    __delay_ms(2);
}

/* Inicializa LCD -----------------------------------------------*/

void LCD_595_init(uint8_t cols, uint8_t rows)
{
    _cols = cols;
    _rows = rows;
    _displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    _displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    
	LCD_595_cmd(0x02);
	LCD_595_cmd(0x28);
	LCD_595_cmd(0x0c);
	LCD_595_cmd(0x80);
//	LCD_595_cmd(LCD_CLEARDISPLAY);

    if (_rows > 1) {
        LCD_595_cmd(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | charsize);
    //} else if (_rows == 1 && charsize != 0) {
    //    LCD_595_cmd(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | charsize);
    } else {
        LCD_595_cmd(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | charsize);
    }

    LCD_595_cmd(LCD_DISPLAYCONTROL | _displayControl);
    LCD_595_cmd(LCD_CLEARDISPLAY);
    //LCD_595_cmd(LCD_ENTRYMODESET | _displayMode);
    
}

/* Envia cadena de caracteres al LCD -----------------------------*/

void LCD_595_print(char *str)
{
	while (*str) LCD_595_char(*str++);
}

void LCD_595_clear() {
    LCD_595_cmd(LCD_CLEARDISPLAY);
}

void LCD_595_home() {
    LCD_595_cmd(LCD_RETURNHOME);
}

void LCD_595_setCursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40, (0x00 | _cols), (0x40 | _cols)};
    if (row > _rows) {
        row = _rows - 1;
    }
    LCD_595_cmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}