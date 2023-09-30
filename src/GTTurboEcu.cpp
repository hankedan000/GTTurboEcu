#include "GTTurboEcu.h"

void toUpperCase(char *str, size_t nBytes) {
    for (size_t i=0; i<nBytes; i++) {
        str[i] = toupper(str[i]);
    }
}

GTTurboEcu::GTTurboEcu(uint32_t baudRate, uint8_t rxPin, uint8_t txPin)
 : _connection(baudRate, rxPin, txPin)
 , _atProcessor(&_connection)
 , _pidProcessor(&_connection) {
    _currCommand[0] = '\0';
    _lastCommand[0] = '\0';
}

GTTurboEcu::~GTTurboEcu() {
}


ModeWithPID GTTurboEcu::readPidRequest() {
    size_t nBytesRead = 0;
    do {
        nBytesRead = _connection.readData(_currCommand, MAX_CMD_LENGTH);
        toUpperCase(_currCommand, nBytesRead);
        // TODO ignore spaces, and all control chars (tab, etc)
        // TODO accept single carriage return as repeat last command at or pid
        // ignore null i.e 00
    } while (processResponse(StringView(_currCommand, nBytesRead)));
    const uint32_t mwpU32 = strtol(_currCommand, NULL, 16);
    return ModeWithPID(mwpU32);
}



bool GTTurboEcu::registerMode01Pid(uint32_t pid) {
    _pidProcessor.registerMode01Pid(pid);
}


void GTTurboEcu::writePidNotSupported() {
    _connection.writeEndNoData();
}


void GTTurboEcu::writePidResponse(ModeWithPID requestPid, uint8_t numberOfBytes, uint32_t value) {
    _pidProcessor.writePidResponse(requestPid, numberOfBytes, value);
}

bool GTTurboEcu::processResponse(StringView command) {

    //TODO: check for no 0X0D char in command, return ? and >
    //TODO: receive only <cr>, repeat last command

    //DEBUG("request: " + command);

    // carriage return, means repeat last command
    if (command == "\n") {
        command = StringView(_lastCommand);
    }

    // empty command, do not send to user
    if (command.length() == 0) {
        return true;
    }

    // TODO: check if this is repeated
    if(_atProcessor.process(command)) {
        strncpy(_lastCommand, command.data(), min(MAX_CMD_LENGTH, command.length()));
        return true;
    }

    // check if is valid hex command
    if(!isValidHex(command)) {
        _connection.writeEndUnknown();
        command.dbgPrint("Invalid HEX command: ");
        return true;
    }

    // TODO: check if this is repeated
    if(_pidProcessor.process(command)) {
        strncpy(_lastCommand, command.data(), min(MAX_CMD_LENGTH, command.length()));
        return true;
    }

    return false;
}


bool GTTurboEcu::isValidHex(const StringView &s) {
    for (size_t i=0; i<s.length(); i++) {
        char c = s.charAt(i);
        if (c >= '0' && c<= '9') {
            continue;
        } else if (c >= 'A' && c <= 'F') {
            continue;
        } else if (c >= 'a' && c <= 'f') {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

