#include <Arduino.h>
#include <U8g2lib.h>
#include <AHT20.h>
#include <Adafruit_NeoPixel.h>
#include "Thermometer.h"
#include "Fan.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// 关闭系统的引脚
#define OFF_PIN 7

// 风扇电机使用的频道
#define FAN_CHANNEL 0
// 风扇引脚
#define FAN_PIN_A 8
#define FAN_PIN_B 9

// OLED使用的引脚
#define OLED_SCL_PIN 12
#define OLED_SDA_PIN 13

// 按钮使用的引脚
#define BUTTON1_PIN 5
#define BUTTON2_PIN 6

// RGBLED使用的引脚
#define RGBLED_PIN 14

// AHT20使用的引脚
#define AHT20_SCL_PIN 10
#define AHT20_SDA_PIN 11

// AHT20的默认校准量
#define DEFAULT_TEMPERATURE_OFFSET -2.0
#define DEFAULT_HUMIDITY_OFFSET -4.0

U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

AHT20 aht20;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, RGBLED_PIN, NEO_GRB + NEO_KHZ800);

Thermometer thermometer(&aht20, DEFAULT_TEMPERATURE_OFFSET, DEFAULT_HUMIDITY_OFFSET);
Fan fan(FAN_CHANNEL, FAN_PIN_A, FAN_PIN_B, FAN_PIN_B, 39000, 10, 990, 1000, 1012, 1024);

int i = 10;
int fanSpeed = 1024;
int buttonTimer = 0;

void taskFunction(void *parameter);
uint32_t Wheel(byte WheelPos);

void setup()
{
    Serial.begin(115200);

    pinMode(OFF_PIN, OUTPUT_OPEN_DRAIN);
    digitalWrite(OFF_PIN, LOW);
    delay(2000);
    digitalWrite(OFF_PIN, HIGH);

    fan.setup();

    pinMode(BUTTON1_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON2_PIN, INPUT_PULLDOWN);

    // thermometer
    Wire.begin(AHT20_SDA_PIN, AHT20_SCL_PIN);
    thermometer.setup();

    Wire1.begin(OLED_SDA_PIN, OLED_SCL_PIN);

    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    // u8g2.setFontPosTop();
    u8g2.enableUTF8Print();
    u8g2.begin();

    pixels.begin();

    xTaskCreatePinnedToCore(taskFunction, "Task", 2048, NULL, 2, NULL, 1);
}

void taskFunction(void *parameter)
{
    for (;;)
    {
        uint32_t j;
        for (j = 0; j < 256; j++)
        {
            pixels.setPixelColor(0, Wheel(j & 255));
            pixels.show();
            vTaskDelay(100);
        }
        vTaskDelay(100);
    }
}

void loop()
{
    thermometer.loop();
    fan.loop();

    if (digitalRead(BUTTON1_PIN) == LOW)
    {
        if (millis() - buttonTimer >= 500)
        {
            buttonTimer = millis();
            fan.nextLevel();
        }
    }
    else if (digitalRead(BUTTON2_PIN) == LOW)
    {

        digitalWrite(OFF_PIN, LOW);
        delay(1000);
    }

    u8g2.firstPage();
    do
    {
        if (i < u8g2.getWidth())
        {
            i = i + fan.getLevel();
        }
        else
        {
            i = 0;
        }

        if (fan.getLevel() > 0)
        {
            u8g2.drawHLine(0, u8g2.getHeight() - 1, i);
        }

        char words[20];
        sprintf(words, "Level: %d", fan.getLevel());
        u8g2.setFont(u8g2_font_8x13B_tf);
        u8g2.drawStr(0, 24, words);

        u8g2.setFont(u8g2_font_7x13_tf);
        sprintf(words, "%.1fC", thermometer.getTemperature());
        u8g2.drawStr(0, 54, words);
        sprintf(words, "%.1fRH", thermometer.getHumidity());
        u8g2.drawStr(50, 54, words);
    } while (u8g2.nextPage());
}

uint32_t Wheel(byte WheelPos)
{
    if (WheelPos < 85)
    {
        return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}