/**@file TFT-Reader.ino */

#include "AnalogButton.h"
#include "TFTDisplay.h"
#include "TFTGUI.h"
#include <SD.h>

/** @name Analog button defines
 *	Because of insufficient amount of pins on NodeMCU board 
 *	use single analog pin connected to buttons through resistors
 */
///@{
#define BTN_ANALOG_PIN A0
#define ANALOG_VAL_BTN_DOWN 674
#define ANALOG_VAL_BTN_SELECT 1024
#define ANALOG_VAL_BTN_UP 859
///@}

#define PIN_SD_CS D1 ///< Chip Select pin for SD-card reader

/** @name ILI9341 pins */
///@{
#define TFT_CS    D2
#define TFT_RST   D3
#define TFT_DC    D4 
///@}

/** @name Analog buttons
 *	Uses single analog pin for 3 different buttons.
 */
///@{
AnalogButton btnUp(BTN_ANALOG_PIN, ANALOG_VAL_BTN_UP);
AnalogButton btnSelect(BTN_ANALOG_PIN, ANALOG_VAL_BTN_SELECT);
AnalogButton btnDown(BTN_ANALOG_PIN, ANALOG_VAL_BTN_DOWN);
///@}

Adafruit_ILI9341 ili9341 = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST); ///< Lower level TFT Display object
U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx; ///< Font system initialization

TFTDisplay tft(240, 320, &ili9341, &u8g2_for_adafruit_gfx); ///< TFT Display object, supports text printing
TFTGUI menu(tft); ///< Menu object, can scan SD card and display files


void setup() {
  Serial.begin(9600);
  
  SD.begin(PIN_SD_CS);
  tft.begin();
  menu.begin();
}

void loop() {
  if(menu.m_menuMode){
    if (btnUp.clicked()){
        menu.selectPrevious();
    }
    
    if (btnSelect.clicked()){
        menu.openSelectedFile();
    }
  
    if (btnDown.clicked()){
        menu.selectNext();
    }
 } else {
    if (btnUp.clicked()){
        menu.printPreviousPage();
    }
    
    if (btnSelect.holded()){
        menu.openMenu();
    }
  
    if (btnDown.clicked()){
        menu.printNextPage();
    }
 }
}
