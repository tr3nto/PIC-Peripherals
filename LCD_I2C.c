#include "mcc_generated_files/system/system.h"
#include "LCD_I2C.h"
#include <string.h>

// Global variables to store LCD state
static uint8_t _lcd_addr;
static uint8_t _lcd_cols;
static uint8_t _lcd_rows;
static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;
static uint8_t _numlines;
static uint8_t _backlightval;
static bool _oled;

// I2C Write Function - Implement this for your specific microcontroller
static void I2C_Write(uint8_t addr, uint8_t data) {
    while (I2C1_Host.IsBusy());
    I2C1_Host.Write(addr, &data, 1);
}

void LCD_Init(uint8_t addr, uint8_t cols, uint8_t rows) {
    _lcd_addr = addr;
    _lcd_cols = cols;
    _lcd_rows = rows;
    _backlightval = LCD_NOBACKLIGHT;
    _oled = false;
    LCD_InitPriv();
}

void LCD_InitPriv(void) {
    // I2C_Init();  // Initialize I2C
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    LCD_Begin(_lcd_cols, _lcd_rows, LCD_5x8DOTS);
}

void LCD_Begin(uint8_t cols, uint8_t rows, uint8_t charsize) {
    if (rows > 1) {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = rows;

    if ((charsize != 0) && (rows == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }

    // Initialize display
    __delay_ms(50);
    
    LCD_ExpanderWrite(_backlightval);
    __delay_ms(1000);

    // 4-bit initialization
    LCD_Write4Bits(0x03 << 4);
    __delay_us(4500);
    
    LCD_Write4Bits(0x03 << 4);
    __delay_us(4500);
    
    LCD_Write4Bits(0x03 << 4);
    __delay_us(150);
    
    LCD_Write4Bits(0x02 << 4);

    // Set # lines, font size, etc.
    LCD_Command(LCD_FUNCTIONSET | _displayfunction);

    // Turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_Display();

    // Clear display
    LCD_Clear();

    // Initialize to default text direction
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);

    LCD_Home();
}

void LCD_Clear(void) {
    LCD_Command(LCD_CLEARDISPLAY);
    __delay_us(2000);  // this command takes a long time!
    if (_oled) {
        LCD_SetCursor(0, 0);
    }
}

void LCD_Home(void) {
    LCD_Command(LCD_RETURNHOME);  // set cursor position to zero
    __delay_us(2000);  // this command takes a long time!
}

void LCD_SetCursor(uint8_t col, uint8_t row) {
    int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row >= _numlines) {
        row = _numlines - 1;    // we count rows starting with 0
    }
    LCD_Command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LCD_NoDisplay(void) {
    _displaycontrol &= ~LCD_DISPLAYON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD_Display(void) {
    _displaycontrol |= LCD_DISPLAYON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LCD_NoCursor(void) {
    _displaycontrol &= ~LCD_CURSORON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD_Cursor(void) {
    _displaycontrol |= LCD_CURSORON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LCD_NoBlink(void) {
    _displaycontrol &= ~LCD_BLINKON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD_Blink(void) {
    _displaycontrol |= LCD_BLINKON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LCD_ScrollDisplayLeft(void) {
    LCD_Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCD_ScrollDisplayRight(void) {
    LCD_Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LCD_LeftToRight(void) {
    _displaymode |= LCD_ENTRYLEFT;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LCD_RightToLeft(void) {
    _displaymode &= ~LCD_ENTRYLEFT;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LCD_Autoscroll(void) {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LCD_NoAutoscroll(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LCD_CreateChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    LCD_Command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        LCD_Write(charmap[i]);
    }
}

// Turn the (optional) backlight off/on
void LCD_NoBacklight(void) {
    _backlightval = LCD_NOBACKLIGHT;
    LCD_ExpanderWrite(0);
}

void LCD_Backlight(void) {
    _backlightval = LCD_BACKLIGHT;
    LCD_ExpanderWrite(0);
}

// Helper Functions

void LCD_Command(uint8_t value) {
    LCD_Send(value, 0);
}

void LCD_Write(uint8_t value) {
    LCD_Send(value, Rs);
}

void LCD_Print(const char* str) {
    while (*str) {
        LCD_Write(*str++);
    }
}

void LCD_Send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    LCD_Write4Bits((highnib) | mode);
    LCD_Write4Bits((lownib) | mode);
}

void LCD_Write4Bits(uint8_t value) {
    LCD_ExpanderWrite(value);
    LCD_PulseEnable(value);
}

void LCD_ExpanderWrite(uint8_t data) {
    I2C_Write(_lcd_addr, data | _backlightval);
}

void LCD_PulseEnable(uint8_t data) {
    LCD_ExpanderWrite(data | En);    // En high
    __delay_us(1);                   // enable pulse must be >450ns
    LCD_ExpanderWrite(data & ~En);   // En low
    __delay_us(50);                  // commands need >37us to settle
}
