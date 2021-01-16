// MIT License

// Copyright (c) 2018 AlexGyver

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef ANALOG_BUTTON_H
#define ANALOG_BUTTON_H

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
        bool pressed();
        bool clicked();
        bool holding();
        bool holded();

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

    bool btnState = !(
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

bool AnalogButton::pressed()
{
    AnalogButton::tick();
    if (flags.pressF) {
        flags.pressF = false;
        return true;
    }
    else
        return false;
}

bool AnalogButton::clicked()
{
    AnalogButton::tick();
    if (flags.clickF) {
        flags.clickF = false;
        return true;
    }
    else
        return false;
}

bool AnalogButton::holding()
{
    AnalogButton::tick();
    return flags.holdF;
}

bool AnalogButton::holded()
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
