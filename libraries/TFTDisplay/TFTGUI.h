#ifndef OLED_GUI_H
#define OLED_GUI_H

#include <SD.h>
#include "TFTDisplay.h"

#define MAX_FILES_IN_MENU 15

struct SDTextFile
{
	char name[32];
};

struct pageLengthStack
{
	uint16_t positions[100];
	int8_t topId = -1;

	void push(uint16_t x){
		topId++;
		positions[topId] = x;
	}

	uint16_t pop(){
		if (topId != -1) {
            topId--;
            return positions[topId + 1];
        } else {
            return 0;
        }
	}
};

char* getFileExtension(char* name);

class TFTGUI
{
	public:
		bool m_menuMode = true;

		TFTGUI(TFTDisplay &tft);
		void begin();
		
		void openSelectedFile();
		void openMenu(bool scanSDCard = false);

		void selectPrevious();
		void selectNext();
		
		void printPage();
		void printNextPage();
		void printPreviousPage();
	
	private:
		byte m_selectedPosition, m_txtFilesCount;
		pageLengthStack m_printedPageCharsStack;

		File m_openedTextFile;
		SDTextFile m_textFiles[16];
		TFTDisplay m_tft;
};

#endif