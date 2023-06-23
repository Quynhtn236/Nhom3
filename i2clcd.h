#ifndef I2CLCD_H
#define I2CLCD_H

// Định nghĩa một số thông số của thiết bị
#define I2C_ADDR   0x27 // Địa chỉ thiết bị I2C

// Định nghĩa một số hằng số của thiết bị
#define LCD_CHR  1 // Chế độ - Gửi dữ liệu
#define LCD_CMD  0 // Chế độ - Gửi lệnh

#define LINE1  0x80 // Dòng 1
#define LINE2  0xC0 // Dòng 2

#define LCD_BACKLIGHT   0x08  // Bật đèn nền LCD
// LCD_BACKLIGHT = 0x00  # Tắt đèn nền LCD

#define ENABLE  0b00000100 // Bit Enable



void lcd_init(void); // khoi dong LCD
void ClrLcd(void); // Xóa LCD và đưa về vị trí ban đầu
void whiteLCD(const char *s, int line ); // viet vao LCD

#endif
