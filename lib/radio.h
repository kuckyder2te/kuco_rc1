#pragma once
#ifndef MY_RADIO_H
#define MY_RADIO_H

#include <TaskManager.h>
#include <RF24.h>
#include <../lib/myLogger.h>

//#define LOCAL_DEBUG
//#include "myLogger.h"

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
} TX_payload_t;                 // sendet zur Drohne

typedef struct __attribute__((__packed__))
{
    float yaw;          // Fluglage via MPU9250
    float pitch;
    float roll;
    uint16_t altitude;      // Höhe via BMP280  
    uint16_t sonic;         // US Sensor
    float temperature;   // MPU9250
    float pressure;     // BMP280
} RX_payload_t;                 // empfängt Daten von der Drohne

typedef struct
{
    bool isconnect;
    RX_payload_t RX_payload; 
    TX_payload_t TX_payload;
} RC_interface_t;

RF24 nrf24(PIN_CE, PIN_CSN);

//const uint64_t pipe[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

// struct PayloadStruct {
//   char message[7];  // only using 6 characters for TX & ACK payloads
//   uint8_t counter;
// };
//PayloadStruct payload;

class Radio : public Task::Base {

//    bool radioNumber; // 0 uses pipe[0] to transmit, 1 uses pipe[1] to received
//    bool role, _role,__role,___role;        // true(>0) = TX role, false(0) = RX role
  //  const uint64_t pipe[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
    const uint64_t pipe_RX = 0xF0F0F0F0E1LL;
    const uint64_t pipe_TX = 0xF0F0F0F0D2LL;

    RX_payload_t debugRX_payload;
    unsigned long _lastReceivedPacket;

protected:
    RF24 *_radio; 
    RC_interface_t *RC_interface;   
    TX_payload_t debugTX_payload;

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

        // bool role = true;  //~ Sender
        // bool radioNumber = true; 

        nrf24.begin(); ///< optionally, increase the delay between retries & # of retries
        if (!nrf24.begin())
        {
            Serial.println("radio hardware is not responding!!");
            while (1)
            {} // hold in infinite loop
        }

        //  // print example's introductory prompt
        // Serial.println(F("RF24/examples/AcknowledgementPayloads"));

        // // To set the radioNumber via the Serial monitor on startup
        // Serial.println(F("Which radio is this? Enter '0' or '1'. Defaults to '0'"));
        // while (!Serial.available()) {
        //   // wait for user input
        // }
        // char input = Serial.parseInt();
        // radioNumber = input == 1;
        // Serial.print(F("radioNumber = "));
        // Serial.println((int)radioNumber);

        // // role variable is hardcoded to RX behavior, inform the user of this
        // Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

        nrf24.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.
        nrf24.enableDynamicPayloads(); // ACK payloads are dynamically sized
        nrf24.enableAckPayload();
        // nrf24.setRetries(15,15);
        nrf24.setPayloadSize(sizeof(TX_payload_t));	///< optionally, reduce the payload size. seems to improve reliability
        nrf24.openWritingPipe(pipe_TX);
        nrf24.openReadingPipe(1, pipe_RX);

        nrf24.stopListening();                 // put radio in TX mode
          
          // else {
          //   // setup the ACK payload & load the first response into the FIFO

          //   memcpy(payload.message, "World ", 6);  // set the payload message
          //   // load the payload for the first received transmission on pipe 0
          //   nrf24.writeAckPayload(1, &payload, sizeof(payload));

          //   nrf24.startListening();  // put radio in RX mode
          // }
          // For debugging info
          // printf_begin();             // needed only once for printing details
          // radio.printDetails();       // (smaller) function that prints raw register values
          // radio.printPrettyDetails(); // (larger) function that prints human readable data
    }

    virtual void update() override {

// if (role) {
    // This device is a TX node

    unsigned long start_timer = micros();                  // start the timer
    bool report = nrf24.write(&RC_interface->TX_payload, sizeof(TX_payload_t));  // transmit & save the report

  //  LOGGER_NOTICE_FMT("Role = %i  RadioNumber = %i", role, radioNumber);
    //  Serial.print("Role = ");Serial.println(role);
    //  Serial.print("Radionumber = ");Serial.println(radioNumber);

    unsigned long end_timer = micros();                    // end the timer

      if (report) {
      //  LOGGER_NOTICE_FMT("Role = %i  RadioNumber = %i", role, radioNumber);
        Serial.print(F("Transmission successful! "));  // payload was delivered
        Serial.print(F("Time to transmit = "));
        Serial.print(end_timer - start_timer);  // print the timer result
        Serial.print(F(" us. Sent: "));
        Serial.print(RC_interface->TX_payload.rcThrottle);  // print the outgoing message
        //Serial.print(payload.counter);  // print the outgoing counter
  //      uint8_t pipe;
        if (nrf24.available()) {  // is there an ACK payload? grab the pipe number that received it
       //   PayloadStruct received;
          nrf24.read(&RC_interface->RX_payload, sizeof(RX_payload_t));  // get incoming ACK payload
          //LOGGER_NOTICE_FMT("Role = %i  RadioNumber = %i", role, radioNumber);
          //LOGGER_NOTICE_FMT("Received %i bytes on pipe %i",_radio->getDynamicPayloadSize(), pipe);
          Serial.print(F(" Recieved "));
          Serial.print(nrf24.getDynamicPayloadSize());  // print incoming payload size
          Serial.print(F(" bytes"));
//          Serial.print(pipe);  // print pipe number that received the ACK
//          LOGGER_NOTICE_FMT("Message = %s counter = %i",received.message,received.counter);
          Serial.print(F(": "));
          Serial.print(RC_interface->RX_payload.sonic);    // print incoming message
//          Serial.println(received.counter);  // print incoming counter

          // save incoming counter & increment for next outgoing
//          payload.counter = received.counter + 1;

        } else {
          //LOGGER_FATAL("Recieved: an empty ACK packet");
          Serial.println(F(" Recieved: an empty ACK packet"));  // empty ACK packet received
        }
      } else {
        LOGGER_FATAL("Transmission failed or timed out");  // payload was not delivered  // payload was not delivered
      }

    // to make this example readable in the serial monitor
    delay(100);  // slow transmissions down by 1 second

 // } 
  // else {
  //   // This device is a RX node

  //   uint8_t pipe;
  //   if (nrf24.available(&pipe)) {                     // is there a payload? get the pipe number that recieved it
  //     uint8_t bytes = nrf24.getDynamicPayloadSize();  // get the size of the payload
  //     PayloadStruct received;
  //     nrf24.read(&received, sizeof(received));  // get incoming payload
  //     //LOGGER_NOTICE_FMT("Received %i bytes on pipe %i  %s %i", bytes, pipe,received.message,received.counter);
  //     //LOGGER_NOTICE_FMT("Role = %i  RadioNumber = %i", role, radioNumber);
  //     Serial.print(F("Received "));
  //     Serial.print(bytes);  // print the size of the payload
  //     Serial.print(F(" bytes on pipe "));
  //     Serial.print(pipe);  // print the pipe number
  //     Serial.print(F(": "));
  //     Serial.print(received.message);  // print incoming message
  //     Serial.print(received.counter);  // print incoming counter

  //     Serial.print(F(" Sent: "));
  //     Serial.print(payload.message);    // print outgoing message
  //     Serial.println(payload.counter);  // print outgoing counter

  //     // save incoming counter & increment for next outgoing
  //     payload.counter = received.counter + 1;
  //     // load the payload for the first received transmission on pipe 0
  //     nrf24.writeAckPayload(1, &payload, sizeof(payload));
  //   }
  //     }  // role

      // if (Serial.available()) {
      //   // change the role via the serial monitor

      //   char c = toupper(Serial.read());
      //   if (c == 'T' && !role) {
      //     // Become the TX node

      //     role = true;
      //     Serial.println("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK");

      //     memcpy(payload.message, "Hello ", 6);  // change payload message
      //     nrf24.stopListening();                 // this also discards any unused ACK payloads

      //   } else if (c == 'R' && role) {
      //     // Become the RX node

      //     role = false;
      //     Serial.println("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK");
      //     memcpy(payload.message, "World ", 6);  // change payload message

      //     // load the payload for the first received transmission on pipe 0
      //     nrf24.writeAckPayload(1, &payload, sizeof(payload));
      //     nrf24.startListening();
      //   }
      // }       
    } // end of update
};

#endif  // MY_RADIO_H
