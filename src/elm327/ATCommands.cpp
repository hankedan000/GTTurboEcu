#include "ATCommands.h"


ATCommands::ATCommands(OBDSerialComm *connection)
 : _connection(connection) {
}


bool ATCommands::process(StringView command) {
    bool processed = false;
    if (isATCommand(command)) {
        command.dbgPrint("RX AT: ");
        processCommand(command);
        processed = true;
    }
    return processed;
}

void ATCommands::processCommand(const StringView & command) {

    // if space is enabled (ex: AT H[charPosition])
    uint8_t offset = 2;
    if (command.charAt(2) == 0x20) {
        offset = 3;
    }

    // refer to ELM327 specs
    StringView specificCommand = command.substring(offset, command.length() - 1);
    if (specificCommand.startsWith("D",offset)) {
        ATCommands::ATD();
    } else if (specificCommand.startsWith("Z")) {
        ATCommands::ATZ();
    } else if (specificCommand.startsWith("I")) {
        ATCommands::ATI();
    } else if (specificCommand.startsWith("E")) {
        ATCommands::ATEx(specificCommand);
    } else if (specificCommand.startsWith("L")) {
        ATCommands::ATLx(specificCommand);
    } else if (specificCommand.startsWith("M")) {
        ATCommands::ATMx(specificCommand);
    } else if (specificCommand.startsWith("S")) {
        if (specificCommand.startsWith("SP")) {
            ATCommands::ATSPx(specificCommand);
        } else {
            ATCommands::ATSx(specificCommand);
        }
    } else if (specificCommand.startsWith("H")) {
        ATCommands::ATHx(specificCommand);
    } else if (specificCommand.startsWith("AT")) {
        ATCommands::ATATx(specificCommand);
    } else if (specificCommand.startsWith("DPN")) {
        ATCommands::ATDPN();
    }  else if (specificCommand.startsWith("DESC") || specificCommand.startsWith("@1")) {
        ATCommands::ATDESC();
    } else if (specificCommand.startsWith("PC")) {
        ATCommands::ATPC();
    } else {

        // lets assume we process any at command
        _connection->writeEndOK();
    }
}

// set all to defaults
void ATCommands::ATD() {
    _connection->setToDefaults();
    _connection->writeTo("BUS INIT: ...");
    _connection->writeEndOK();

}


// reset all
void ATCommands::ATZ() {
    _connection->setEcho(true);
    _connection->setStatus(_connection->IDLE);
    _connection->writeTo(ID);
    _connection->writeEndOK();
}

// Print the version ID
void ATCommands::ATI() {
    _connection->setStatus(_connection->READY);
    _connection->writeTo(ID);
    _connection->writeEndOK();
}

// send description
void ATCommands::ATDESC() {
    _connection->writeTo(DESC);
    _connection->writeEndOK();
}

// set echoEnable 0=off 1=on
void ATCommands::ATEx(const StringView & cmd) {
    _connection->setEcho(cmd.equals("E0") ? false : true);
    _connection->writeEndOK();
}

// set memory off=0 on=1
void ATCommands::ATMx(const StringView & cmd) {
    _connection->setMemory(cmd.equals("M0") ? false : true);
    _connection->writeEndOK();
}

// line feeds off=0 on=1
void ATCommands::ATLx(const StringView & cmd) {
    _connection->setLineFeeds(cmd.equals("L0") ? false : true);
    _connection->writeEndOK();
}

// ATSx printing spaces off=0 on=1
void ATCommands::ATSx(const StringView & cmd) {
    _connection->setWhiteSpaces(cmd.equals("S0") ? false : true);
    _connection->writeEndOK();
}

// Headers off=0 on=1
void ATCommands::ATHx(const StringView & cmd) {
    _connection->setHeaders(cmd.equals("H0") ? false : true);
    _connection->writeEndOK();
}

// ATSPx Define protocol 0=auto
void ATCommands::ATSPx(const StringView & cmd) {
    _connection->writeEndOK();
}

// set protocol
void ATCommands::ATDPN() {
    _connection->writeTo(PROTOCOL);
    _connection->writeEndOK();
}

// AT AT2 adaptative time control
void ATCommands::ATATx(const StringView & cmd) {
    _connection->writeEndOK();
}

// Terminates current diagnostic session. Protocol close
void ATCommands::ATPC() {
    _connection->writeEndOK();
}

// return true ir connectionand is AT
bool ATCommands::isATCommand(const StringView & command) {
    return command.startsWith("AT") ? true : false;
}

