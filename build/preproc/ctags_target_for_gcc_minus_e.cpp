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
uint8_t displaybuf[192 * 16 / 8];
Buffer buff(displaybuf, 192, 16);
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
int stateTime = 0;
int target[3] = {0, 0, 0};
byte ha, hb, hc, ma, mb, mc = 0;
char buffer[3][10];

# 63 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 2

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
    display.begin(displaybuf, 192, 16);
    Timer1.initialize(800); // 100
    Timer1.attachInterrupt(refresh);
    TimerTwo::getInstance().init(800u, backgroundTask);
    TimerTwo::getInstance().start();
    getCurrentShift();
    display.setBrightness(200); // low brightness to save energy
    buff.clear(); // clear display led matrix
    // buff.bitmap(128, 0, 64, 16, logo, 1); // bitmap logo
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

// 1 2 3 4 5
void displayNumber(int x, int y)
{
    char c[8];
    itoa(x, c, 10);
    ronn.setFont(B_7SEGMENT);
    if (x <= 9)
    {
        ronn.printText(c, y + 64 - 15, 0);
    }
    else if (x <= 99)
    {
        ronn.printText(c, y + 64 - 26, 0);
    }
    else if (x <= 999)
    {
        ronn.printText(c, y + 64 - 37, 0);
    }
    else if (x <= 9999)
    {
        ronn.printText(c, y + 64 - 48, 0);
    }
    else
    {
        ronn.printText(c, y + 64 - 59, 0);
    }
}

void getTextDisplay()
{
    displayNumber(target[currentShift], 64);
    displayNumber(counter, 0);
}

void saveTarget(void)
{
    for (int i = 0; i < 3; i++)
    {
        int num = i2dig(10) * 10000 + i2dig(10) * 100 + i2dig(10);
        EEPROM.put(eeadrTarget[i], num);
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 148 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 148 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       "\nInput new target with value : "
# 148 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 148 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(num); };
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 149 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 149 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       " | on address : "
# 149 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 149 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
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
# 159 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 159 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       "\nInput new shift time with value : "
# 159 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 159 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(num); };
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 160 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 160 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       " | on address : "
# 160 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 160 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(eeadrShiftTime[i]); };
    }
}

/**

 * @brief Manual

 *

 */
# 168 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void usage(void)
{
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 170 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 170 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n"
# 170 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 170 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 171 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 171 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n/********************************************************"
# 171 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 171 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 172 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 172 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n * [XJX TECH | 0856 9439 9449 | agusbasari29@gmail.com] *"
# 172 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 172 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 173 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 173 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n ********************************************************/"
# 173 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 173 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 174 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 174 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n? ======================== help - this message"
# 174 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 174 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 175 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 175 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ntw yyyymmdd hhmmss dw ==== write the current date, time and day of week (1-7)"
# 175 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 175 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 176 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 176 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ntr ======================= read the current time"
# 176 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 176 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 177 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 177 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ni ======================== init all data"
# 177 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 177 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 178 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 178 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ne ======================== get eeprom data"
# 178 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 178 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 179 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 179 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nce nnnnnn ================ edit counter value"
# 179 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 179 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 180 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 180 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\ncr ======================= reset counter to 0."
# 180 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 180 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 181 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 181 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nst aaaaaa bbbbbb cccccc == set target for each shift"
# 181 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 181 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 182 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 182 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nss hhmm hhmm hhmm ======== set shift time"
# 182 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 182 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
}

/**

 * @brief Button event handler

 *

 */
# 189 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void button()
{
    if (safeButton == 10)
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
# 223 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
char readNext()
{
    char c;
    do
    {
        while (!Serial.available())
        {
            char clock[8];
            if (state == 0)
            {
                ronn.setFont(B_STD);
                ronn.scanText_R(shiftText[currentShift], 1 + 128, 1);
                delay(500);
                state++;
            }
            else if (state > 0 && state < 7)
            {
                delay(500);
                state++;
            }
            else if (state == 7)
            {
                ronn.clear_D(65 + 64, 0, 64, 16);
                state++;
                delay(500);
            }
            else if (state == 8)
            {
                RTC.readTime();
                ronn.setFont(B_7SEGMENT);
                strcpy(&clock[0], p2dig(RTC.h, 10));
                strcpy(&clock[2], ":");
                strcpy(&clock[3], p2dig(RTC.m, 10));
                ronn.scrollText_D(clock, 9 + 128, 0, 64 - 11);
                delay(500);
                state++;
            }
            else if (state > 8 && state < 20)
            {
                RTC.readTime();
                ronn.setFont(B_7SEGMENT);
                String s;
                s = p2dig(RTC.h, 10);
                s += ":";
                s += p2dig(RTC.m, 10);
                ronn.printText(s, 9 + 128, 0);
                // delay(3);
                if (stateTime == 0)
                {
                    buff.fillRect(31 + 128, 0, 2, 12, 0);
                    delay(500);
                    RTC.readTime();
                    uint8_t h = RTC.h;
                    uint8_t m = RTC.m;
                    if (RTC.m != m)
                    {
                        char newM[3];
                        m = RTC.m;
                        strcpy(newM, p2dig(RTC.m, 10));
                        strcpy(&clock[3], p2dig(RTC.m, 10));
                        ronn.scrollText_D(newM, 45 - 11 + 128, 0, 21);
                    }
                    else if (RTC.h != h)
                    {
                        char newH[3];
                        h = RTC.h;
                        strcpy(newH, p2dig(RTC.m, 10));
                        strcpy(&clock[0], p2dig(RTC.m, 10));
                        ronn.scrollText_D(newH, 9 + 128, 0, 21);
                    }
                    stateTime = 1;
                }
                else
                {
                    ronn.printText(":", 31 + 128, 0);
                    delay(500);
                    RTC.readTime();
                    uint8_t h = RTC.h;
                    uint8_t m = RTC.m;
                    if (RTC.m != m)
                    {
                        char newM[3];
                        m = RTC.m;
                        strcpy(newM, p2dig(RTC.m, 10));
                        strcpy(&clock[3], p2dig(RTC.m, 10));
                        ronn.scrollText_D(newM, 45 - 11 + 128, 0, 21);
                    }
                    else if (RTC.h != h)
                    {
                        char newH[3];
                        h = RTC.h;
                        strcpy(newH, p2dig(RTC.m, 10));
                        strcpy(&clock[0], p2dig(RTC.m, 10));
                        ronn.scrollText_D(newH, 9 + 128, 0, 21);
                    }
                    stateTime = 0;
                }
                state++;
            }
            else if (state == 20)
            {
                ronn.clear_R(65 + 64, 0, 64, 16);
                delay(500);
                state = 0;
            }
            getTextDisplay();
            getCurrentShift();
            ;
        }
        c = Serial.read();
    } while (isspace(c));
    return (c);
}

void console()
{
    char c;
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 340 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 340 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n"
# 340 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 340 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
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
# 402 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 402 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       "\nBad parameter '"
# 402 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 402 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(c); };
        Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 403 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 403 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       "'"
# 403 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 403 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
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
# 415 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void setEeprom(void)
{
    target[0] = 700;
    EEPROM.put(eeadrTarget[0], target[0]);
    delay(10);
    target[1] = 600;
    EEPROM.put(eeadrTarget[1], target[1]);
    delay(10);
    target[2] = 500;
    EEPROM.put(eeadrTarget[2], target[2]);
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
# 443 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 443 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nSuccessfully set data to EEPROM"
# 443 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 443 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    getEeprom();
}

void getEeprom(void)
{
    EEPROM.get(eeadrTarget[0], target[0]);
    EEPROM.get(eeadrTarget[1], target[1]);
    EEPROM.get(eeadrTarget[2], target[2]);
    EEPROM.get(eeadrShiftTime[0], ha);
    EEPROM.get(eeadrShiftTime[1], ma);
    EEPROM.get(eeadrShiftTime[2], hb);
    EEPROM.get(eeadrShiftTime[3], mb);
    EEPROM.get(eeadrShiftTime[4], hc);
    EEPROM.get(eeadrShiftTime[5], mc);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 458 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 458 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nSuccessfully get data from EEPROM"
# 458 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 458 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 459 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 459 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTarget Shift 1 : "
# 459 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 459 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(target[0]); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 460 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 460 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTarget Shift 2 : "
# 460 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 460 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(target[1]); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 461 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 461 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTarget Shift 3 : "
# 461 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 461 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(target[2]); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 462 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 462 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 1 hour : "
# 462 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 462 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(ha); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 463 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 463 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 1 minute : "
# 463 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 463 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(ma); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 464 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 464 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 2 hour : "
# 464 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 464 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(hb); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 465 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 465 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 2 minute : "
# 465 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 465 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(mb); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 466 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 466 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 3 hour : "
# 466 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 466 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(hc); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 467 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 467 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime Shift 3 minute : "
# 467 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 467 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
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
# 478 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 478 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           "\nEeprom is empty in address 0x"
# 478 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           ); &__c[0];}))
# 478 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ))); Serial.print(i * 32); };
            { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 479 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 479 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           " : "
# 479 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           ); &__c[0];}))
# 479 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ))); Serial.print(check); };
            return false;
        }
        else
        {
            { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 484 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 484 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           "\n Eeprom is exists in address 0x%"
# 484 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           ); &__c[0];}))
# 484 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ))); Serial.print(i * 32); };
            { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 485 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 485 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ":"
# 485 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
           ); &__c[0];}))
# 485 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
           ))); Serial.print(check); };
            return true;
        }
    }
}

/**

 * @brief Counter

 *

 */
# 495 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void addCounter(void)
{
    counter++;
    itoa(counter, buffer[2], 10);
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 499 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 499 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nCounter : "
# 499 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 499 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(counter); };
}

void resetCounter(void)
{
    counter = 0;
    itoa(counter, buffer[2], 10);
    ronn.clear_D(0, 0, 64, 16);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 507 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 507 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nReset Counter to 0."
# 507 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 507 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 508 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 508 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n"
# 508 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 508 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
}

void editCounter(void)
{
    counter = i2dig(10) * 10000 + i2dig(10) * 100 + i2dig(10);
    itoa(counter, buffer[2], 10);
    // P.setTextBuffer(2, buffer[2]);
    // P.displayReset(2);
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 517 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 517 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nEdit Counter to "
# 517 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 517 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(counter); };
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 518 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 518 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\n"
# 518 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 518 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
}

/**

 * @brief Get the Current Shift object

 *

 */
# 525 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
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

/**

 * @brief Tools

 *

 */
# 598 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
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
# 668 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
void showTime()
{
    RTC.readTime();
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 671 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 671 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "\nTime - "
# 671 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 671 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   )));
    printTime();
}

void printTime()
{
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 677 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 677 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ""
# 677 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 677 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(RTC.yyyy); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 678 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 678 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "-"
# 678 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 678 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.mm, 10)); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 679 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 679 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   "-"
# 679 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 679 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.dd, 10)); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 680 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 680 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   " "
# 680 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 680 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.h, 10)); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 681 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 681 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ":"
# 681 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 681 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.m, 10)); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 682 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 682 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ":"
# 682 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 682 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ))); Serial.print(p2dig(RTC.s, 10)); };
    if (RTC.status(DS3231_12H) == DS3231_ON)
        { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 684 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 684 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ""
# 684 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
       ); &__c[0];}))
# 684 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
       ))); Serial.print(RTC.pm ? " pm" : " am"); };
    { Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 685 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 685 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
   ""
# 685 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino" 3
   ); &__c[0];}))
# 685 "/home/jabrix/projects/Arduino/Projects/Project 01 rev (Display Counter Prod)/arduino-project-display-01-rev01/sketch.ino"
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
