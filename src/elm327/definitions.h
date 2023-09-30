//
// Created by dragon on 12-05-2016.
//

#ifndef GTTURBOECU_DEFINITIONS_H
#define GTTURBOECU_DEFINITIONS_H

#include <stdint.h>


extern HardwareSerial Serial;


#define DO_DEBUG true
#define DEBUG(x) do {if (DO_DEBUG) { Serial.println(x); } } while (0)


#define xtoc(x) ((x < 10) ? ('0' + x) : ('A' - 10 + x))
#define getNumOfHexChars(nBytes) (nBytes * 2)

#define PID_N_BYTES 2
#define N_CHARS_IN_BYTE 2


// Device ID
#define ID  "ELM327 - GTTurboECU V0.0.1"
#define DESC  "GTTurbo OBD2 Arduino libs, based on ELM"
#define PROTOCOL "6" // canbus 500k 11 bit protocol id for elm.

// Char representing end of serial string
#define SERIAL_END_CHAR  0x0D
#define SERIAL_READ_TIMEOUT 20000L


const uint8_t maxPid = 0xFF;

const uint8_t N_MODE01_INTERVALS = 7;
const uint8_t PID_INTERVAL_OFFSET = 0x20;

using mode_t = uint8_t;
using pid_t = uint16_t;

// type capable of holding a string with mode and pid formatted in HEX
using ModeWithPID_HexString = char[7];

struct ModeWithPID {
    ModeWithPID()
     : mode(0xFF)
     , pid(0xFFFF)
    {}

    ModeWithPID(uint32_t mwp)
    {
        if (mwp > (uint32_t)(0xFFFF)) {
            mode = mwp >> 16;
            pid = mwp & 0xFFFF;
        } else {
            mode = mwp >> 8;
            pid = mwp & 0xFF;
        }
    }

    void toString(ModeWithPID_HexString &out) {
        if (pid > 0xFFu) {
            sprintf(out,"%02X%04X",mode,pid);
        } else {
            sprintf(out,"%02X%02X",mode,pid&0xFF);
        }
    }

    void toRespString(ModeWithPID_HexString &out) {
        if (pid > 0xFFu) {
            sprintf(out,"4%X%04X",mode&0xF,pid);
        } else {
            sprintf(out,"4%X%02X",mode&0xF,pid&0xFF);
        }
    }

    mode_t mode;
    pid_t pid;
};

#endif //GTTURBOECU_DEFINITIONS_H
