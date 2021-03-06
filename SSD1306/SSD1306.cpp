
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "SSD1306.h"

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//#define SSD1306_128_64
#define SSD1306_128_32

#ifdef SSD1306_128_64
  #define SSD1306_LCDHEIGHT             64
  #define SSD1306_LCDWIDTH              128
#endif

#if defined(SSD1306_128_32)
  #define SSD1306_LCDHEIGHT             32
  #define SSD1306_LCDWIDTH              128
#endif

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//// ERROR CHECKING SECTION

#if !defined(SSD1306_128_64) &&  !defined(SSD1306_128_32)
  #error "SSD1306 buffer size must be defined"
#endif

#if defined(SSD1306_128_64) && defined(SSD1306_128_32)
  #error "Only one SSD1306 buffer size define must be defined"
#endif

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//// OLED
#define OLED_ADDRESS                  0x3C     // OLED at address 0x3C in 7bit

#define SSD1306_PAGEMODE              0X01
#define SSD1306_HORIZONTALMODE        0X02

#define SSD1306_ADDR                  (0x3c<<1)
#define SSD1306_CMD                   0x80
#define SSD1306_DATA                  0x40

#define SSD1306_SETCONTRAST           0x81
#define SSD1306_NORMALDISPLAY         0xA6
#define SSD1306_INVERTDISPLAY         0xA7
#define SSD1306_DISPLAYALLON_RESUME   0xA4
#define SSD1306_DISPLAYALLON          0xA5
#define SSD1306_DISPLAYOFF            0xAE
#define SSD1306_DISPLAYON             0xAF
#define SSD1306_SETDISPLAYOFFSET      0xD3
#define SSD1306_SETCOMPINS            0xDA
#define SSD1306_SETVCOMDETECT         0xDB
#define SSD1306_SETDISPLAYCLOCKDIV    0xD5
#define SSD1306_SETPRECHARGE          0xD9
#define SSD1306_SETMULTIPLEX          0xA8
#define SSD1306_SETLOWCOLUMN          0x00
#define SSD1306_SETHIGHCOLUMN         0x10
#define SSD1306_SETSTARTLINE          0x40
#define SSD1306_MEMORYMODE            0x20
#define SSD1306_COLUMNADDR            0x21
#define SSD1306_PAGEADDR              0x22
#define SSD1306_FLIPV                 0xC0
#define SSD1306_COMSCANDEC            0xC8
#define SSD1306_NOP                   0xE3
#define SSD1306_SCANCOM0TOCOMN_1      0xC0
#define SSD1306_SCANCOMN_1TOCOM0      0xC8
#define SSD1306_FLIPH                 0xA0
#define SSD1306_CHARGEPUMP            0x8D
#define SSD1306_EXTERNALVCC           0x1
#define SSD1306_SWITCHCAPVCC          0x2
#define SSD1306_REMAP02SEG0           0xA0
#define SSD1306_REMAP1272SEG0         0xA1
#define SSD1306_ZOOM                  0xD6

#define SSD1306_STARTPAGE0            0xb0
#define SSD1306_STARTLOWERCOLUMN0     0x00
#define SSD1306_STARTHIGHCOLUMN0      0x10

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL                       0x2F
#define SSD1306_DEACTIVATE_SCROLL                     0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA              0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL               0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL                0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL  0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL   0x2A

#define END_INI_SEQ                                   0xFF

char digit10000(uint16_t v) {return '0' + v / 10000;}
char digit1000(uint16_t v) {return '0' + v / 1000 - (v/10000) * 10;}
char digit100(uint16_t v) {return '0' + v / 100 - (v/1000) * 10;}
char digit10(uint16_t v) {return '0' + v / 10 - (v/100) * 10;}
char digit1(uint16_t v) {return '0' + v - (v/10) * 10;}
  
//////////////////////////
//////////////////////////
//////////////////////////

const uint8_t PROGMEM myFont[][5] = { // Refer to "Times New Roman" Font Database... 5 x 7 font
  { 0x00,0x00,0x00,0x00,0x00},
  { 0x00,0x00,0x4F,0x00,0x00}, //   (  1)  ! - 0x0021 Exclamation Mark
  { 0x00,0x07,0x00,0x07,0x00}, //   (  2)  " - 0x0022 Quotation Mark
  { 0x14,0x7F,0x14,0x7F,0x14}, //   (  3)  # - 0x0023 Number Sign
  { 0x24,0x2A,0x7F,0x2A,0x12}, //   (  4)  $ - 0x0024 Dollar Sign
  { 0x23,0x13,0x08,0x64,0x62}, //   (  5)  % - 0x0025 Percent Sign
  { 0x36,0x49,0x55,0x22,0x50}, //   (  6)  & - 0x0026 Ampersand
  { 0x00,0x05,0x03,0x00,0x00}, //   (  7)  ' - 0x0027 Apostrophe
  { 0x00,0x1C,0x22,0x41,0x00}, //   (  8)  ( - 0x0028 Left Parenthesis
  { 0x00,0x41,0x22,0x1C,0x00}, //   (  9)  ) - 0x0029 Right Parenthesis
  { 0x14,0x08,0x3E,0x08,0x14}, //   ( 10)  * - 0x002A Asterisk
  { 0x08,0x08,0x3E,0x08,0x08}, //   ( 11)  + - 0x002B Plus Sign
  { 0x00,0x50,0x30,0x00,0x00}, //   ( 12)  , - 0x002C Comma
  { 0x08,0x08,0x08,0x08,0x08}, //   ( 13)  - - 0x002D Hyphen-Minus
  { 0x00,0x60,0x60,0x00,0x00}, //   ( 14)  . - 0x002E Full Stop
  { 0x20,0x10,0x08,0x04,0x02}, //   ( 15)  / - 0x002F Solidus
  { 0x3E,0x51,0x49,0x45,0x3E}, //   ( 16)  0 - 0x0030 Digit Zero
  { 0x00,0x42,0x7F,0x40,0x00}, //   ( 17)  1 - 0x0031 Digit One
  { 0x42,0x61,0x51,0x49,0x46}, //   ( 18)  2 - 0x0032 Digit Two
  { 0x21,0x41,0x45,0x4B,0x31}, //   ( 19)  3 - 0x0033 Digit Three
  { 0x18,0x14,0x12,0x7F,0x10}, //   ( 20)  4 - 0x0034 Digit Four
  { 0x27,0x45,0x45,0x45,0x39}, //   ( 21)  5 - 0x0035 Digit Five
  { 0x3C,0x4A,0x49,0x49,0x30}, //   ( 22)  6 - 0x0036 Digit Six
  { 0x01,0x71,0x09,0x05,0x03}, //   ( 23)  7 - 0x0037 Digit Seven
  { 0x36,0x49,0x49,0x49,0x36}, //   ( 24)  8 - 0x0038 Digit Eight
  { 0x06,0x49,0x49,0x29,0x1E}, //   ( 25)  9 - 0x0039 Dight Nine
  { 0x00,0x36,0x36,0x00,0x00}, //   ( 26)  : - 0x003A Colon
  { 0x00,0x56,0x36,0x00,0x00}, //   ( 27)  ; - 0x003B Semicolon
  { 0x08,0x14,0x22,0x41,0x00}, //   ( 28)  < - 0x003C Less-Than Sign
  { 0x14,0x14,0x14,0x14,0x14}, //   ( 29)  = - 0x003D Equals Sign
  { 0x00,0x41,0x22,0x14,0x08}, //   ( 30)  > - 0x003E Greater-Than Sign
  { 0x02,0x01,0x51,0x09,0x06}, //   ( 31)  ? - 0x003F Question Mark
  { 0x32,0x49,0x79,0x41,0x3E}, //   ( 32)  @ - 0x0040 Commercial At
  { 0x7E,0x11,0x11,0x11,0x7E}, //   ( 33)  A - 0x0041 Latin Capital Letter A
  { 0x7F,0x49,0x49,0x49,0x36}, //   ( 34)  B - 0x0042 Latin Capital Letter B
  { 0x3E,0x41,0x41,0x41,0x22}, //   ( 35)  C - 0x0043 Latin Capital Letter C
  { 0x7F,0x41,0x41,0x22,0x1C}, //   ( 36)  D - 0x0044 Latin Capital Letter D
  { 0x7F,0x49,0x49,0x49,0x41}, //   ( 37)  E - 0x0045 Latin Capital Letter E
  { 0x7F,0x09,0x09,0x09,0x01}, //   ( 38)  F - 0x0046 Latin Capital Letter F
  { 0x3E,0x41,0x49,0x49,0x7A}, //   ( 39)  G - 0x0047 Latin Capital Letter G
  { 0x7F,0x08,0x08,0x08,0x7F}, //   ( 40)  H - 0x0048 Latin Capital Letter H
  { 0x00,0x41,0x7F,0x41,0x00}, //   ( 41)  I - 0x0049 Latin Capital Letter I
  { 0x20,0x40,0x41,0x3F,0x01}, //   ( 42)  J - 0x004A Latin Capital Letter J
  { 0x7F,0x08,0x14,0x22,0x41}, //   ( 43)  K - 0x004B Latin Capital Letter K
  { 0x7F,0x40,0x40,0x40,0x40}, //   ( 44)  L - 0x004C Latin Capital Letter L
  { 0x7F,0x02,0x0C,0x02,0x7F}, //   ( 45)  M - 0x004D Latin Capital Letter M
  { 0x7F,0x04,0x08,0x10,0x7F}, //   ( 46)  N - 0x004E Latin Capital Letter N
  { 0x3E,0x41,0x41,0x41,0x3E}, //   ( 47)  O - 0x004F Latin Capital Letter O
  { 0x7F,0x09,0x09,0x09,0x06}, //   ( 48)  P - 0x0050 Latin Capital Letter P
  { 0x3E,0x41,0x51,0x21,0x5E}, //   ( 49)  Q - 0x0051 Latin Capital Letter Q
  { 0x7F,0x09,0x19,0x29,0x46}, //   ( 50)  R - 0x0052 Latin Capital Letter R
  { 0x46,0x49,0x49,0x49,0x31}, //   ( 51)  S - 0x0053 Latin Capital Letter S
  { 0x01,0x01,0x7F,0x01,0x01}, //   ( 52)  T - 0x0054 Latin Capital Letter T
  { 0x3F,0x40,0x40,0x40,0x3F}, //   ( 53)  U - 0x0055 Latin Capital Letter U
  { 0x1F,0x20,0x40,0x20,0x1F}, //   ( 54)  V - 0x0056 Latin Capital Letter V
  { 0x3F,0x40,0x38,0x40,0x3F}, //   ( 55)  W - 0x0057 Latin Capital Letter W
  { 0x63,0x14,0x08,0x14,0x63}, //   ( 56)  X - 0x0058 Latin Capital Letter X
  { 0x07,0x08,0x70,0x08,0x07}, //   ( 57)  Y - 0x0059 Latin Capital Letter Y
  { 0x61,0x51,0x49,0x45,0x43}, //   ( 58)  Z - 0x005A Latin Capital Letter Z
  { 0x00,0x7F,0x41,0x41,0x00}, //   ( 59)  [ - 0x005B Left Square Bracket
  { 0x02,0x04,0x08,0x10,0x20}, //   ( 60)  \ - 0x005C Reverse Solidus
  { 0x00,0x41,0x41,0x7F,0x00}, //   ( 61)  ] - 0x005D Right Square Bracket
  { 0x04,0x02,0x01,0x02,0x04}, //   ( 62)  ^ - 0x005E Circumflex Accent
  { 0x40,0x40,0x40,0x40,0x40}, //   ( 63)  _ - 0x005F Low Line
  { 0x01,0x02,0x04,0x00,0x00}, //   ( 64)  ` - 0x0060 Grave Accent
  { 0x20,0x54,0x54,0x54,0x78}, //   ( 65)  a - 0x0061 Latin Small Letter A
  { 0x7F,0x48,0x44,0x44,0x38}, //   ( 66)  b - 0x0062 Latin Small Letter B
  { 0x38,0x44,0x44,0x44,0x20}, //   ( 67)  c - 0x0063 Latin Small Letter C
  { 0x38,0x44,0x44,0x48,0x7F}, //   ( 68)  d - 0x0064 Latin Small Letter D
  { 0x38,0x54,0x54,0x54,0x18}, //   ( 69)  e - 0x0065 Latin Small Letter E
  { 0x08,0x7E,0x09,0x01,0x02}, //   ( 70)  f - 0x0066 Latin Small Letter F
  { 0x06,0x49,0x49,0x49,0x3F}, //   ( 71)  g - 0x0067 Latin Small Letter G
  { 0x7F,0x08,0x04,0x04,0x78}, //   ( 72)  h - 0x0068 Latin Small Letter H
  { 0x00,0x44,0x7D,0x40,0x00}, //   ( 73)  i - 0x0069 Latin Small Letter I
  { 0x20,0x40,0x44,0x3D,0x00}, //   ( 74)  j - 0x006A Latin Small Letter J
  { 0x7F,0x10,0x28,0x44,0x00}, //   ( 75)  k - 0x006B Latin Small Letter K
  { 0x00,0x41,0x7F,0x40,0x00}, //   ( 76)  l - 0x006C Latin Small Letter L
  { 0x7C,0x04,0x18,0x04,0x7C}, //   ( 77)  m - 0x006D Latin Small Letter M
  { 0x7C,0x08,0x04,0x04,0x78}, //   ( 78)  n - 0x006E Latin Small Letter N
  { 0x38,0x44,0x44,0x44,0x38}, //   ( 79)  o - 0x006F Latin Small Letter O
  { 0x7C,0x14,0x14,0x14,0x08}, //   ( 80)  p - 0x0070 Latin Small Letter P
  { 0x08,0x14,0x14,0x18,0x7C}, //   ( 81)  q - 0x0071 Latin Small Letter Q
  { 0x7C,0x08,0x04,0x04,0x08}, //   ( 82)  r - 0x0072 Latin Small Letter R
  { 0x48,0x54,0x54,0x54,0x20}, //   ( 83)  s - 0x0073 Latin Small Letter S
  { 0x04,0x3F,0x44,0x40,0x20}, //   ( 84)  t - 0x0074 Latin Small Letter T
  { 0x3C,0x40,0x40,0x20,0x7C}, //   ( 85)  u - 0x0075 Latin Small Letter U
  { 0x1C,0x20,0x40,0x20,0x1C}, //   ( 86)  v - 0x0076 Latin Small Letter V
  { 0x3C,0x40,0x30,0x40,0x3C}, //   ( 87)  w - 0x0077 Latin Small Letter W
  { 0x44,0x28,0x10,0x28,0x44}, //   ( 88)  x - 0x0078 Latin Small Letter X
  { 0x0C,0x50,0x50,0x50,0x3C}, //   ( 89)  y - 0x0079 Latin Small Letter Y
  { 0x44,0x64,0x54,0x4C,0x44}, //   ( 90)  z - 0x007A Latin Small Letter Z
  { 0x00,0x08,0x36,0x41,0x00}, //   ( 91)  { - 0x007B Left Curly Bracket
  { 0x00,0x00,0x7F,0x00,0x00}, //   ( 92)  | - 0x007C Vertical Line
  { 0x00,0x41,0x36,0x08,0x00}, //   ( 93)  } - 0x007D Right Curly Bracket
  { 0x02,0x01,0x02,0x04,0x02}, //   ( 94)  ~ - 0x007E Tilde
  { 0x3E,0x55,0x55,0x41,0x22}, //   ( 95)  C - 0x0080 <Control>
  { 0x00,0x00,0x00,0x00,0x00}, //   ( 96)    - 0x00A0 No-Break Space
  { 0x00,0x00,0x79,0x00,0x00}, //   ( 97)  ! - 0x00A1 Inverted Exclamation Mark
  { 0x18,0x24,0x74,0x2E,0x24}, //   ( 98)  c - 0x00A2 Cent Sign
  { 0x48,0x7E,0x49,0x42,0x40}, //   ( 99)  L - 0x00A3 Pound Sign
  { 0x5D,0x22,0x22,0x22,0x5D}, //   (100)  o - 0x00A4 Currency Sign
  { 0x15,0x16,0x7C,0x16,0x15}, //   (101)  Y - 0x00A5 Yen Sign
  { 0x00,0x00,0x77,0x00,0x00}, //   (102)  | - 0x00A6 Broken Bar
  { 0x0A,0x55,0x55,0x55,0x28}, //   (103)    - 0x00A7 Section Sign
  { 0x00,0x01,0x00,0x01,0x00}, //   (104)  " - 0x00A8 Diaeresis
  { 0x00,0x0A,0x0D,0x0A,0x04}, //   (105)    - 0x00AA Feminine Ordinal Indicator
  { 0x08,0x14,0x2A,0x14,0x22}, //   (106) << - 0x00AB Left-Pointing Double Angle Quotation Mark
  { 0x04,0x04,0x04,0x04,0x1C}, //   (107)    - 0x00AC Not Sign
  { 0x00,0x08,0x08,0x08,0x00}, //   (108)  - - 0x00AD Soft Hyphen
  { 0x01,0x01,0x01,0x01,0x01}, //   (109)    - 0x00AF Macron
  { 0x00,0x02,0x05,0x02,0x00}, //   (110)    - 0x00B0 Degree Sign
  { 0x44,0x44,0x5F,0x44,0x44}, //   (111) +- - 0x00B1 Plus-Minus Sign
  { 0x00,0x00,0x04,0x02,0x01}, //   (112)  ` - 0x00B4 Acute Accent
  { 0x7E,0x20,0x20,0x10,0x3E}, //   (113)  u - 0x00B5 Micro Sign
  { 0x06,0x0F,0x7F,0x00,0x7F}, //   (114)    - 0x00B6 Pilcrow Sign
  { 0x00,0x18,0x18,0x00,0x00}, //   (115)  . - 0x00B7 Middle Dot
  { 0x00,0x40,0x50,0x20,0x00}, //   (116)    - 0x00B8 Cedilla
  { 0x00,0x0A,0x0D,0x0A,0x00}, //   (117)    - 0x00BA Masculine Ordinal Indicator
  { 0x22,0x14,0x2A,0x14,0x08}, //   (118) >> - 0x00BB Right-Pointing Double Angle Quotation Mark
  { 0x17,0x08,0x34,0x2A,0x7D}, //   (119) /4 - 0x00BC Vulgar Fraction One Quarter
  { 0x17,0x08,0x04,0x6A,0x59}, //   (120) /2 - 0x00BD Vulgar Fraction One Half
  { 0x30,0x48,0x45,0x40,0x20}, //   (121)  ? - 0x00BE Inverted Question Mark
  { 0x42,0x00,0x42,0x00,0x42}, //   (122)    - 0x00BF Bargraph - 0
  { 0x7E,0x42,0x00,0x42,0x00}, //   (123)    - 0x00BF Bargraph - 1
  { 0x7E,0x7E,0x00,0x42,0x00}, //   (124)    - 0x00BF Bargraph - 2
  { 0x7E,0x7E,0x7E,0x42,0x00}, //   (125)    - 0x00BF Bargraph - 3
  { 0x7E,0x7E,0x7E,0x7E,0x00}, //   (126)    - 0x00BF Bargraph - 4
  { 0x7E,0x7E,0x7E,0x7E,0x7E}, //   (127)    - 0x00BF Bargraph - 5
};

//////////////////////////
//////////////////////////
//////////////////////////

// the memory buffer for the LCD

static uint8_t videoBuffer[SSD1306_LCDHEIGHT * (SSD1306_LCDWIDTH / 8)] = { 
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
,0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x80, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80
,0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xFF
,0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00
,0x80, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80
,0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x8C, 0x8E, 0x84, 0x00, 0x00, 0x80, 0xF8
,0xF8, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80
,0x00, 0xE0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xC7, 0x01, 0x01
,0x01, 0x01, 0x83, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xFE, 0xC7, 0x01, 0x01, 0x01, 0x01, 0x83, 0xFF
,0xFF, 0xFF, 0x00, 0x38, 0xFE, 0xC7, 0x83, 0x01, 0x01, 0x01, 0x83, 0xC7, 0xFF, 0xFF, 0x00, 0x00
,0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0xFF, 0xFF, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0xFF
,0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFF
,0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x03, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x8F
,0x8F, 0x9F, 0xBF, 0xFF, 0xFF, 0xC3, 0xC0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC
,0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x01, 0x03, 0x03, 0x03
,0x03, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01
,0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00
,0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
,0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03
,0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#ifdef SSD1306_128_64
,0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F
,0x87, 0xC7, 0xF7, 0xFF, 0xFF, 0x1F, 0x1F, 0x3D, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0x7C, 0x7D, 0xFF
,0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x00, 0x30, 0x30, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC0, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0xC0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F
,0x0F, 0x07, 0x1F, 0x7F, 0xFF, 0xFF, 0xF8, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xE0
,0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00
,0x00, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x0E, 0xFC, 0xF8, 0x00, 0x00, 0xF0, 0xF8, 0x1C, 0x0E
,0x06, 0x06, 0x06, 0x0C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFC
,0xFE, 0xFC, 0x00, 0x18, 0x3C, 0x7E, 0x66, 0xE6, 0xCE, 0x84, 0x00, 0x00, 0x06, 0xFF, 0xFF, 0x06
,0x06, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x06, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0xC0, 0xF8
,0xFC, 0x4E, 0x46, 0x46, 0x46, 0x4E, 0x7C, 0x78, 0x40, 0x18, 0x3C, 0x76, 0xE6, 0xCE, 0xCC, 0x80
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00
,0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x0C
,0x18, 0x18, 0x0C, 0x06, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x01, 0x0F, 0x0E, 0x0C, 0x18, 0x0C, 0x0F
,0x07, 0x01, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00
,0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x07
,0x07, 0x0C, 0x0C, 0x18, 0x1C, 0x0C, 0x06, 0x06, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif
};

//////////////////////////
//////////////////////////
//////////////////////////

#if defined(SSD1306_128_64) //ssd1306_128x64_init

  static const uint8_t ssd1306_init[] PROGMEM = {
    SSD1306_DISPLAYOFF,         // display off, sleep mode 
    //- HW config --------------
    SSD1306_SETSTARTLINE,       // start line - 0x40 -
    SSD1306_REMAP1272SEG0,      //Set Segment Re-Map  
    SSD1306_SETMULTIPLEX,
    0x3f,                       // multiplex ratio 
    SSD1306_SCANCOMN_1TOCOM0,   //Set COM Output Scan Direction
    SSD1306_SETDISPLAYOFFSET,
    0x00,                       // display offset   
    SSD1306_SETCOMPINS,
    0x12,   
    //- Timing and Driving ----- 
    SSD1306_SETDISPLAYCLOCKDIV,
    0x80,                       // clock divide ratio (0x00=1) and oscillator frequency (0x8) 
    SSD1306_SETPRECHARGE,
    0xf1,     
    SSD1306_CHARGEPUMP,         // charge pump setting (p62): 0x014 enable, 0x010 disable 
    0x14,                       
    SSD1306_SETVCOMDETECT,
    0x40,  
    //- Memory mode -----------  
    SSD1306_MEMORYMODE,         // Vertical addressing mode, c8: reverse 
    0x00,        
    SSD1306_COLUMNADDR,         // Column addres start 0 em 127
    0x00,
    0x7F,
    SSD1306_PAGEADDR,            // Page addres start 0 em 7
    0x00,
    0x07,
    SSD1306_STARTPAGE0,          // Start page 0 
    SSD1306_STARTLOWERCOLUMN0,    //Start low column 0x00
    SSD1306_STARTHIGHCOLUMN0,     //Start low column 0x10
    //- Miscellaneous ---------  
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_FLIPH,
    SSD1306_FLIPV,
    SSD1306_ZOOM,                //Zoom off
    0x00,                       
    SSD1306_SETCONTRAST,         //Set initial contrast
    0xcf, 
    SSD1306_DISPLAYON,          //Set Display On 
    END_INI_SEQ                 // end of sequence 
  };
  
#else //ssd1306_128x32_init

  static const uint8_t ssd1306_init[] PROGMEM = {
    SSD1306_DISPLAYOFF,         // display off, sleep mode 
    //- HW config --------------
    SSD1306_SETSTARTLINE,       // start line - 0x40 -
    SSD1306_REMAP1272SEG0,      //Set Segment Re-Map
    SSD1306_SETMULTIPLEX,
    0x3f,                       // multiplex ratio 
    SSD1306_SCANCOMN_1TOCOM0,   //Set COM Output Scan Direction
    SSD1306_SETDISPLAYOFFSET,
    0x00,                       // display offset 
    SSD1306_SETCOMPINS,
    0x12,
    //- Timing and Driving -----
    SSD1306_SETDISPLAYCLOCKDIV,
    0x80,                       // clock divide ratio (0x00=1) and oscillator frequency (0x8) 
    SSD1306_SETPRECHARGE,
    0xf1,
    SSD1306_CHARGEPUMP,         // charge pump setting (p62): 0x014 enable, 0x010 disable 
    0x14,
    SSD1306_SETVCOMDETECT,
    0x40,
    //- Memory mode -----------
    SSD1306_MEMORYMODE,         // Vertical addressing mode, c8: reverse 
    0x00,
    SSD1306_COLUMNADDR,         // Column addres start 0 em 127
    0x00,
    0x7F,
    SSD1306_PAGEADDR,            // Page addres start 0 em 3
    0x00,
    0x03,
    SSD1306_STARTPAGE0,          // Start page 0 
    SSD1306_STARTLOWERCOLUMN0,    //Start low column 0x00
    SSD1306_STARTHIGHCOLUMN0,     //Start low column 0x10
    //- Miscellaneous ---------
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_FLIPH,
    SSD1306_FLIPV,
    SSD1306_ZOOM,                //Zoom on
    0x01,
    SSD1306_SETCONTRAST,         //Set initial contrast
    0xcf,
    SSD1306_DISPLAYON,          //Set Display On
    END_INI_SEQ                 // end of sequence 
  };
#endif

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

void  SSD1306::init(bool fastmode)
{
  if(fastmode)  // switch to 400KHz I2C - eheheh
    TWBR = ((F_CPU / 400000L) - 16) / 2; // see twi_init in Wire/utility/twi.c
  
  
  uint8_t dataBuffer;
  const uint8_t* pBuffer (ssd1306_init);
  while(1)
  {
    dataBuffer = pgm_read_byte(pBuffer++); // call the macro to read ROM byte and put it in buffer 
    if(dataBuffer == END_INI_SEQ )
      return;
    _send_Cmd(dataBuffer);
  }
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::displayVideoBuffer(void) 
{
  _send_Cmd(SSD1306_STARTPAGE0);     //set page address
  _send_Data_buff(videoBuffer,SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH/8);
}

//////////////////////////
//////////////////////////
//////////////////////////

// clear everything
void SSD1306::clearVideoBuffer(void)
{
  memset(videoBuffer, 0, SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::clearDisplay(void)
{
  _send_Cmd(SSD1306_STARTPAGE0);     //set page address
  for(int i=0;i<1024;i++)
    _send_Data(0x00);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::normalDisplay()
{
  _send_Cmd(SSD1306_NORMALDISPLAY); //Set Normal Display
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::startScrollLeft( uint8_t start, uint8_t stop)
{
  _startScroll(SSD1306_LEFT_HORIZONTAL_SCROLL,start,stop);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::startScrollRight( uint8_t start, uint8_t stop)
{
  _startScroll(SSD1306_RIGHT_HORIZONTAL_SCROLL,start,stop);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::startScrollDiagLeft( uint8_t start, uint8_t stop)
{
  _startScrollDiag(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL,start,stop);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::startScrollDiagRight( uint8_t start, uint8_t stop)
{
  _startScrollDiag(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL,start,stop);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::deactive_scroll()
{
  _send_Cmd(SSD1306_DEACTIVATE_SCROLL);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::dim(bool dim) 
{
  _send_Cmd(SSD1306_SETCONTRAST);
  _send_Cmd(dim?0:0xCF);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::inverDisplay()
{
  _send_Cmd(SSD1306_INVERTDISPLAY);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::brightness(unsigned char Brightness)
{
  _send_Cmd(SSD1306_SETCONTRAST);
  _send_Cmd(Brightness);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::_startScroll(uint8_t dir, uint8_t start, uint8_t stop)
{
  _send_Cmd(dir);
  _send_Cmd(0x00);
  _send_Cmd(start);
  _send_Cmd(0x00);
  _send_Cmd(stop);
  _send_Cmd(0X00);
  _send_Cmd(0xff);
  _send_Cmd(SSD1306_ACTIVATE_SCROLL);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::_startScrollDiag(uint8_t dir, uint8_t start, uint8_t stop)
{
  _send_Cmd(SSD1306_SET_VERTICAL_SCROLL_AREA);  
  _send_Cmd(0x00);
  _send_Cmd(SSD1306_LCDHEIGHT);
  _send_Cmd(dir);
  _send_Cmd(0x00);
  _send_Cmd(start);
  _send_Cmd(0x00);
  _send_Cmd(stop);
  _send_Cmd(0X01);
  _send_Cmd(SSD1306_ACTIVATE_SCROLL);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::send_string(const char *s, uint8_t row, uint8_t col, bool bInvert)
{
  while (*s) 
    _drawChar(*s++, row, col, bInvert);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::send_long(long lData,uint8_t row, uint8_t col, bool bInvert)
{
  _send_long(lData, row, col, bInvert);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::send_float(float fData, uint8_t row, uint8_t col, unsigned char decimal, bool bInvert)
{
  _send_float(fData, row, col, decimal, bInvert);
}

//////////////////////////
//////////////////////////
//////////////////////////

bool SSD1306::getPixel(uint8_t row, uint8_t col)
{
  return (_getPixelByte(row,col) & (1<<(row%8)) != 0);
}

//////////////////////////
//////////////////////////
//////////////////////////

void  SSD1306::setPixel(uint8_t row, uint8_t col)
{
  *(_getPixelByteP(row,col)) |= (1<<(row%8));
}

//////////////////////////
//////////////////////////
//////////////////////////

void  SSD1306::clearPixel(uint8_t row, uint8_t col)
{
  *(_getPixelByteP(row,col)) &= ~(1<<(row%8));
}

//////////////////////////
//////////////////////////
//////////////////////////

void  SSD1306::drawHLine(uint8_t row, uint8_t col, int8_t lenght)
{
  if (lenght>=0)
  {
    while((lenght--) > 0)
      setPixel(row, col++);
  }
  else
  {
    while((lenght++) < 0)
      setPixel(row, col--);
  }
}

//////////////////////////
//////////////////////////
//////////////////////////

void  SSD1306::drawVLine(uint8_t row, uint8_t col, int8_t lenght)
{
  if (lenght>0)
  {
    while((lenght--) > 0)
      setPixel(row++, col);
  }
  else
  {
    while((lenght++) < 0)
      setPixel(row--, col);
  }
}

//////////////////////////
//////////////////////////
//////////////////////////

void  SSD1306::drawFillRectangle(uint8_t row, uint8_t col, int8_t width, int8_t height)
{
  drawRectangle(row,col,width,height,127);
}

//////////////////////////
//////////////////////////
//////////////////////////

void  SSD1306::drawRectangle(uint8_t row, uint8_t col, int8_t width, int8_t height, uint8_t thickness)
{
  if(!thickness || !width || !height)
    return;
    
  while((thickness--) > 0 && width > 0 && height > 0)
  {
    drawHLine(row, col, width);
    drawVLine(row, col + width, height);
    drawHLine(row + height, col + width, -width);
    drawVLine(row + height, col, -height);
    row++;col++;width-=2;height-=2;
  }
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

void SSD1306::_send_Cmd(uint8_t command) 
{
  I2Cdev::writeByte(false,OLED_ADDRESS,SSD1306_CMD,command);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::_send_Data(uint8_t val) 
{
  I2Cdev::writeByte(false,OLED_ADDRESS,SSD1306_DATA,val);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::_send_Data_buff(uint8_t *buff, uint16_t size)
{
  uint8_t uiChunk(size/16);
  uint8_t i (0);
  for( ; i < uiChunk ;i++)
    I2Cdev::writeBytes(false,OLED_ADDRESS, SSD1306_DATA,16, buff + (16*i));
  I2Cdev::writeBytes(false,OLED_ADDRESS, SSD1306_DATA,size%8, buff + (16*i));
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::_send_float(float fData, uint8_t& row, uint8_t& col, unsigned char decimal, bool bInvert)
{
  if(fData<0.0f)
  {
    _drawChar('-', row, col, bInvert);
    fData = -fData;
  }

  long temp = (long)fData;
  _send_long(temp, row, col, bInvert);
  
  if(decimal>0)
    _drawChar('.', row, col, bInvert);
  
  float rounding = 0.5f;
  for (unsigned char i=0; i<decimal; ++i)
    rounding /= 10.0f;
  
  fData += rounding;

  float decy = fData - (float)temp;//decimal part,
  for(unsigned char i=0;i<decimal;i++)//4
  {
    decy *=10.0f;// for the next decimal
    temp = (long)decy;//get the decimal
    _send_long(temp, row, col, bInvert);
    decy -= (float)temp;
  }
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::_send_long(long lData,uint8_t& row, uint8_t& col, bool bInvert)
{
    unsigned char char_buffer[10]="";

    if (lData < 0)
    {
        _drawChar('-', row, col, bInvert);
        lData = -lData;
    }
    else if (lData == 0)
    {
        _drawChar('0', row, col, bInvert);
        return;
    }

    uint8_t i = 0;
    while (lData > 0)
    {
        char_buffer[i++] = lData % 10;
        lData /= 10;
    }

    for(; i > 0; i--)
      _drawChar('0'+ char_buffer[i - 1], row, col, bInvert);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::_drawChar(unsigned char ascii, uint8_t& row, uint8_t& col, bool bInvert)
{
  uint8_t c;
  
  for(uint8_t i=0;i<5;i++)
  {
    c = (uint8_t)(pgm_read_byte(&(myFont[ascii - 32][i])))^(bInvert?0b01111111:0);
    for (uint8_t j = 0 ; j < 8 ;j++)
      c&(1<<j)?setPixel(row+j,col):clearPixel(row+j,col); 
    col++;
  }
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

uint16_t SSD1306::_getPixelBytePos(uint8_t row, uint8_t col)
{
  #ifdef SSD1306_128_64
    row &= 0x3F;
  #else
    row &= 0x1F;
  #endif

  return ((row/8)*SSD1306_LCDWIDTH) + (col&0x7F);
}

//////////////////////////
//////////////////////////
//////////////////////////

uint8_t SSD1306::_getPixelByte(uint8_t row, uint8_t col)
{
  return videoBuffer[_getPixelBytePos(row,col)];
}

//////////////////////////
//////////////////////////
//////////////////////////

uint8_t* SSD1306::_getPixelByteP(uint8_t row, uint8_t col)
{
  return &(videoBuffer[_getPixelBytePos(row,col)]);
}

//////////////////////////
//////////////////////////
//////////////////////////

void SSD1306::_setPixelByte(uint8_t row, uint8_t col,uint8_t data)
{
  *_getPixelByteP(row,col) = data;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

