#include <Arduino.h>

#include "ModbusClientTCP.h"
#include "WiFi.h"
#include "general.h"

#define IP_CLIENT_1 192
#define IP_CLIENT_2 168
#define IP_CLIENT_3 1
#define IP_CLIENT_4 101
#define IP_4_SERVER 100

#define MODBUS_TCP_PORT 502
#define MODBUS_TCP_SERVER_ID 1
#define MODBUS_TCP_TIMEOUT 60000
#define MAX_RESPONSE_LENGTH 255
const char tagModbus[20] = "MODBUS-TCP-CLIENT";
const char* ssid = "XXXXXXX";      // Nama jaringan WiFi
const char* password = "XXXXXXX";  // Kata sandi WiFi
uint8_t startAddress = 1;
uint8_t countAddress = 3;

WiFiClient ModbusTCPClient;
IPAddress ModbusTCPDefaultIP = {IP_CLIENT_1, IP_CLIENT_2, IP_CLIENT_3, IP_CLIENT_4};
ModbusClientTCP TCPClient(ModbusTCPClient, ModbusTCPDefaultIP, MODBUS_TCP_PORT);
