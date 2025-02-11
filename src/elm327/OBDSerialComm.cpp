#include "OBDSerialComm.h"


OBDSerialComm::OBDSerialComm(uint32_t baudRate, uint8_t rxPin, uint8_t txPin) 
 : serial(rxPin, txPin) {
    setBaudRate(baudRate);
    serial.begin(getBaudRate());
    serial.setTimeout(SERIAL_READ_TIMEOUT);
    setToDefaults();
}

OBDSerialComm::~OBDSerialComm() {
}


void OBDSerialComm::writeEnd() {

    // 1 - write carriage return
    //    writeTo(0x0D);
    writeTo("\n");
    //    writeTo(13);


    // 2- (optional ) write linefeed
    if (lineFeedEnable) {
        writeTo("\r");
        // writeTo(10);
    }

    // 3 - Write prompt
    //    writeTo(0x3E);
    writeTo('>');

    serial.flush();
};


void OBDSerialComm::writeEndOK() {
    writeTo("OK");
    writeEnd();
}

void OBDSerialComm::writeEndERROR() {
    writeTo("ERROR");
    writeEnd();
}

void OBDSerialComm::writeEndNoData() {
    writeTo("NO DATA");
    writeEnd();
}

void OBDSerialComm::writeEndUnknown() {
    writeTo("?");
    writeEnd();
}

void OBDSerialComm::setToDefaults() {
    setEcho(true);
    setStatus(READY);
    setWhiteSpaces(true);
    setHeaders(false);
    setLineFeeds(true);
    setMemory(false);
}

void OBDSerialComm::writeTo(char const *response) {
    serial.write(response);
}


void OBDSerialComm::writeTo(uint8_t cChar) {
    serial.write(cChar);
}

void OBDSerialComm::writeEndPidTo(char const *response) {
    if (whiteSpacesEnabled) {
        uint8_t len = strlen(response);
        char spacedResponse[len + len / 2 + 1];
        addSpacesToResponse(response, spacedResponse);
        writeTo(spacedResponse);
    } else {
        writeTo(response);
    }
    writeEnd();
}

size_t OBDSerialComm::readData(char *buff, size_t buff_size) {
    serial.flush(); // temp remove this
    size_t nbytes = serial.readBytesUntil(SERIAL_END_CHAR, buff, buff_size);
    buff[nbytes] = '\0';
    if (isEchoEnable()) {
        writeTo(buff);
    }
    return nbytes;
}

void OBDSerialComm::setBaudRate(uint32_t rate) {
    this->baudRate = rate;
}

long OBDSerialComm::getBaudRate() {
    return baudRate;
}

bool OBDSerialComm::isEchoEnable() {
    return echoEnable;
}

void OBDSerialComm::setEcho(bool echo) {
    this->echoEnable = echo;
}

void OBDSerialComm::setStatus(STATUS status) {
    this->status = status;
}

void OBDSerialComm::setLineFeeds(bool status) {
    this->lineFeedEnable = status;
}

void OBDSerialComm::setMemory(bool status) {
    this->memoryEnabled = status;
}

void OBDSerialComm::setWhiteSpaces(bool status) {
    this->whiteSpacesEnabled = status;
}

void OBDSerialComm::setHeaders(bool status) {
    this->headersEnabled = status;
}

void OBDSerialComm::addSpacesToResponse(const char *response, char spacedRes[]) {
    uint8_t len = strlen(response);
    int j = 0;
    for (int i = 0; i < len;) {
        *(spacedRes + j++) = *(response + i++);
        *(spacedRes + j++) = *(response + i++);
        if (i < len) {
            *(spacedRes + j++) = 0x20;
        }
    }
    *(spacedRes + j) = '\0';
}
