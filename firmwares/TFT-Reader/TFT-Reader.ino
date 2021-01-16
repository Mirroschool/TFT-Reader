#include "AnalogButton.h"
#include "TFTDisplay.h"
#include "TFTGUI.h"
#include <SD.h>

// Button pins
#define BTN_ANALOG_PIN A0
#define ANALOG_VAL_BTN_DOWN 674
#define ANALOG_VAL_BTN_SELECT 1024
#define ANALOG_VAL_BTN_UP 859

// Chip Select pin for SD-card reader
#define PIN_SD_CS D1

#define TFT_CS    D2
#define TFT_RST   D3
#define TFT_DC    D4 

// Button instances construction
AnalogButton btnUp(BTN_ANALOG_PIN, ANALOG_VAL_BTN_UP);
AnalogButton btnSelect(BTN_ANALOG_PIN, ANALOG_VAL_BTN_SELECT);
AnalogButton btnDown(BTN_ANALOG_PIN, ANALOG_VAL_BTN_DOWN);

Adafruit_ILI9341 ili9341 = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

TFTDisplay tft(240, 320, &ili9341, &u8g2_for_adafruit_gfx);

TFTGUI menu(tft);

void setup() {
  // put your setup code here, to run once:
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
