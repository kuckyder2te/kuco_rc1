#pragma once
#ifndef MY_RADIO_H
#define MY_RADIO_H

#include <TaskManager.h>
#include <SPI.h>
#include <RF24.h>
#include <../lib/myLogger.h>

//#define LOCAL_DEBUG
#include "myLogger.h"

#define PIN_CE 49
#define PIN_CSN 53

#define LED_ALERT    10
#define PIN_RADIO_LED   1

typedef struct __attribute__((__packed__))
{
    float checksum;
    uint16_t rcYaw;
    uint8_t rcPitch;
    uint8_t rcRoll;
    int16_t rcThrottle; //!< Get the positions of the rc joysticks
    uint16_t rcAltitudeSonicAdj;
    uint16_t rcAltitudeBaroAdj;
    bool rcSwi1;
    bool rcSwi2;
    bool rcSwi3;
} TX_payload_t;                 // sendet zur Drohne

typedef struct __attribute__((__packed__))
{
    float yaw;          // Fluglage via MPU9250
    float pitch;
    float roll;
    uint16_t altitude;   // Höhe via MS5611  
//    uint16_t sonic;      // US Sensor
    float temperature;   // MPU9250
    float pressure;      // MS5611
    uint16_t distance_down; // US Sensor
    uint16_t distance_front;  
} RX_payload_t;                 // empfängt Daten von der Drohne

typedef struct
{
    bool isconnect;
    RX_payload_t RX_payload; 
    TX_payload_t TX_payload;
} RC_interface_t;
class Radio : public Task::Base {

  const uint64_t pipe_RX = 0xF0F0F0E1L;
  const uint64_t pipe_TX = 0xF0F0F0D2L;
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
        _radio = new RF24(PIN_CE, PIN_CSN);
        if (!_radio->begin())
        {
            LOGGER_FATAL("radio hardware is not responding!!");
            while (1)
            {} // hold in infinite loop
        }

        _radio->setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.
        _radio->enableDynamicPayloads(); // ACK payloads are dynamically sized
        _radio->enableAckPayload();
        // nrf24.setRetries(15,15);
        //_radio->setPayloadSize(sizeof(TX_payload_t));	///< optionally, reduce the payload size. seems to improve reliability
        _radio->openWritingPipe(pipe_TX);
        _radio->openReadingPipe(1, pipe_RX);
        _radio->stopListening();                 // put radio in TX mode
    }//---------------------- end of begin ------------------------------------------------------//

    virtual void update() override {
    RC_interface->isconnect = _radio->write(&RC_interface->TX_payload, sizeof(TX_payload_t));  // transmit & save the report
    if (RC_interface->isconnect) {
        #ifndef SERIAL_STUDIO
            LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcThrottle,debugTX_payload.rcThrottle,"Throttle = %i", RC_interface->TX_payload.rcThrottle);
            LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcYaw,debugTX_payload.rcYaw,"Yaw = %i", RC_interface->TX_payload.rcYaw);
            LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcPitch,debugTX_payload.rcPitch,"Pitch = %i", RC_interface->TX_payload.rcPitch);
            LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcRoll,debugTX_payload.rcRoll,"Roll = %i", RC_interface->TX_payload.rcRoll);
            LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcSwi1,debugTX_payload.rcSwi1,"Swi 1 = %i", RC_interface->TX_payload.rcSwi1);
            LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcSwi2,debugTX_payload.rcSwi2,"Swi 2 = %i", RC_interface->TX_payload.rcSwi2);
            LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcSwi3,debugTX_payload.rcSwi3,"Swi 3 = %i", RC_interface->TX_payload.rcSwi3);
        #endif
        if (_radio->available()) {  // is there an ACK payload? grab the pipe number that received it
          _radio->read(&RC_interface->RX_payload, sizeof(RX_payload_t));  // get incoming ACK payload
          #ifndef SERIAL_STUDIO
            LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.yaw,debugRX_payload.yaw,"Received Yaw = %i",RC_interface->RX_payload.yaw);
            LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.pitch,debugRX_payload.pitch,"Received Pitch = %i",RC_interface->RX_payload.pitch);
            LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.sonic,debugRX_payload.sonic,"Received Roll = %i",RC_interface->RX_payload.roll);
            LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.sonic,debugRX_payload.temperature,"Received temperature = %i *C",RC_interface->RX_payload.temperature);
            LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.sonic,debugRX_payload.pressure,"Received pressure = %i hP",RC_interface->RX_payload.pressure);
            LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.altitude,debugRX_payload.altitude,"Received Sonic = %i cm",RC_interface->RX_payload.altitude);
            LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.sonic,debugRX_payload.sonic,"Received Altitude = %i m",RC_interface->RX_payload.sonic);
          #endif
        } else {
          LOGGER_FATAL("Recieved: an empty ACK packet");
        }
      } else {
        LOGGER_FATAL("Transmission failed or timed out");  // payload was not delivered  // payload was not delivered
      }
    //delay(100);  // slow transmissions down by 1 second      
    }//---------------------- end of update ------------------------------------------------------//
};

#endif  // MY_RADIO_H
