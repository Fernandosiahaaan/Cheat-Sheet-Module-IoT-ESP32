#include "serverTCP.hpp"

serverTCP::serverTCP(/* args */) {
}

serverTCP::~serverTCP() {
}

/**
 * @brief Show payload data modbus request
 * @param req input param request data from client
 * @param startAdress input param start adress sensor request (0x00)
 * @param lengthAdress input param length request data sensor (0x31)
 */
void serverTCP::getInformationRequest(ModbusMessage req, uint16_t *startAdress, uint16_t *lenghtAdress) {
    int cx = 0;
    int cTotal = 0;
    char dataPost[maxPayload] = "";
    for (uint16_t len = 0; len < req.size(); len++) {
        cx = snprintf(dataPost + cTotal, maxPayload - cTotal - 1, " %02x", req[len]);
        cTotal += cx;
    }
    ESP_LOGI(tcpTag, "Request From Client(S-Controller) = [%s]", dataPost);
    req.get(2, *startAdress), req.get(4, *lenghtAdress);
    ESP_LOGI(tcpTag, "Addr = %d; Wrds =  %d", *startAdress, *lenghtAdress);
}

/**
 * @brief Show result payload data modbus respond
 * @param respond input/output param respond data server
 */
void serverTCP::getReplyServer(ModbusMessage respond) {
    int cx = 0;
    int cTotal = 0;
    char dataPost[maxPayload] = "";
    for (uint16_t len = 0; len < respond.size(); len++) {
        cx = snprintf(dataPost + cTotal, maxPayload - cTotal - 1, " %02x", respond[len]);
        cTotal += cx;
    }
    ESP_LOGI(tcpTag, "Response by Server(S-Monitor) = [%s]", dataPost);
}

/**
 * @brief Convert data from modbusmessage to array
 * @param input input param modbus message data
 * @param output output param array uint8_t data
 * @return lenght array output
 */
uint16_t serverTCP::convertToArray(ModbusMessage input, uint8_t *output) {
    for (uint16_t len = 0; len < input.size(); len++) {
        output[len] = input[len];
    }
    return input.size();
}

/**
 * @brief Convert data from array to modbusmessage
 * @param input input param array uint8_t data
 * @param lenInput input param lenght of array
 * @param output output param modbus message data
 */
void serverTCP::convertFromArray(ModbusMessage *output, uint8_t *input, uint16_t lenInput) {
    output->clear();
    for (uint16_t len = 0; len < lenInput; len++) {
        output->add(input[len]);
    }
}

/**
 * @brief Create result respond data (ServerId,FunctionalCode,lenght,AdressDataSensor)
 * @param output output param modbus message data
 * @param input input param array uint8_t data
 * @param lenInput input param lenght of array
 * @param serverID input param id Server (Discrete Coils/Flags)
 * @param functionalCode input param Register Mapping (Example : Read Holding Registers, Read input Registers, etc)
 */
void serverTCP::resultTCP(ModbusMessage *output, uint16_t *input, uint8_t lenInput, uint8_t serverID, uint8_t functionalCode) {
    output->clear();
    output->add(serverID, functionalCode, lenInput);
    for (uint16_t len = 0; len < lenInput; len++) {
        output->add(input[len]);
    }
}

/**
 * @brief Add data to outputArray
 * @param data input param data will set to outputArray
 * @param outputArray output param array data respond
 * @param lenghtArray input param position of data in outputArray
 * @return lenght of Data in outputArray
 */
uint8_t serverTCP::adds(uint16_t data, uint16_t *outputArray, uint16_t lenghtArray) {
    outputArray[lenghtArray] = data;
    return 1;
}

/**
 * @brief Starts the DNS server with a given MAC address.
 * @param macAddress The MAC address used to generate the DNS server name.
 * @return true if the DNS server is started successfully, false otherwise.
 */
bool serverTCP::startDns(char *macAddress) {
    char dnsServerName[MAX_DNSNAME_LENGTH] = "";
    if (snprintf(dnsServerName, MAX_DNSNAME_LENGTH - 1, "esp32_%s", macAddress) > 0) {
        if (!MDNS.begin(dnsServerName)) {
            ESP_LOGE(tcpTag, "Error starting mDNS");
            return false;
        }
        IPAddress lIP = WiFi.localIP();
        ESP_LOGI(tcpTag, "IP address: %u.%u.%u.%u", lIP[0], lIP[1], lIP[2], lIP[3]);
        ESP_LOGI(tcpTag, "mDNS: %s", dnsServerName);
    } else {
        return false;
    }

    return true;
}