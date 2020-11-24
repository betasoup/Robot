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
};


void sendHeartbeat()
{

    Serial.print("Sending heartbeat");
    RadioPacket radioData;
    radioData.PacketType = Heartbeat;
    radioData.FromRadioId = RADIO_ID;
    radioData.analogStickX = xVal;
    radioData.analogStickY = yVal;
    radioData.OnTimeMillis = millis();
    radioData.robotState = robotStateLocal;

    if (_radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData)))
    {
        Serial.println("...Success");
        robotConnected = 1;
    }
    else
    {
        Serial.println("...Failed");
        robotConnected = 0;
    }
}

void requestData()
{
    Serial.println("Requesting data");
    Serial.print("  Sending BeginGetData");

    RadioPacket radioData;
    radioData.PacketType = BeginGetData; // When the receiver sees this packet type, it will load an ACK data packet.
    
    if (_radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData)))
    {
        Serial.println("...Success");
        Serial.print("  Sending EndGetData");

        radioData.PacketType = EndGetData; // When the receiver acknowledges this packet, we will get the ACK data.

        if (_radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData)))
        {
            Serial.println("...Success");

            while (_radio.hasAckData()) // Look to see if the receiver provided the ACK data.
            {
                RadioPacket ackData;
                _radio.readData(&ackData);

                if (ackData.PacketType == AcknowledgementData)
                {
                    String msg = "  Received ACK data from ";
                    msg += ackData.FromRadioId;
                    msg += ", ";
                    msg += ackData.OnTimeMillis;
                    msg += " ms";
                    msg += " X/Y";
                    msg += ackData.analogStickX;
                    msg += " X/Y";
                    msg += ackData.analogStickY;
                    Serial.println(msg);

                }
            }
        }
        else
        {
            Serial.println("...Failed");
        }
    }
    else
    {
        Serial.println("...Failed");
    }    
}
