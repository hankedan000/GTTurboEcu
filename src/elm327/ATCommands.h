#ifndef GTTURBOECU_ATCommands_h
#define GTTURBOECU_ATCommands_h


#include <Arduino.h>
#include "OBDSerialComm.h"
#include "StringView.h"

class ATCommands {

public:
    ATCommands(OBDSerialComm *connection);

    ~ATCommands();

    bool process(StringView string);

private:

    // Variables
    OBDSerialComm *connection;

    void ATD();

    void ATZ();

    void ATI();

    void ATEx(const StringView &x);

    void ATMx(const StringView & x);

    void ATLx(const StringView & x);

    void ATSx(const StringView & x);

    void ATHx(const StringView & x);

    void ATSPx(const StringView & x);

    void ATATx(const StringView & cmd);

    void ATPC();

    void ATDPN();

    void ATDESC();

    void processCommand(const StringView & command);

    bool isATCommand(const StringView & command);

};

#endif

