#pragma once
#ifndef MY_RADIO_H
#define MY_RADIO_H

#include <TaskManager.h>
#include <SPI.h>
#include <RF24.h>
#include <../lib/myLogger.h>
#include "def.h"

#define LOCAL_DEBUG
#include "myLogger.h"

typedef struct __attribute__((__packed__))
{
  int16_t rcYaw; //!< Get the positions of the rc joysticks
  int8_t rcPitch;
  int8_t rcRoll;
  int16_t rcThrottle;
  uint16_t rcAltitudeSonicAdj;
  uint16_t rcAltitudeBaroAdj;
  bool rcSwi[4];
  
} TX_payload_t; // sendet zur Drohne

typedef struct __attribute__((__packed__))
{
  int16_t yaw; // Fluglage MPU9250
  int8_t secondaryAxis;
  int8_t primaryAxis;
  uint16_t altitude;       // MS5611
  float temperature;       // MPU9250
  float pressure;          // MS5611
  uint16_t distance_down;  // US Sensor looks down
  uint16_t distance_front; // US Sensor looks ahead
  uint16_t battery;        // State of the battery
} RX_payload_t;            // empfängt Daten von der Drohne

typedef struct
{
  bool isconnect;
  RX_payload_t RX_payload;
  TX_payload_t TX_payload;
} RC_interface_t;

void read_from_the_drohne();
void write_to_the_drohne();

class Radio : public Task::Base
{

  const uint64_t pipe_RX = 0xF0F0F0E1L;
  const uint64_t pipe_TX = 0xF0F0F0D2L;
  unsigned long _lastReceivedPacket;

protected:
  RF24 *_radio;
  RC_interface_t *RC_interface;
  TX_payload_t debugTX_payload;
  RX_payload_t debugRX_payload;

public:
  Radio(const String &name)
      : Task::Base(name)
  {
  }

  Radio *setModel(RC_interface_t *_model)
  {
    LOGGER_VERBOSE("Enter....");
    RC_interface = _model;
    LOGGER_VERBOSE("....leave");
    return this;
  }

  virtual void begin() override
  {
    pinMode(LED_RADIO, OUTPUT);
    digitalWrite(LED_RADIO, LOW);
    
    _radio = new RF24(PIN_CE, PIN_CSN);

    if (!_radio->begin())
    {
      LOGGER_FATAL("radio hardware is not responding!!");
      while (1)
      {
      }
    }

    _radio->setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.
    _radio->enableDynamicPayloads(); // ACK payloads are dynamically sized
    _radio->enableAckPayload();
    _radio->openWritingPipe(pipe_TX);
    _radio->openReadingPipe(1, pipe_RX);
    _radio->stopListening(); // put radio in TX mode
  }//---------------------- end of begin ------------------------------------------------------//

  virtual void update() override
  {
    RC_interface->isconnect = _radio->write(&RC_interface->TX_payload, sizeof(TX_payload_t)); // transmit & save the report
    if (RC_interface->isconnect)
    {
      digitalWrite(LED_RADIO, LOW);
      
      write_to_the_drohne();

      if (_radio->available())
      {                                                                // is there an ACK payload? grab the pipe number that received it
        _radio->read(&RC_interface->RX_payload, sizeof(RX_payload_t)); // get incoming ACK payload
      //  read_from_the_drohne();
      }
      else
      {
        LOGGER_FATAL("Recieved: an empty ACK packet");
      }
      digitalWrite(LED_RADIO, HIGH);
    }
    else
    {
      //LOGGER_FATAL("Transmission failed or timed out"); // payload was not delivered  // payload was not delivered
    }
  } //---------------------- end of update ------------------------------------------------------//

  void read_from_the_drohne()
  {
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.yaw, debugRX_payload.yaw, "Received Yaw = %i", RC_interface->RX_payload.yaw);
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.secondaryAxis, debugRX_payload.secondaryAxis, "Received Pitch = %i", RC_interface->RX_payload.secondaryAxis);
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.primaryAxis, debugRX_payload.primaryAxis, "Received Roll = %i", RC_interface->RX_payload.primaryAxis);
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.temperature, debugRX_payload.temperature, "Received temperature = %i *C", RC_interface->RX_payload.temperature);
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.pressure, debugRX_payload.pressure, "Received pressure = %i hP", RC_interface->RX_payload.pressure);
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.altitude, debugRX_payload.altitude, "Received Alt = %i cm", RC_interface->RX_payload.altitude);
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.distance_down, debugRX_payload.distance_down, "Received Ground = %i m", RC_interface->RX_payload.distance_down);
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.distance_front, debugRX_payload.distance_front, "Received Front = %i m", RC_interface->RX_payload.distance_front);
    LOGGER_NOTICE_FMT_CHK(RC_interface->RX_payload.battery, debugRX_payload.battery, "Battery state = %i", RC_interface->RX_payload.battery);
  }

  void write_to_the_drohne()
  {
    LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcThrottle, debugTX_payload.rcThrottle, "Throttle = %i", RC_interface->TX_payload.rcThrottle);
    // LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcYaw, debugTX_payload.rcYaw, "Yaw = %i", RC_interface->TX_payload.rcYaw);
    // LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcPitch, debugTX_payload.rcPitch, "Pitch = %i", RC_interface->TX_payload.rcPitch);
    // LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcRoll, debugTX_payload.rcRoll, "Roll = %i", RC_interface->TX_payload.rcRoll);

    // LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcSwi[0], debugTX_payload.rcSwi[0], "Swi 0 = %i", RC_interface->TX_payload.rcSwi[0]);
    // LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcSwi[1], debugTX_payload.rcSwi[1], "Swi 1 = %i", RC_interface->TX_payload.rcSwi[1]);
    
    // LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcAltitudeBaroAdj, debugTX_payload.rcAltitudeBaroAdj, "Adjus Hoehe = %i", RC_interface->TX_payload.rcAltitudeBaroAdj);
    // LOGGER_NOTICE_FMT_CHK(RC_interface->TX_payload.rcAltitudeSonicAdj, debugTX_payload.rcAltitudeSonicAdj, "Adjust Ground = %i", RC_interface->TX_payload.rcAltitudeSonicAdj);
  }
};
/*------------------------- end of radio class --------------------------------------------------*/
#endif // MY_RADIO_H
