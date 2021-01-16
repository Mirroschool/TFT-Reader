#include "TFTDisplay.h"
#include "TFTGUI.h"
#include <SD.h>


TFTGUI::TFTGUI(TFTDisplay& oled): m_tft(oled) {}

void TFTGUI::openMenu(bool scanSDCard){
    m_printedPageCharsStack.topId = -1;
    
    m_tft.clear();
    //m_tft.home();

    m_menuMode = true;

    m_openedTextFile.close();

    if (scanSDCard){
        File rootDirectory = SD.open("/");
        
        while (true) {
            File entry = rootDirectory.openNextFile();
            Serial.println(entry.name());

            if (!entry or m_txtFilesCount > MAX_FILES_IN_MENU) {
                break;
            }

            // If entry is file
            if (!entry.isDirectory()) {
                const char* name = entry.name();
                m_txtFilesCount++;
                strcpy(m_textFiles[m_txtFilesCount - 1].name, name);
            }
            entry.close();
        }
        rootDirectory.close();
    }

    for (int i = 0; i < m_txtFilesCount; i++)
    {
        m_tft.setCursor(0, m_tft.m_lineHeight + i * m_tft.m_lineHeight);
        if (i == m_selectedPosition) {
            m_tft.print(m_textFiles[i].name);
            m_tft.drawRoundRect(0, i * m_tft.m_lineHeight, m_tft.m_width, m_tft.m_lineHeight+VERTICAL_DISTANCE_BETWEEN_CHARACTERS, 5, 0xF800);
        } else {
            m_tft.print(m_textFiles[i].name);
        }
        
    }
}

void TFTGUI::begin()
{
    m_tft.begin();
    openMenu(true);
}

void TFTGUI::openSelectedFile()
{
    m_tft.clear();
    m_menuMode = false;

    printPage();
}

void TFTGUI::selectPrevious()
{
    if (m_selectedPosition > 0) {
        m_selectedPosition--;

        m_tft.drawRoundRect(0, (m_selectedPosition+1) * m_tft.m_lineHeight, m_tft.m_width, m_tft.m_lineHeight+VERTICAL_DISTANCE_BETWEEN_CHARACTERS, 5, 0x0000);
        m_tft.drawRoundRect(0, m_selectedPosition * m_tft.m_lineHeight, m_tft.m_width, m_tft.m_lineHeight+VERTICAL_DISTANCE_BETWEEN_CHARACTERS, 5, 0xF800);
    }
}

void TFTGUI::selectNext()
{
    if (m_selectedPosition < MAX_FILES_IN_MENU && m_selectedPosition < m_txtFilesCount-1) {
        m_selectedPosition++;

        m_tft.drawRoundRect(0, (m_selectedPosition-1) * m_tft.m_lineHeight, m_tft.m_width, m_tft.m_lineHeight+VERTICAL_DISTANCE_BETWEEN_CHARACTERS, 5, 0x0000);
        m_tft.drawRoundRect(0, m_selectedPosition * m_tft.m_lineHeight, m_tft.m_width, m_tft.m_lineHeight+VERTICAL_DISTANCE_BETWEEN_CHARACTERS, 5, 0xF800);
    }
}

void TFTGUI::printPage(){
    if (!m_menuMode){
        SDTextFile selectedTextFile = m_textFiles[m_selectedPosition];
        if (!m_openedTextFile){
            m_openedTextFile = SD.open(selectedTextFile.name);
            if (!m_openedTextFile){
                Serial.print("Failed opening file: ");
                Serial.println(selectedTextFile.name);
                return;
            }
        }

        if (m_openedTextFile.peek() != -1){
            m_tft.clear();
            m_tft.home();
            
            uint8_t currentByte, currentCharLength;
            uint8_t tempCharBuf[5];

            uint16_t printedBytes = 0;
            memset(tempCharBuf, 0x00, 5);
            
            while(m_openedTextFile.available()){
                currentByte = m_openedTextFile.read();
                currentCharLength = 0;

                // Ignore \r symbol
                if (currentByte == 0x0D){
                    printedBytes += 1;
                    continue;
                }

                if ((currentByte & 0x80) == 0){
                    tempCharBuf[0] = currentByte;
                    currentCharLength = 1;

                } else if ((currentByte & 0xE0) == 0xC0) {
                    tempCharBuf[0] = currentByte;
                    tempCharBuf[1] = m_openedTextFile.read();
                    currentCharLength = 2;

                } else if ((currentByte & 0xF0) == 0xE0) {
                    tempCharBuf[0] = currentByte;
                    tempCharBuf[1] = m_openedTextFile.read();
                    tempCharBuf[2] = m_openedTextFile.read();
                    currentCharLength = 3;

                } else if ((currentByte & 0xF8) == 0xF0) {
                    tempCharBuf[0] = currentByte;
                    tempCharBuf[1] = m_openedTextFile.read();
                    tempCharBuf[2] = m_openedTextFile.read();
                    tempCharBuf[3] = m_openedTextFile.read();
                    currentCharLength = 4;
                }

                if (m_tft.printChar((const char*)tempCharBuf)){
                    printedBytes += currentCharLength;
                } else {
                    m_openedTextFile.seek(m_openedTextFile.position() - currentCharLength);
                    break;
                }
                memset(tempCharBuf, 0x00, 5);
            }

            // Clear previous page characters if any left
            m_printedPageCharsStack.push(printedBytes);
        }
    }
}

void TFTGUI::printNextPage()
{
    if (m_printedPageCharsStack.topId <= 99){
        printPage();
    }
}

void TFTGUI::printPreviousPage()
{
    if (m_printedPageCharsStack.topId >= 1){
        long previousPagePosition = m_openedTextFile.position() 
                                    - m_printedPageCharsStack.pop()  // Current page length
                                    - m_printedPageCharsStack.pop(); // Previous page length
        
        m_openedTextFile.seek(previousPagePosition);
        printPage();
    }   
}