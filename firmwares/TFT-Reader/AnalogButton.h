#ifndef ANALOG_BUTTON_H
#define ANALOG_BUTTON_H
// мини-класс для работы с кнопкой, версия 1.0

#define ANALOG_BTN_RANGE 20

#pragma pack(push, 1)
typedef struct {
    bool holdedFlag : 1;
    bool btnFlag : 1;
    bool pressF : 1;
    bool clickF : 1;
    bool holdF : 1;
} analogButtonFlags;
#pragma pack(pop)

class AnalogButton {
public:
    AnalogButton(uint8_t pin, uint16_t analogValue);
    boolean pressed();
    boolean clicked();
    boolean holding();
    boolean holded();

private:
    analogButtonFlags flags;
    void tick();
    uint16_t _analogValue;
    uint32_t _btnTimer;
    byte _pin;
};

AnalogButton::AnalogButton(uint8_t pin, uint16_t analogValue)
{
    _pin = pin;
    _analogValue = analogValue;
}

void AnalogButton::tick()
{
    int16_t currentBtnAnalogValue = analogRead(_pin);

    boolean btnState = !(
    	((_analogValue - ANALOG_BTN_RANGE) < currentBtnAnalogValue)
    								&&  
    	(currentBtnAnalogValue < (_analogValue + ANALOG_BTN_RANGE))
    );

    if (!btnState && !flags.btnFlag && ((uint32_t)millis() - _btnTimer > 90)) {
        flags.btnFlag = true;
        _btnTimer = millis();
        flags.pressF = true;
        flags.holdedFlag = true;
    }
    if (btnState && flags.btnFlag && ((uint32_t)millis() - _btnTimer < 350)) {
        flags.btnFlag = false;
        _btnTimer = millis();
        flags.clickF = true;
        flags.holdF = false;
    }
    if (flags.btnFlag && ((uint32_t)millis() - _btnTimer > 700)) {
        if (!btnState) {
            flags.holdF = true;
        }
        else {
            flags.btnFlag = false;
            flags.holdF = false;
            _btnTimer = millis();
        }
    }
}

boolean AnalogButton::pressed()
{
    AnalogButton::tick();
    if (flags.pressF) {
        flags.pressF = false;
        return true;
    }
    else
        return false;
}

boolean AnalogButton::clicked()
{
    AnalogButton::tick();
    if (flags.clickF) {
        flags.clickF = false;
        return true;
    }
    else
        return false;
}

boolean AnalogButton::holding()
{
    AnalogButton::tick();
    if (flags.holdF) {
        return true;
    }
    else
        return false;
}

boolean AnalogButton::holded()
{
    AnalogButton::tick();
    if (flags.holdF && flags.holdedFlag) {
        flags.holdedFlag = false;
        return true;
    }
    else
        return false;
}
#endif