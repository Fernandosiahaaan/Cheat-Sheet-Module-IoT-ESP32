#define MODBUS_SERVER

#include <Arduino.h>

#include "WiFi.h"
#include "general.h"
const char* ssid = "Create_Your_SSID_WiFi";      // Nama jaringan WiFi
const char* password = "Create_Your_Pass_WiFi";  // Kata sandi WiFi

#ifdef MODBUS_SERVER
// Variabel Modbus Server TCP
#include "serverTCP.hpp"
serverTCP modbusServer;
ModbusServerWiFi MBserver;
EventGroupHandle_t ModbusHandle;
uint16_t addr = 0, lenAddr = 0;  // Number of address to read & Start Lenght address to read
ModbusMessage respondOutput;
const uint16_t portTCP = 502;
const uint8_t maxClient = 1;
const uint16_t timeoutTCP = 20 * SECOND_TIME;  // 60 sec
const uint16_t respondModbusLen = 5;
TaskHandle_t TaskModbusServer;
#endif