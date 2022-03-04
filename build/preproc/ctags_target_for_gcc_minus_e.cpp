# 1 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
/**

 * @file sketch.ino

 * @author agus basari (agusbasari29@gmail.com)

 * @brief

 * @version 0.1

 * @date 2022-03-03

 *

 * @copyright Copyright (c) 2022

 *

 */
# 12 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
# 13 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 14 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 15 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 16 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 17 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 18 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 19 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 20 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 21 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2
# 35 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
HUB08SPI display;
uint8_t displaybuf[64 * 16 / 8];
Buffer buff(displaybuf, 64, 16);
const char shiftText[3][10] =
    {
        "SHIFT 1",
        "SHIFT 2",
        "SHIFT 3",
};
const int eeadrTarget[] = {0, 20, 40};
const int eeadrShiftTime[] = {60, 61, 62, 63, 64, 65};
const byte counterBtn = 8;
const byte resetBtn = 12;
int currentShift = 0;
int counter = 0;
int buttonState = 0;
int lastButtonState = 0;
int resetButton = 0;
int lastButtonReset = 0;
int safeButton = 0;
int skipComm = 0;
int state = 0;
int target1, target2, target3;
byte ha, hb, hc, ma, mb, mc = 0;
char buffer[3][10];

# 62 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2

void refresh()
{
    display.scan();
}

void setup()
{
    Wire.begin();
    Serial.begin(38400);
    pinMode(counterBtn, 0x2);
    pinMode(resetBtn, 0x2);
    usage();
    bool eepStatus = getEepromStatus();
    if (!eepStatus)
    {
        setEeprom();
    }
    else
    {
        getEeprom();
    }
    display.begin(displaybuf, 64, 16);
    Timer1.initialize(800); // 100
    Timer1.attachInterrupt(refresh);
    TimerTwo::getInstance().init(800u, backgroundTask);
    TimerTwo::getInstance().start();
    getCurrentShift();
    display.setBrightness(200); // low brightness to save energy
    buff.clear(); // clear display led matrix
    buff.bitmap(0, 0, 64, 16, logo, 1); // bitmap logo
    delay(2000);
    ronn.clear_A1();
}

void loop()
{
    console();
}

void backgroundTask()
{
    button();
}

void transWire()
{

    switch (currentShift)
    {
    case 0:
        sendToOther(target1, counter);
        break;
    case 1:
        sendToOther(target2, counter);
        break;
    case 2:
        sendToOther(target3, counter);
        break;
    }
}

void sendToOther(int x, int y)
{
    Wire.beginTransmission(11);
    Wire.write(x);
    Wire.endTransmission();
    Wire.beginTransmission(12);
    Wire.write(y);
    Wire.endTransmission();
}

void shift()
{
    ronn.setFont(B_STD);
    ronn.scanText_R(shiftText[currentShift], 1, 1);
    delay(5000);
    ronn.clear_D();
    // delay(500);
}

void time()
{
    ronn.setFont(B_7SEGMENT);
    char clock[8];
    uint8_t h = RTC.h;
    uint8_t m = RTC.m;
    RTC.readTime();
    strcpy(&clock[0], p2dig(RTC.h, 10));
    strcpy(&clock[2], ":");
    strcpy(&clock[3], p2dig(RTC.m, 10));
    ronn.scrollText_D(clock, 9, 0, 64 - 11);
    for (int i = 1; i <= 5; i++)
    {
        delay(400);
        buff.fillRect(31, 0, 2, 12, 0);
        delay(600);
        ronn.printText(":", 31, 0);
        RTC.readTime();
        if (RTC.m != m)
        {
            char newM[3];
            m = RTC.m;
            strcpy(newM, p2dig(RTC.m, 10));
            strcpy(&clock[3], p2dig(RTC.m, 10));
            ronn.scrollText_D(newM, 45 - 11, 0, 21);
        }
        else if (RTC.h != h)
        {
            char newH[3];
            h = RTC.h;
            strcpy(newH, p2dig(RTC.m, 10));
            strcpy(&clock[0], p2dig(RTC.m, 10));
            ronn.scrollText_D(newH, 9, 0, 21);
        }
    }
    delay(500);
    ronn.clear_R();
}

void saveTarget(void)
{
    for (int i = 0; i < 3; i++)
    {
        int num = i2dig(10) * 10000 + i2dig(10) * 100 + i2dig(10);
        EEPROM.put(eeadrTarget[i], num);
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 188 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 188 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       "\nInput new target with value : "
# 188 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 188 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(num); };
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 189 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 189 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       " | on address : "
# 189 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 189 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(eeadrTarget[i]); };
    }
}

void saveShiftTime(void)
{
    for (int i = 0; i < 6; i++)
    {
        int num = i2dig(10);
        EEPROM.put(eeadrShiftTime[i], num);
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 199 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 199 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       "\nInput new shift time with value : "
# 199 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 199 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(num); };
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 200 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 200 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       " | on address : "
# 200 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 200 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(eeadrShiftTime[i]); };
    }
}

/**

 * @brief Manual

 *

 */
# 208 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void usage(void)
{
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 210 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 210 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n"
# 210 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 210 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 211 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 211 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n/********************************************************"
# 211 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 211 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 212 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 212 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n * [XJX TECH | 0856 9439 9449 | agusbasari29@gmail.com] *"
# 212 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 212 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 213 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 213 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n ********************************************************/"
# 213 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 213 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 214 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 214 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n? ======================== help - this message"
# 214 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 214 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 215 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 215 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ntw yyyymmdd hhmmss dw ==== write the current date, time and day of week (1-7)"
# 215 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 215 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 216 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 216 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ntr ======================= read the current time"
# 216 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 216 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 217 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 217 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ni ======================== init all data"
# 217 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 217 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 218 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 218 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ne ======================== get eeprom data"
# 218 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 218 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 219 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 219 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nce nnnnnn ================ edit counter value"
# 219 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 219 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 220 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 220 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ncr ======================= reset counter to 0."
# 220 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 220 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 221 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 221 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nst aaaaaa bbbbbb cccccc == set target for each shift"
# 221 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 221 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 222 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 222 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nss hhmm hhmm hhmm ======== set shift time"
# 222 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 222 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
}

/**

 * @brief Button event handler

 *

 */
# 229 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void button()
{
    if (safeButton == 2)
    {
        buttonState = digitalRead(8);
        resetButton = digitalRead(12);
        if (buttonState != lastButtonState)
        {
            if (!buttonState == 0x1)
            {
                addCounter();
            }
            delay(500);
        }
        lastButtonState = buttonState;
        if (resetButton != lastButtonReset)
        {
            if (!resetButton == 0x1)
            {
                resetCounter();
            }
            delay(500);
        }
        lastButtonReset = resetButton;
        safeButton = 0;
    }
    safeButton++;
}

/**

 * @brief Console Program

 *

 * @return char

 */
# 263 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
char readNext()
{
    char c;
    do
    {
        while (!Serial.available())
        {
            if (state == 0)
            {
                shift();
                state = 1;
            }
            else
            {
                time();
                state = 0;
            }
            getCurrentShift();
            transWire();
        }
        c = Serial.read();
    } while (isspace(c));
    return (c);
}

void console()
{
    char c;
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 291 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 291 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n"
# 291 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 291 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    c = readNext();
    switch (toupper(c))
    {
    case '?':
        usage();
        break;
    case 'I':
        setEeprom();
        break;
    case 'E':
        getEeprom();
        break;
    case 'T':
        c = readNext();
        switch (toupper(c))
        {
        case 'R':
            showTime();
            break;
        case 'W':
            inputTime();
            RTC.writeTime();
            showTime();
            break;
        default:
            goto no_good;
        }
        break;
    case 'C':
        c = readNext();
        switch (toupper(c))
        {
        case 'R':
            resetCounter();
            break;
        case 'E':
            editCounter();
            break;
        default:
            goto no_good;
        }
        break;
    case 'S':
        c = readNext();
        switch (toupper(c))
        {
        case 'T':
            saveTarget();
            getEeprom();
            break;
        case 'S':
            saveShiftTime();
            getEeprom();
            break;
        default:
            goto no_good;
        }
        break;
    default:
    no_good:
    {
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 353 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 353 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       "\nBad parameter '"
# 353 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 353 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(c); };
        Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 354 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 354 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       "'"
# 354 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 354 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       )));
        while (Serial.available())
            c = readNext();
    }
    break;
    }
}

/**

 * @brief Set the Eeprom object

 *

 */
# 366 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void setEeprom(void)
{
    target1 = 700;
    EEPROM.put(eeadrTarget[0], target1);
    delay(10);
    target2 = 600;
    EEPROM.put(eeadrTarget[1], target2);
    delay(10);
    target3 = 500;
    EEPROM.put(eeadrTarget[2], target3);
    delay(10);
    ha = 7;
    EEPROM.put(eeadrShiftTime[0], ha);
    delay(10);
    ma = 30;
    EEPROM.put(eeadrShiftTime[1], ma);
    delay(10);
    hb = 15;
    EEPROM.put(eeadrShiftTime[2], hb);
    delay(10);
    mb = 30;
    EEPROM.put(eeadrShiftTime[3], mb);
    delay(10);
    hc = 23;
    EEPROM.put(eeadrShiftTime[4], hc);
    delay(10);
    mc = 30;
    EEPROM.put(eeadrShiftTime[5], mc);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 394 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 394 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nSuccessfully set data to EEPROM"
# 394 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 394 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    getEeprom();
}

void getEeprom(void)
{
    EEPROM.get(eeadrTarget[0], target1);
    EEPROM.get(eeadrTarget[1], target2);
    EEPROM.get(eeadrTarget[2], target3);
    EEPROM.get(eeadrShiftTime[0], ha);
    EEPROM.get(eeadrShiftTime[1], ma);
    EEPROM.get(eeadrShiftTime[2], hb);
    EEPROM.get(eeadrShiftTime[3], mb);
    EEPROM.get(eeadrShiftTime[4], hc);
    EEPROM.get(eeadrShiftTime[5], mc);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 409 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 409 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nSuccessfully get data from EEPROM"
# 409 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 409 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 410 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 410 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTarget Shift 1 : "
# 410 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 410 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(target1); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 411 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 411 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTarget Shift 2 : "
# 411 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 411 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(target2); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 412 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 412 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTarget Shift 3 : "
# 412 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 412 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(target3); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 413 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 413 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 1 hour : "
# 413 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 413 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(ha); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 414 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 414 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 1 minute : "
# 414 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 414 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(ma); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 415 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 415 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 2 hour : "
# 415 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 415 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(hb); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 416 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 416 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 2 minute : "
# 416 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 416 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(mb); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 417 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 417 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 3 hour : "
# 417 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 417 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(hc); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 418 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 418 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 3 minute : "
# 418 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 418 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(mc); };
}

bool getEepromStatus(void)
{
    for (int i = 0; i < 6; i++)
    {
        int check;
        EEPROM.get(i * 64, check);
        if (check == 0)
        {
            { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 429 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 429 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           "\nEeprom is empty in address 0x"
# 429 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           ); &__c[0];}))
# 429 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ))); Serial.print(i * 32); };
            { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 430 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 430 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           " : "
# 430 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           ); &__c[0];}))
# 430 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ))); Serial.print(check); };
            return false;
        }
        else
        {
            { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 435 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 435 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           "\n Eeprom is exists in address 0x%"
# 435 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           ); &__c[0];}))
# 435 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ))); Serial.print(i * 32); };
            { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 436 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 436 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ":"
# 436 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           ); &__c[0];}))
# 436 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ))); Serial.print(check); };
            return true;
        }
    }
}

/**

 * @brief Counter

 *

 */
# 446 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void addCounter(void)
{
    counter++;
    itoa(counter, buffer[2], 10);
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 450 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 450 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nCounter : "
# 450 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 450 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(counter); };
}

void resetCounter(void)
{
    counter = 0;
    itoa(counter, buffer[2], 10);
    // P.setTextBuffer(2, buffer[2]);
    // P.displayReset(2);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 459 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 459 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nReset Counter to 0."
# 459 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 459 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 460 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 460 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n"
# 460 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 460 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
}

void editCounter(void)
{
    counter = i2dig(10) * 10000 + i2dig(10) * 100 + i2dig(10);
    itoa(counter, buffer[2], 10);
    // P.setTextBuffer(2, buffer[2]);
    // P.displayReset(2);
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 469 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 469 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nEdit Counter to "
# 469 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 469 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(counter); };
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 470 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 470 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n"
# 470 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 470 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
}

/**

 * @brief Get the Current Shift object

 *

 */
# 477 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void getCurrentShift()
{
    RTC.readTime();

    if (RTC.h < ha)
    {
        currentShift = 2;
    }
    else if (RTC.h >= ha)
    {
        if (RTC.h == ha && RTC.m < ma)
        {
            currentShift = 2;
        }
        else if (RTC.h == ha && RTC.m == ma)
        {
            if (currentShift != 0)
            {
                resetCounter();
            }
            currentShift = 0;
        }
        else if (RTC.h < hb)
        {
            currentShift = 0;
        }
        else if (RTC.h >= hb)
        {
            if (RTC.h == hb && RTC.m < mb)
            {
                currentShift = 0;
            }
            else if (RTC.h == hb && RTC.m == mb)
            {
                if (currentShift != 1)
                {
                    resetCounter();
                }
                currentShift = 1;
            }
            else if (RTC.h < hc)
            {
                currentShift = 1;
            }
            else if (RTC.h >= hc)
            {
                if (RTC.h == hc && RTC.m < mc)
                {
                    currentShift = 1;
                }
                else if (RTC.h == hc && RTC.m == mc)
                {
                    if (currentShift != 2)
                    {
                        resetCounter();
                    }
                    currentShift = 2;
                }
                else
                {
                    currentShift = 2;
                }
            }
        }
    }
    // PRINT("\nCurrent Shift : ", currentShift);
}

void getTargetShift(void)
{
    char target[8];

    switch (currentShift)
    {
    case 0:
        itoa(target1, target, 10);
        // PRINTS("\nGet target shift 0");
        break;
    case 1:
        itoa(target2, target, 10);
        // PRINTS("\nGet target shift 1");
        break;
    case 2:
        itoa(target3, target, 10);
        // PRINTS("\nGet target shift 2");
        break;
    default:
        break;
    }
    // PRINT("\nCurrent Target : ", target);
    strcpy(buffer[1], target);
}

/**

 * @brief Tools

 *

 */
# 575 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
const char *dow2String(uint8_t code)
{
    static const char *str[] = {" ---", " Sun", " Mon", " Tue", " Wed", " Thu", " Fri", " Sat"};

    if (code > 7)
        code = 0;
    return (str[code]);
}

uint8_t htoi(char c)
{
    c = toupper(c);

    if (c >= '0' && c <= '9')
        return (c - '0');
    else if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    else
        return (0);
}

uint8_t i2dig(uint8_t mode)
{
    uint8_t v = 0;
    char c[3] = "00";

    c[0] = readNext();
    c[1] = readNext();

    switch (mode)
    {
    case 10:
        v = atoi(c);
        break;
    case 16:
        v = (htoi(c[0]) << 4) + htoi(c[1]);
        ;
        break;
    }

    return (v);
}

char *p2dig(uint8_t v, uint8_t mode)
{
    static char s[3];
    uint8_t i = 0;
    uint8_t n = 0;

    switch (mode)
    {
    case 16:
        n = 16;
        break;
    case 10:
        n = 10;
        break;
    }

    if (v < n)
        s[i++] = '0';
    itoa(v, &s[i], n);

    return (s);
}

/**

 * @brief Time

 *

 */
# 645 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void showTime()
{
    RTC.readTime();
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 648 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 648 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime - "
# 648 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 648 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    printTime();
}

void printTime()
{
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 654 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 654 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ""
# 654 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 654 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(RTC.yyyy); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 655 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 655 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "-"
# 655 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 655 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.mm, 10)); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 656 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 656 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "-"
# 656 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 656 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.dd, 10)); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 657 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 657 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   " "
# 657 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 657 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.h, 10)); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 658 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 658 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ":"
# 658 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 658 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.m, 10)); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 659 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 659 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ":"
# 659 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 659 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.s, 10)); };
    if (RTC.status(DS3231_12H) == DS3231_ON)
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 661 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 661 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ""
# 661 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 661 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(RTC.pm ? " pm" : " am"); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 662 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 662 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ""
# 662 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 662 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(dow2String(RTC.dow)); };
}

void inputTime(void)
{
    RTC.yyyy = i2dig(10) * 100 + i2dig(10);
    RTC.mm = i2dig(10);
    RTC.dd = i2dig(10);

    RTC.h = i2dig(10);
    RTC.m = i2dig(10);
    RTC.s = i2dig(10);

    RTC.dow = i2dig(10);
}
