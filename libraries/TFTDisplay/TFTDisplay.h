#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <Arduino.h>

#include <Adafruit_ILI9341.h>
#include <U8g2_for_Adafruit_GFX.h>

#define HORIZONTAL_DISTANCE_BETWEEN_CHARACTERS 0
#define VERTICAL_DISTANCE_BETWEEN_CHARACTERS 2

class TFTDisplay{
    public:
        TFTDisplay(uint16_t width, uint16_t height, Adafruit_ILI9341 *tft, U8G2_FOR_ADAFRUIT_GFX *u8g2);

        void begin();
        void clear();
        void home();

        void setCursor(uint16_t x, uint16_t y);
        void drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);

        void print(const char *strToPrint);
        bool printChar(const char *utf8Char);
        uint16_t m_lineHeight, m_width, m_height;

    private:
        Adafruit_ILI9341 *m_tft;
        U8G2_FOR_ADAFRUIT_GFX *m_u8g2;

        uint16_t m_fontAscent, m_fontDescent, m_cursorX, m_cursorY;

        int16_t _getUTF8Width(const char *utf8Char);
        int16_t _printChar(uint16_t x, uint16_t y, const char *utf8Char);
};
#endif
