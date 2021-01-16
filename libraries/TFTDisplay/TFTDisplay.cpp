#include "TFTDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <U8g2_for_Adafruit_GFX.h>

TFTDisplay::TFTDisplay(uint16_t width, uint16_t height, Adafruit_ILI9341 *tft, U8G2_FOR_ADAFRUIT_GFX *u8g2)
{
    m_width = width;
    m_height = height;

    m_tft = tft;
    m_u8g2 = u8g2;
}

void TFTDisplay::begin(){
    m_tft -> begin();
    m_u8g2 -> begin(*m_tft);

    m_u8g2 -> setFontMode(1); // use u8g2 transparent mode (this is default)
    m_u8g2 -> setFontDirection(0); // left to right (this is default)
    m_u8g2 -> setForegroundColor(ILI9341_WHITE); // apply Adafruit GFX color
    m_u8g2 -> setFont(u8g2_font_cu12_t_cyrillic); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

    m_fontAscent = m_u8g2 -> getFontAscent();
    m_fontDescent = m_u8g2 -> getFontDescent();
    m_lineHeight = m_fontAscent - m_fontDescent + VERTICAL_DISTANCE_BETWEEN_CHARACTERS;
    

    clear();
    home();
}

void TFTDisplay::clear(){
    m_tft -> fillScreen(ILI9341_BLACK);
}

void TFTDisplay::drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color){
    m_tft -> drawRoundRect(x0, y0, w, h, radius, color);
}

void TFTDisplay::home(){
    setCursor(0, m_fontAscent);
}

void TFTDisplay::setCursor(uint16_t x, uint16_t y){
    m_cursorX = x;
    m_cursorY = y;
}


void TFTDisplay::print(const char *strToPrint)
{
    uint8_t currentByte;
    uint8_t tempCharBuf[5];
    memset(tempCharBuf, 0x00, 5);
    
    while(true){
        currentByte = (uint8_t)*strToPrint;

        if (currentByte == 0x00){
            break;
        }

        if ((currentByte & 0x80) == 0){
            tempCharBuf[0] = currentByte;
            strToPrint++;

        } else if ((currentByte & 0xE0) == 0xC0) {
            tempCharBuf[0] = currentByte;
            strToPrint++;
            tempCharBuf[1] = (uint8_t)*strToPrint;
            strToPrint++;

        } else if ((currentByte & 0xF0) == 0xE0) {
            tempCharBuf[0] = currentByte;
            strToPrint++;
            tempCharBuf[1] = (uint8_t)*strToPrint;
            strToPrint++;
            tempCharBuf[2] = (uint8_t)*strToPrint;
            strToPrint++;

        } else if ((currentByte & 0xF8) == 0xF0) {
            tempCharBuf[0] = currentByte;
            strToPrint++;
            tempCharBuf[1] = (uint8_t)*strToPrint;
            strToPrint++;
            tempCharBuf[2] = (uint8_t)*strToPrint;
            strToPrint++;
            tempCharBuf[3] = (uint8_t)*strToPrint;
            strToPrint++;
        }

        printChar((const char*)tempCharBuf);
        memset(tempCharBuf, 0x00, 5);     
    }
}

bool TFTDisplay::printChar(const char *utf8Char){
    uint8_t charWidth = _getUTF8Width(utf8Char);

    if ((m_cursorX + charWidth <= m_width) && (m_cursorY <= m_height))
    {
        _printChar(m_cursorX, m_cursorY, utf8Char);
        m_cursorX += charWidth + HORIZONTAL_DISTANCE_BETWEEN_CHARACTERS;
        return true;
    } 

    else 
    {
        // Shift cursor to new row if it's possible
        if (m_cursorY + m_lineHeight <= m_height) {
            m_cursorX = 0;
            m_cursorY += m_lineHeight;

            _printChar(m_cursorX, m_cursorY, utf8Char);
            m_cursorX += charWidth + HORIZONTAL_DISTANCE_BETWEEN_CHARACTERS;
            
            return true;
        } else {
            return false;
        }
    }

    return false;
}

int16_t TFTDisplay::_getUTF8Width(const char *utf8Char){
    if ((uint8_t) *utf8Char == 0x0A){
        return m_width - m_cursorX;
    } else if ((uint8_t) *utf8Char == 0xC2 && (uint8_t) *(utf8Char+1) == 0xA0){
        return 10;
    }

    return m_u8g2 -> getUTF8Width(utf8Char);
}

int16_t TFTDisplay::_printChar(uint16_t x, uint16_t y, const char *utf8Char){
    return m_u8g2 -> drawUTF8(m_cursorX, m_cursorY, utf8Char);
}