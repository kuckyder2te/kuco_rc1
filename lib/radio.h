#pragma once
#ifndef MY_RADIO_H
#define MY_RADIO_H

#include <TaskManager.h>
#include <RF24.h>
#include <../lib/myLogger.h>

//#define LOCAL_DEBUG
#include "myLogger.h"

#define PIN_CE 48
#define PIN_CSN 53

typedef struct __attribute__((__packed__))
{
    uint16_t rcThrottle; //!< Get the positions of the rc joysticks
    float rcYaw;
    float rcPitch;
    float rcRoll;
    bool rcSwi1;     // bool ???
    bool rcSwi2;
    bool rcSwi3;
    float checksum;
} RX_payload_t;                 // sendet zur Drohne

typedef struct __attribute__((__packed__))
{
    float yaw;          // Fluglage via MPU9250
    float pitch;
    float roll;
    uint16_t altitude;      // Höhe via BMP280  
    uint16_t sonic;         // US Sensor
    float temperature;   // MPU9250
    float pressure;     // BMP280
} TX_payload_t;                 // empfängt Daten von der Drohne

typedef struct
{
    bool isconnect;
    RX_payload_t RX_payload; 
} RC_interface_t;

RF24 nrf24(PIN_CE, PIN_CSN);

const uint64_t pipe[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

struct PayloadStruct {
  char message[7];  // only using 6 characters for TX & ACK payloads
  uint8_t counter;
};
PayloadStruct payload;

class Radio : public Task::Base {

    bool radioNumber; // 0 uses pipe[0] to transmit, 1 uses pipe[1] to received
    bool role, _role,__role,___role;        // true(>0) = TX role, false(0) = RX role
    const uint64_t pipe[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
    RX_payload_t _RX_payload;
    unsigned long _lastReceivedPacket;

protected:
    RF24 *_radio; 
    RC_interface_t *RC_interface;   
    TX_payload_t TX_payload, _TX_payload;

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

        bool role = true;  //~ Sender
        bool radioNumber = 1; 

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
        // nrf24.setPayloadSize(sizeof(txValues_t));	///< optionally, reduce the payload size. seems to improve reliability
        nrf24.openWritingPipe(pipe[radioNumber]);
        nrf24.openReadingPipe(1, pipe[radioNumber]);

        memcpy(payload.message, "Hello ", 6);  // set the payload message
        nrf24.stopListening();                 // put radio in TX mode


        nrf24.startListening();

    }

    virtual void update() override {

 if (role) {
    // This device is a TX node

    unsigned long start_timer = micros();                  // start the timer
    bool report = nrf24.write(&payload, sizeof(payload));  // transmit & save the report

    Serial.print("Report = ");Serial.println(report);
    Serial.print("Role = ");Serial.println(role);
    Serial.print("Radionumber = ");Serial.println(radioNumber);

    unsigned long end_timer = micros();                    // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));  // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);  // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.print(payload.message);  // print the outgoing message
      Serial.print(payload.counter);  // print the outgoing counter
      uint8_t pipe;
      if (nrf24.available(&pipe)) {  // is there an ACK payload? grab the pipe number that received it
        PayloadStruct received;
        nrf24.read(&received, sizeof(received));  // get incoming ACK payload
        Serial.print(F(" Recieved "));
        Serial.print(nrf24.getDynamicPayloadSize());  // print incoming payload size
        Serial.print(F(" bytes on pipe "));
        Serial.print(pipe);  // print pipe number that received the ACK
        Serial.print(F(": "));
        Serial.print(received.message);    // print incoming message
        Serial.println(received.counter);  // print incoming counter

        // save incoming counter & increment for next outgoing
        payload.counter = received.counter + 1;

      } else {
        Serial.println(F(" Recieved: an empty ACK packet"));  // empty ACK packet received
      }


    } else {
      Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
    }

    // to make this example readable in the serial monitor
    delay(1000);  // slow transmissions down by 1 second

  } else {
    // This device is a RX node

    uint8_t pipe;
    if (nrf24.available(&pipe)) {                     // is there a payload? get the pipe number that recieved it
      uint8_t bytes = nrf24.getDynamicPayloadSize();  // get the size of the payload
      PayloadStruct received;
      nrf24.read(&received, sizeof(received));  // get incoming payload
      Serial.print(F("Received "));
      Serial.print(bytes);  // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);  // print the pipe number
      Serial.print(F(": "));
      Serial.print(received.message);  // print incoming message
      Serial.print(received.counter);  // print incoming counter
      Serial.print(F(" Sent: "));
      Serial.print(payload.message);    // print outgoing message
      Serial.println(payload.counter);  // print outgoing counter

      // save incoming counter & increment for next outgoing
      payload.counter = received.counter + 1;
      // load the payload for the first received transmission on pipe 0
      nrf24.writeAckPayload(1, &payload, sizeof(payload));
    }
  }  // role

  if (Serial.available()) {
    // change the role via the serial monitor

    char c = toupper(Serial.read());
    if (c == 'T' && !role) {
      // Become the TX node

      role = true;
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));

      memcpy(payload.message, "Hello ", 6);  // change payload message
      nrf24.stopListening();                 // this also discards any unused ACK payloads

    } else if (c == 'R' && role) {
      // Become the RX node

      role = false;
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      memcpy(payload.message, "World ", 6);  // change payload message

      // load the payload for the first received transmission on pipe 0
      nrf24.writeAckPayload(1, &payload, sizeof(payload));
      nrf24.startListening();
    }
  }
       
    } // end of update


};

#endif  // MY_RADIO_H
