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

const static uint8_t RADIO_ID = 0;
const static uint8_t DESTINATION_RADIO_ID = 1;
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;


enum RadioPacketType
{
    AcknowledgementData, // Produced by the primary receiver and provided to the transmitter via an acknowledgement data packet.
    Heartbeat,    // Sent by the primary transmitter.
    BeginGetData, // Sent by the primary transmitter to tell the receiver it should load itself with an acknowledgement data packet.
    EndGetData    // Sent by the primary transmitter to receive the acknowledgement data packet from the receiver.
};

struct RadioPacket
{
    RadioPacketType PacketType;
    uint8_t FromRadioId;
    uint32_t analogStickX;
    uint32_t analogStickY;
    uint32_t OnTimeMillis;
    uint8_t robotState;
    uint8_t robotSpeedL;
    uint8_t robotSpeedR;
    uint8_t direction;

};

NRFLite _radio;

void radioCheck()
{
    while (_radio.hasData())
    {
        RadioPacket radioData;
        _radio.readData(&radioData);

        if (radioData.PacketType == Heartbeat)
        {
            String msg = "Heartbeat from ";
            msg += radioData.FromRadioId;
            msg += ", ";
            msg += radioData.OnTimeMillis;
            msg += " ms";
            msg += " X/Y";
            msg += radioData.analogStickX;
            msg += " X/Y";
            msg += radioData.analogStickY;
            Serial.print("xyDir");
            Serial.print('\n');
            xVal = radioData.analogStickX;
            yVal = radioData.analogStickY;
            if(robotStateLocal != radioData.robotState)
            robotStateLocal = radioData.robotState;
            Serial.print(String(xVal));
            Serial.print('\n');
            Serial.print(String(yVal));
            Serial.print('\n');
            Serial.print(radioData.robotState);
            Serial.print('\n');
        }
        else if (radioData.PacketType == BeginGetData)
        {
            //Serial.println("Received data request, adding ACK data packet");

            RadioPacket ackData;
            ackData.PacketType = AcknowledgementData;
            ackData.FromRadioId = RADIO_ID;
            ackData.analogStickX = xVal;
            ackData.analogStickY = yVal;
            ackData.OnTimeMillis = millis();
            ackData.robotSpeedL = yVal;
            ackData.robotSpeedR = yVal;
            ackData.direction = roboDirection;
            
            // Add the data to send back to the transmitter into the radio.
            // The next packet we receive will be acknowledged with this data.
            _radio.addAckData(&ackData, sizeof(ackData));
        }
        else if (radioData.PacketType == EndGetData)
        {
            // The transmitter hopefully received the acknowledgement data packet at this point.
        }
    }
}
