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

#include <avr/pgmspace.h>
#include <MD_DS3231.h>
#include <Wire.h>
#include <HUB08SPI.h>
#include <SPI.h>
#include <EEPROM.h>
#include <TimerOne.h>
#include <TimerTwo.h>
#include <Buffer.h>

#define WIDTH 192
#define HEIGHT 16

#define I2C_SLAVE1_ADDRESS 11
#define I2C_SLAVE2_ADDRESS 12

#define PRINTS(s) Serial.print(F(s))
#define PRINT(s, v)         \
    {                       \
        Serial.print(F(s)); \
        Serial.print(v);    \
    }

HUB08SPI display;
uint8_t displaybuf[WIDTH * HEIGHT / 8];
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

#include <ronnAnimation.h>

void refresh()
{
    display.scan();
}

void setup()
{
    Wire.begin();
    Serial.begin(38400);
    pinMode(counterBtn, INPUT_PULLUP);
    pinMode(resetBtn, INPUT_PULLUP);
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
    display.begin(displaybuf, WIDTH, HEIGHT);
    Timer1.initialize(800); // 100
    Timer1.attachInterrupt(refresh);
    Timer2.init(800u, backgroundTask);
    Timer2.start();
    getCurrentShift();
    display.setBrightness(200); // low brightness to save energy
    buff.clear();               // clear display led matrix
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
        int num = i2dig(DEC) * 10000 + i2dig(DEC) * 100 + i2dig(DEC);
        EEPROM.put(eeadrTarget[i], num);
        PRINT("\nInput new target with value : ", num);
        PRINT(" | on address : ", eeadrTarget[i]);
    }
}

void saveShiftTime(void)
{
    for (int i = 0; i < 6; i++)
    {
        int num = i2dig(DEC);
        EEPROM.put(eeadrShiftTime[i], num);
        PRINT("\nInput new shift time with value : ", num);
        PRINT(" | on address : ", eeadrShiftTime[i]);
    }
}

/**
 * @brief Manual
 *
 */
void usage(void)
{
    PRINTS("\n");
    PRINTS("\n/********************************************************");
    PRINTS("\n * [XJX TECH | 0856 9439 9449 | agusbasari29@gmail.com] *");
    PRINTS("\n ********************************************************/");
    PRINTS("\n? ======================== help - this message");
    PRINTS("\ntw yyyymmdd hhmmss dw ==== write the current date, time and day of week (1-7)");
    PRINTS("\ntr ======================= read the current time");
    PRINTS("\ni ======================== init all data");
    PRINTS("\ne ======================== get eeprom data");
    PRINTS("\nce nnnnnn ================ edit counter value");
    PRINTS("\ncr ======================= reset counter to 0.");
    PRINTS("\nst aaaaaa bbbbbb cccccc == set target for each shift");
    PRINTS("\nss hhmm hhmm hhmm ======== set shift time");
}

/**
 * @brief Button event handler
 *
 */
void button()
{
    if (safeButton == 10)
    {
        buttonState = digitalRead(8);
        resetButton = digitalRead(12);
        if (buttonState != lastButtonState)
        {
            if (!buttonState == HIGH)
            {
                addCounter();
            }
            delay(500);
        }
        lastButtonState = buttonState;
        if (resetButton != lastButtonReset)
        {
            if (!resetButton == HIGH)
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
                strcpy(&clock[0], p2dig(RTC.h, DEC));
                strcpy(&clock[2], ":");
                strcpy(&clock[3], p2dig(RTC.m, DEC));
                ronn.scrollText_D(clock, 9 + 128, 0, 64 - 11);
                delay(500);
                state++;
            }
            else if (state > 8 && state < 20)
            {
                RTC.readTime();
                ronn.setFont(B_7SEGMENT);
                String s;
                s = p2dig(RTC.h, DEC);
                s += ":";
                s += p2dig(RTC.m, DEC);
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
                        strcpy(newM, p2dig(RTC.m, DEC));
                        strcpy(&clock[3], p2dig(RTC.m, DEC));
                        ronn.scrollText_D(newM, 45 - 11 + 128, 0, 21);
                    }
                    else if (RTC.h != h)
                    {
                        char newH[3];
                        h = RTC.h;
                        strcpy(newH, p2dig(RTC.m, DEC));
                        strcpy(&clock[0], p2dig(RTC.m, DEC));
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
                        strcpy(newM, p2dig(RTC.m, DEC));
                        strcpy(&clock[3], p2dig(RTC.m, DEC));
                        ronn.scrollText_D(newM, 45 - 11 + 128, 0, 21);
                    }
                    else if (RTC.h != h)
                    {
                        char newH[3];
                        h = RTC.h;
                        strcpy(newH, p2dig(RTC.m, DEC));
                        strcpy(&clock[0], p2dig(RTC.m, DEC));
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
    PRINTS("\n");
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
        PRINT("\nBad parameter '", c);
        PRINTS("'");
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
    PRINTS("\nSuccessfully set data to EEPROM");
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
    PRINTS("\nSuccessfully get data from EEPROM");
    PRINT("\nTarget Shift 1 : ", target[0]);
    PRINT("\nTarget Shift 2 : ", target[1]);
    PRINT("\nTarget Shift 3 : ", target[2]);
    PRINT("\nTime Shift 1 hour : ", ha);
    PRINT("\nTime Shift 1 minute : ", ma);
    PRINT("\nTime Shift 2 hour : ", hb);
    PRINT("\nTime Shift 2 minute : ", mb);
    PRINT("\nTime Shift 3 hour : ", hc);
    PRINT("\nTime Shift 3 minute : ", mc);
}

bool getEepromStatus(void)
{
    for (int i = 0; i < 6; i++)
    {
        int check;
        EEPROM.get(i * 64, check);
        if (check == 0)
        {
            PRINT("\nEeprom is empty in address 0x", i * 32);
            PRINT(" : ", check);
            return false;
        }
        else
        {
            PRINT("\n Eeprom is exists in address 0x%", i * 32);
            PRINT(":", check);
            return true;
        }
    }
}

/**
 * @brief Counter
 *
 */
void addCounter(void)
{
    counter++;
    itoa(counter, buffer[2], 10);
    PRINT("\nCounter : ", counter);
}

void resetCounter(void)
{
    counter = 0;
    itoa(counter, buffer[2], 10);
    ronn.clear_D(0, 0, 64, 16);
    PRINTS("\nReset Counter to 0.");
    PRINTS("\n");
}

void editCounter(void)
{
    counter = i2dig(DEC) * 10000 + i2dig(DEC) * 100 + i2dig(DEC);
    itoa(counter, buffer[2], 10);
    // P.setTextBuffer(2, buffer[2]);
    // P.displayReset(2);
    PRINT("\nEdit Counter to ", counter);
    PRINTS("\n");
}

/**
 * @brief Get the Current Shift object
 *
 */
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
    case DEC:
        v = atoi(c);
        break;
    case HEX:
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
    case HEX:
        n = 16;
        break;
    case DEC:
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
void showTime()
{
    RTC.readTime();
    PRINTS("\nTime - ");
    printTime();
}

void printTime()
{
    PRINT("", RTC.yyyy);
    PRINT("-", p2dig(RTC.mm, DEC));
    PRINT("-", p2dig(RTC.dd, DEC));
    PRINT(" ", p2dig(RTC.h, DEC));
    PRINT(":", p2dig(RTC.m, DEC));
    PRINT(":", p2dig(RTC.s, DEC));
    if (RTC.status(DS3231_12H) == DS3231_ON)
        PRINT("", RTC.pm ? " pm" : " am");
    PRINT("", dow2String(RTC.dow));
}

void inputTime(void)
{
    RTC.yyyy = i2dig(DEC) * 100 + i2dig(DEC);
    RTC.mm = i2dig(DEC);
    RTC.dd = i2dig(DEC);

    RTC.h = i2dig(DEC);
    RTC.m = i2dig(DEC);
    RTC.s = i2dig(DEC);

    RTC.dow = i2dig(DEC);
}