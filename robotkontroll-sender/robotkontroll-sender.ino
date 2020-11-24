/*
Radio    Arduino
CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection
VCC   -> No more than 3.6 volts
GND   -> GND

*/
#include <SPI.h>
#include <NRFLite.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 2);
/*
        xVal = analogRead(stick1x);
        yVal = analogRead(stick1y);
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(xVal);
        display.println(yVal);
        display.display();*/

        
//robotState, 0 for auto, 1 for taurus, 2 for manual override
int robotStateLocal = 0;
int robotConnected = 0;

const static uint8_t RADIO_ID = 1;
const static uint8_t DESTINATION_RADIO_ID = 0;
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;

int stick1x = A0;    // select the input pin for the potentiometer
int stick1y = A1;    // select the input pin for the potentiometer
uint32_t xVal = 0;
uint32_t yVal = 0;

NRFLite _radio;

uint32_t  _lastHeartbeatSendTime,
          _lastDataRequestTime,
          _lastButtonCheck,
          _lastScreenUpdate,
          _lastButtonPush;

const int blPin = 3;
int backLightPWM = 70;
int screenContrast = 50;

int robotSpeed = 100;

void setBl() {
  analogWrite(blPin, backLightPWM);
}
void setContrast() {
  display.setContrast(screenContrast);
}

#include "menu.h"
#include "radio.h"


void setup()
{
  pinMode(muxPin, INPUT_PULLUP);
  pinMode(muxOut0, OUTPUT);
  pinMode(muxOut1, OUTPUT);
  pinMode(muxOut2, OUTPUT);
  pinMode(muxOut3, OUTPUT);
  
  //set screen backlight with pwm.
  setBl();
  Serial.begin(115200);
  display.begin();
  display.clearDisplay();

  display.drawBitmap(0, 0,  pixilframe0_Bitmap, 84, 48, BLACK);

  display.display();
  delay(2999);


  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)){
        Serial.println("Cannot communicate with radio");
        while (1); // Wait here forever.
  }
}

void loop()
{
        xVal = analogRead(stick1x);
        yVal = analogRead(stick1y);

    // Send a heartbeat with once every tenth of a second.
    if (millis() - _lastHeartbeatSendTime > 99)
    {
        _lastHeartbeatSendTime = millis();
        sendHeartbeat();

      
    }

    // Request data from the robot every half a second.
    if (millis() - _lastDataRequestTime > 3999)
    {
        _lastDataRequestTime = millis();
        requestData();
     
    }
    if (millis() - _lastButtonCheck > 24)
    {
        _lastButtonCheck = millis();
        buttonChecker();
    }
    if (millis() - _lastScreenUpdate > 49)
    {
        _lastScreenUpdate = millis();
        updateMenu();     
    }

}
