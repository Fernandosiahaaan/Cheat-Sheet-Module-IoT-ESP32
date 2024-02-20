/**
 * @file ServerTCP.hpp
 * @brief Modbus TCP Server Module
 * @author Fernando Siahaan
 *
 * This header file defines the interface for the Modbus TCP server module
 * The module provides functions and data
 * structures to handle Modbus TCP communication on the device.
 */

#ifndef __SERVER_TCP__
#define __SERVER_TCP__

#include <ESPmDNS.h>

#include "ModbusServerWiFi.h"

class serverTCP {
private:
    /* data */
    const char tcpTag[19] = "MODBUS-TCP-SERVER";
    const uint8_t MAX_DNSNAME_LENGTH = 20;
    const uint16_t errorValueAdress = 0xFFFF;
    const uint16_t maxPayload = 512;
    uint8_t adds(uint16_t data, uint16_t* outputArray, uint16_t lenghtArray);

public:
    serverTCP(/* args */);
    ~serverTCP();
    void getReplyServer(ModbusMessage respond);
    void getInformationRequest(ModbusMessage req, uint16_t* startAdress, uint16_t* lenghtAdress);
    uint16_t convertToArray(ModbusMessage input, uint8_t* output);
    void convertFromArray(ModbusMessage* output, uint8_t* input, uint16_t lenInput);
    void resultTCP(ModbusMessage* output, uint16_t* input, uint8_t lenInput, uint8_t serverID = 0x01, uint8_t functionalCode = 0x04);
    bool startDns(char* macAddress);
};
#endif