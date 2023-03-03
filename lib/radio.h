#pragma once
#ifndef MY_RADIO_H
#define MY_RADIO_H

#include <TaskManager.h>
#include <RF24.h>
#include <../lib/myLogger.h>

#define LOCAL_DEBUG
#include "myLogger.h"

#define PIN_CE 48
#define PIN_CSN 53

typedef struct __attribute__((__packed__))
{
    int16_t rcThrottle; //!< Get the positions of the rc joysticks
    float rcYaw;
    float rcPitch;
    float rcRoll;
    bool rcSwi1;     // bool ???
    bool rcSwi2;
    bool rcSwi3;
    float checksum;
} TX_payload_t;                 // sendet zur Drohne

typedef struct __attribute__((__packed__))
{
    float yaw;          // Fluglage via MPU9250
    float pitch;
    float roll;
    uint16_t altitude;   // Höhe via MS5611  
    uint16_t sonic;      // US Sensor
    float temperature;   // MPU9250
    float pressure;      // MS5611
} RX_payload_t;                 // empfängt Daten von der Drohne

typedef struct
{
    bool isconnect;
    RX_payload_t RX_payload; 
    TX_payload_t TX_payload;
} RC_interface_t;

RF24 nrf24(PIN_CE, PIN_CSN);

class Radio : public Task::Base {

    const uint64_t pipe_RX = 0xF0F0F0F0E1LL;
    const uint64_t pipe_TX = 0xF0F0F0F0D2LL;


    unsigned long _lastReceivedPacket;

protected:
    RF24 *_radio; 
    RC_interface_t *RC_interface;   
    TX_payload_t debugTX_payload;
    RX_payload_t debugRX_payload;

public:
    Radio(const String& name)
    : Task::Base(name) {
    }

    virtual ~Radio() {}

    Radio *setModel(RC_interface_t *_model)
    { 
        LOGGER_VERBOSE("Enter....");
        RC_interface = _model;
        LOGGER_VERBOSE("....leave");
        return this;
    }

    virtual void begin() override {

        nrf24.begin(); ///< optionally, increase the delay between retries & # of retries
        if (!nrf24.begin())
        {
            LOGGER_FATAL("radio hardware is not responding!!");
            while (1)
            {} // hold in infinite loop
        }

        nrf24.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.
        nrf24.enableDynamicPayloads(); // ACK payloads are dynamically sized
        nrf24.enableAckPayload();
        // nrf24.setRetries(15,15);
        nrf24.setPayloadSize(sizeof(TX_payload_t));	///< optionally, reduce the payload size. seems to improve reliability
        nrf24.openWritingPipe(pipe_TX);
        nrf24.openReadingPipe(1, pipe_RX);
        nrf24.stopListening();                 // put radio in TX mode
    }

    virtual void update() override {
    RC_interface->isconnect = nrf24.write(&RC_interface->TX_payload, sizeof(TX_payload_t));  // transmit & save the report
    if (RC_interface->isconnect) {
        LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcThrottle,debugTX_payload.rcThrottle,"Throttle = %i", RC_interface->TX_payload.rcThrottle);
 
        if (nrf24.available()) {  // is there an ACK payload? grab the pipe number that received it
          nrf24.read(&RC_interface->RX_payload, sizeof(RX_payload_t));  // get incoming ACK payload
          LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.sonic,debugRX_payload.sonic,"Received Sonic = %i cm",RC_interface->RX_payload.sonic);
        } else {
          LOGGER_FATAL("Recieved: an empty ACK packet");
        }
      } else {
        LOGGER_FATAL("Transmission failed or timed out");  // payload was not delivered  // payload was not delivered
      }
    delay(100);  // slow transmissions down by 1 second      
    } // end of update
};

#endif  // MY_RADIO_H
