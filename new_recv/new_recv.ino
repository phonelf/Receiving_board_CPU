#include <PinChangeInt.h>
#include <Servo.h>
#include "PWM.hpp"

#define BACK_LIGHT_ANALOG 0
#define PIN_SCE 50
#define PIN_RESET A5
#define PIN_DC 53
#define PIN_SDIN 51
#define PIN_SCLK 52
#define LCD_COMMAND 0
#define LCD_DATA 1
#define LCD_X 84
#define LCD_Y 48

#define FLCpin1 30
#define FLCpin2 31
#define PWMout0 6
#define PWMout1 7
#define PWMout2 8
#define PWMout3 9

static const byte ASCII[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00} // 20
    ,
    {0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
    ,
    {0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
    ,
    {0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
    ,
    {0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
    ,
    {0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
    ,
    {0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
    ,
    {0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
    ,
    {0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
    ,
    {0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
    ,
    {0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
    ,
    {0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
    ,
    {0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
    ,
    {0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
    ,
    {0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
    ,
    {0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
    ,
    {0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
    ,
    {0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
    ,
    {0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
    ,
    {0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
    ,
    {0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
    ,
    {0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
    ,
    {0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
    ,
    {0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
    ,
    {0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
    ,
    {0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
    ,
    {0x00, 0x36, 0x36, 0x00, 0x00} // 3a ：
    ,
    {0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
    ,
    {0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
    ,
    {0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
    ,
    {0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
    ,
    {0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
    ,
    {0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
    ,
    {0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
    ,
    {0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
    ,
    {0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
    ,
    {0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
    ,
    {0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
    ,
    {0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
    ,
    {0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
    ,
    {0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
    ,
    {0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
    ,
    {0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
    ,
    {0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
    ,
    {0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
    ,
    {0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
    ,
    {0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
    ,
    {0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
    ,
    {0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
    ,
    {0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
    ,
    {0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
    ,
    {0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
    ,
    {0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
    ,
    {0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
    ,
    {0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
    ,
    {0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
    ,
    {0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
    ,
    {0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
    ,
    {0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
    ,
    {0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
    ,
    {0x02, 0x04, 0x08, 0x10, 0x20} // 5c ^
    ,
    {0x00, 0x41, 0x41, 0x7f, 0x00} // 5d _
    ,
    {0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
    ,
    {0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
    ,
    {0x00, 0x01, 0x02, 0x04, 0x00} //a
    ,
    {0x20, 0x54, 0x54, 0x54, 0x78} // 61 b
    ,
    {0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
    ,
    {0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
    ,
    {0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
    ,
    {0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
    ,
    {0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
    ,
    {0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
    ,
    {0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
    ,
    {0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
    ,
    {0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
    ,
    {0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
    ,
    {0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
    ,
    {0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
    ,
    {0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
    ,
    {0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
    ,
    {0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
    ,
    {0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
    ,
    {0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
    ,
    {0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
    ,
    {0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
    ,
    {0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
    ,
    {0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
    ,
    {0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
    ,
    {0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
    ,
    {0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
    ,
    {0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
    ,
    {0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
    ,
    {0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
    ,
    {0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
    ,
    {0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
    ,
    {0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
};

char buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
Servo motor0;
Servo motor1;
Servo motor2;
Servo motor3;
int pwmCount = 0;
char inport_c = '`';
uint8_t stat_alm = 0;
char stat001 = '`';
String str1 = "a";
String str2 = "b";
String str3 = "c";
String str4 = "d";
int prev1 = 0;
int prev2 = 0;
int prev3 = 0;
int prev4 = 0;

PWM ch1(2); // Setup pin 2 for input
PWM ch2(3); // Setup pin 3 for input
PWM ch3(18); // Setup pin 18 for input
PWM ch4(19); // Setup pin 19 for input

void setup(void)
{

    LCDInit(); //Init the LCD
    Serial.begin(9600);
    Serial3.begin(9600);
    pinMode(A4, OUTPUT); //lcd back light
    analogWrite(A4, 600);
    pinMode(A0, OUTPUT); //en/chpd pin of esp8266
    analogWrite(A0, 600);
    pinMode(A1, OUTPUT); //rst of esp8266
    analogWrite(A1, 600);
    PCintPort::attachInterrupt(A8, &rising, RISING);
    pinMode(A3, OUTPUT);
    analogWrite(A3, 600); //en pin of node mcu
    pinMode(A2, OUTPUT);
    analogWrite(A2, 600); //rst pin of node mcu
    pinMode(32, INPUT);
    pinMode(33, INPUT);
    pinMode(34, INPUT);
    pinMode(35, INPUT);
    pinMode(36, INPUT);
    pinMode(37, INPUT);
    pinMode(38, INPUT);
    pinMode(39, INPUT);

    pinMode(FLCpin1, OUTPUT); //flc
    pinMode(FLCpin2, OUTPUT); //flc
    digitalWrite(FLCpin1, HIGH);
    digitalWrite(FLCpin2, HIGH);
    digitalWrite(FLCpin1, LOW);
    digitalWrite(FLCpin2, LOW);

    motor0.attach(PWMout0);
    motor1.attach(PWMout1);
    motor2.attach(PWMout2);
    motor3.attach(PWMout3);

    ch1.begin(true); // ch1 on pin 2 reading PWM HIGH duration
    ch2.begin(true); // ch2 on pin 3 reading PWM HIGH duration
    ch3.begin(true); // ch3 on pin 18 reading PWM HIGH duration
    ch4.begin(true); // ch4 on pin 19 reading PWM HIGH duration
}
char val;

void loop(void)
{
    Serial.print("ch1="+String(ch1.getValue()));
    Serial.print("\n");
    Serial.print("ch2="+String(ch2.getValue()));
    Serial.print("\n");
    Serial.print("ch3="+String(ch3.getValue()));
    Serial.print("\n");
    Serial.print("ch4="+String(ch4.getValue()));
    Serial.print("\n");
}
void gotoXY(int x, int y)
{
    LCDWrite(0, 0x80 | x);
    LCDWrite(0, 0x40 | y);
}
void LCDBitmap(char my_array[])
{
    for (int index = 0; index < (LCD_X * LCD_Y / 8); index++)
        LCDWrite(LCD_DATA, my_array[index]);
}
void LCDCharacter(char character)
{
    LCDWrite(LCD_DATA, 0x00);
    for (int index = 0; index < 5; index++)
        LCDWrite(LCD_DATA, ASCII[character - 0x20][index]);
    LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding
}
void LCDString(char *characters)
{
    while (*characters)
        LCDCharacter(*characters++);
}
void LCDClear(void)
{
    for (int index = 0; index < (LCD_X * LCD_Y / 8); index++)

        LCDWrite(LCD_DATA, 0x00);

    gotoXY(0, 0);
}
void LCDInit(void)
{
    //Configure control pins
    pinMode(PIN_SCE, OUTPUT);
    pinMode(PIN_RESET, OUTPUT);
    pinMode(PIN_DC, OUTPUT);
    pinMode(PIN_SDIN, OUTPUT);
    pinMode(PIN_SCLK, OUTPUT);
    digitalWrite(PIN_RESET, LOW);
    digitalWrite(PIN_RESET, HIGH);
    LCDWrite(LCD_COMMAND, 0x21);
    LCDWrite(LCD_COMMAND, 0xB0);
    LCDWrite(LCD_COMMAND, 0x04);
    LCDWrite(LCD_COMMAND, 0x14);
    LCDWrite(LCD_COMMAND, 0x20);
    LCDWrite(LCD_COMMAND, 0x0C);
}
void LCDWrite(byte data_or_command, byte data)
{
    digitalWrite(PIN_DC, data_or_command);
    digitalWrite(PIN_SCE, LOW);
    shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
    digitalWrite(PIN_SCE, HIGH);
}
void LCD_string(String strin)
{
    String str = strin;
    int str_len = str.length() + 1;
    char char_array[str_len];
    str.toCharArray(char_array, str_len);
    LCDString(char_array);
}

//int lcdclear = 0;
void rising()
{
    //lcdclear++;
    //if(lcdclear==6){LCDClear();lcdclear=0;}

    //LCD_string(String(digitalRead(32)) + String(digitalRead(33)) + String(digitalRead(34)) + String(digitalRead(35)) + String(digitalRead(36)) + String(digitalRead(37)) + String(digitalRead(38)) + String(digitalRead(39)));
    char inport_c =
        ((digitalRead(39) == HIGH) ? 1 : 0) +
        ((digitalRead(38) == HIGH) ? 1 : 0) * 2 +
        ((digitalRead(37) == HIGH) ? 1 : 0) * 4 +
        ((digitalRead(36) == HIGH) ? 1 : 0) * 8 +
        ((digitalRead(35) == HIGH) ? 1 : 0) * 16 +
        ((digitalRead(34) == HIGH) ? 1 : 0) * 32 +
        ((digitalRead(33) == HIGH) ? 1 : 0) * 64 +
        ((digitalRead(32) == HIGH) ? 1 : 0) * 128;

    //Serial.print(inport_c);

    //LCD_string(String(inport_c));
    //Serial.print("~~~");

    if (stat_alm == 1)
    {

        stat001 = inport_c;
        stat_alm = 2;
    }
    else if (stat_alm == 2)
    {

        if (stat001 == 'T' && inport_c == 'T')
        {
            digitalWrite(FLCpin1, HIGH);
            digitalWrite(FLCpin2, HIGH);
        }
        if (stat001 == 'S' && inport_c == 'B')
        {

            digitalWrite(FLCpin1, LOW);
            digitalWrite(FLCpin2, LOW);
        }

        if (stat001 == 'S' && inport_c == 'T')
        {

            digitalWrite(FLCpin1, LOW);
            digitalWrite(FLCpin2, HIGH);
        }

        if (stat001 == 'M' && inport_c == 'S')
        {

            digitalWrite(FLCpin1, HIGH);
            digitalWrite(FLCpin2, LOW);
        }
        stat_alm = 0;
    }
    if (inport_c >= 0 && inport_c <= 126)
    {
        pwmCount++;
        //Serial.println("!!");
    }
    buf[pwmCount] = inport_c;

    if (inport_c == '$')
    {

        stat_alm = 1;
        pwmCount = 0;
    }
    else
    {
    }

    if (inport_c == '~')
    {
        str1 = String(buf[9]) + String(buf[10]) + String(buf[11]) + String(buf[12]) + String(buf[13]);
        str2 = String(buf[14]) + String(buf[15]) + String(buf[16]) + String(buf[17]) + String(buf[18]);
        str3 = String(buf[19]) + String(buf[20]) + String(buf[21]) + String(buf[22]) + String(buf[23]);
        str4 = String(buf[24]) + String(buf[25]) + String(buf[26]) + String(buf[27]) + String(buf[28]);
        prev1 = (str1.toInt() > 999 && str1.toInt() < 2001) ? str1.toInt() : prev1;
        prev2 = (str2.toInt() > 999 && str2.toInt() < 2001) ? str2.toInt() : prev2;
        prev3 = (str3.toInt() > 999 && str3.toInt() < 2001) ? str3.toInt() : prev3;
        prev4 = (str4.toInt() > 999 && str4.toInt() < 2001) ? str4.toInt() : prev4;
        motor0.writeMicroseconds(prev1);
        motor1.writeMicroseconds(prev2);
        motor2.writeMicroseconds(prev3);
        motor3.writeMicroseconds(prev4);

        //Serial.println("!!");
        LCD_string(String(prev1) + "," + String(prev2) + "," + String(prev3) + "," + String(prev4) + ";");
        //Serial.println(str1 + "," + str2 + "," + str3 + "," + str4);
        
    }
    else
    {
    }
}
