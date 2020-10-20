#include "AnalogButton.h"

// Button pins
#define BTN_ANALOG_PIN A0

#define ANALOG_VAL_BTN_DOWN 674
#define ANALOG_VAL_BTN_SELECT 859
#define ANALOG_VAL_BTN_UP 1024

// Chip Select pin for SD-card reader
#define PIN_SD_CS 10


// Button instances construction
AnalogButton btnUp(BTN_ANALOG_PIN, ANALOG_VAL_BTN_UP);
AnalogButton btnSelect(BTN_ANALOG_PIN, ANALOG_VAL_BTN_SELECT);
AnalogButton btnDown(BTN_ANALOG_PIN, ANALOG_VAL_BTN_DOWN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  if (btnUp.clicked()){
      Serial.println("UP CLICKED!");
  }

  if (btnSelect.clicked()){
    Serial.println("SELECT CLICKED!");
  }  
  
  if (btnDown.clicked()){
    Serial.println("DOWN CLICKED!");
  }

  if (btnUp.holded()){
    Serial.println("UP holded!");
  }

  if (btnSelect.holded()){
    Serial.println("SELECT holded!");
  }  
  
  if (btnDown.holded()){
    Serial.println("DOWN holded!");
  }
}
