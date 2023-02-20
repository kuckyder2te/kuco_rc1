#pragma once
#ifndef MY_RADIO_H
#define MY_RADIO_H

#include <TaskManager.h>
#include <RF24.h>
#include <../lib/myLogger.h>

//#define LOCAL_DEBUG
#include "myLogger.h"

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
} RX_payload_t;

typedef struct __attribute__((__packed__))
{
    float yaw;          // Fluglage via MPU9250
    float pitch;
    float roll;
    uint16_t altitude;      // Höhe via BMP280  
    uint16_t sonic;         // US Sensor
    float temperature;   // MPU9250
    float pressure;     // BMP280
} TX_payload_t;

typedef struct
{
    bool isconnect;
    RX_payload_t RX_payload; 
} RC_interface_t;

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


    // optional (you can remove this method)
    // virtual void begin() override {
    // }

    // optional (you can remove this method)
    // virtual void enter() override {
    // }

    virtual void update() override {
    }

    // optional (you can remove this method)
    // virtual void exit() override {
    // }

    // optional (you can remove this method)
    // virtual void idle() override {
    // }

    // optional (you can remove this method)
    // virtual void reset() override {
    // }
};

#endif  // MY_RADIO_H
