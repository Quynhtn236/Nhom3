#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "i2clcd.h"


void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);

// ThÃªm bá»Ÿi Lewis
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); // Di chuyá»ƒn con trá»
void ClrLcd(void); // XÃ³a LCD vÃ  Ä‘Æ°a vá» vá»‹ trÃ­ ban Ä‘áº§u
void typeln(const char *s);
void typeChar(char val);
int fd;  // CÃ³ thá»ƒ truy cáº­p tá»« táº¥t cáº£ cÃ¡c hÃ m con
void whiteLCD(const char *s, int line ); // viet vao LCD


void whiteLCD(const char *s, int line ){
 lcdLoc(line);
 typeln(s);
}

// XÃ³a LCD vÃ  Ä‘Æ°a vá» vá»‹ trÃ­ ban Ä‘áº§u (0x80)
void ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// Di chuyá»ƒn con trá» Ä‘áº¿n vá»‹ trÃ­ trÃªn LCD
void lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// Ghi kÃ½ tá»± vÃ o LCD táº¡i vá»‹ trÃ­ hiá»‡n táº¡i
void typeChar(char val)   {

  lcd_byte(val, LCD_CHR);
}


// Há»— trá»£ viáº¿t chuá»—i cÃ³ Ä‘á»™ dÃ i báº¥t ká»³
void typeln(const char *s)   {

  while ( *s ) lcd_byte(*(s++), LCD_CHR);

}

void lcd_byte(int bits, int mode)   {

  // Gá»­i byte dá»¯ liá»‡u Ä‘áº¿n cÃ¡c chÃ¢n dá»¯ liá»‡u
  // bits = dá»¯ liá»‡u
  // mode = 1 cho dá»¯ liá»‡u, 0 cho lá»‡nh
  int bits_high;
  int bits_low;
  // Sá»­ dá»¥ng hai láº§n ghi ná»­a byte cho LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // Bit cao
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Bit tháº¥p
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits)   {

  // Chuyá»ƒn Ä‘á»•i bit Enable trÃªn mÃ n hÃ¬nh LCD
  delayMicroseconds(100);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(100);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(100);
}


void lcd_init()   {

  if (wiringPiSetup () == -1) exit (1);

  fd = wiringPiI2CSetup(I2C_ADDR);

  printf("fd = %d ", fd);

  // Khá»Ÿi táº¡o mÃ n hÃ¬nh
  lcd_byte(0x33, LCD_CMD); // Khá»Ÿi táº¡o
  lcd_byte(0x32, LCD_CMD); // Khá»Ÿi táº¡o
  lcd_byte(0x06, LCD_CMD); // HÆ°á»›ng di chuyá»ƒn con trá»
  lcd_byte(0x0C, LCD_CMD); // 0x0F Báº­t, Nháº¥p nhÃ¡y Táº¯t
  lcd_byte(0x28, LCD_CMD); // Chiá»u dÃ i dá»¯ liá»‡u, sá»‘ dÃ²ng, kÃ­ch thÆ°á»›c font
  lcd_byte(0x01, LCD_CMD); // XÃ³a mÃ n hÃ¬nh
  delayMicroseconds(500);
}
