
#include <stdint.h>
#include "mcc_generated_files/system/system.h"

// Commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// Flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// Flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// Flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit



// Function declarations
void LCD_Init(uint8_t addr, uint8_t cols, uint8_t rows);
void LCD_Begin(uint8_t cols, uint8_t rows, uint8_t charsize);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_NoDisplay(void);
void LCD_Display(void);
void LCD_NoBlink(void);
void LCD_Blink(void);
void LCD_NoCursor(void);
void LCD_Cursor(void);
void LCD_ScrollDisplayLeft(void);
void LCD_ScrollDisplayRight(void);
void LCD_LeftToRight(void);
void LCD_RightToLeft(void);
void LCD_NoBacklight(void);
void LCD_Backlight(void);
void LCD_Autoscroll(void);
void LCD_NoAutoscroll(void);
void LCD_CreateChar(uint8_t location, uint8_t charmap[]);
void LCD_SetCursor(uint8_t col, uint8_t row);
void LCD_Write(uint8_t value);
void LCD_Print(const char* str);

// Private function declarations
void LCD_Command(uint8_t value);
void LCD_Send(uint8_t value, uint8_t mode);
void LCD_Write4Bits(uint8_t value);
void LCD_ExpanderWrite(uint8_t data);
void LCD_PulseEnable(uint8_t data);
void LCD_InitPriv(void);

