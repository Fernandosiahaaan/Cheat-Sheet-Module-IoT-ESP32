#include "main.hpp"

// put function declarations here:
void callbackDataModbus(ModbusMessage response, uint32_t token) {
    ESP_LOGI(tagModbus, "Response: serverID=%d, FC=%d, Token=%08X, length=%d:", response.getServerID(), response.getFunctionCode(), token, response.size());
    ESP_LOGI(tagModbus, "Response from Server = [");

    uint8_t arrayResponse[MAX_RESPONSE_LENGTH] = {0};
    uint8_t idx = 0;
    for (auto &byte : response) {
        arrayResponse[idx] = byte;
        Serial.printf("%02X ", byte);
        idx++;
    }
    Serial.printf("]\n");
}

void callbackErrorModbus(Error error, uint32_t token) {
    ModbusError mer(error);
    ESP_LOGE(tagModbus, "Error response: %s\n", (const char *)mer);
}

void connectToWiFi() {
    ESP_LOGI(initTag, "Connecting to WiFi...");
    WiFi.begin(ssid, password);  // Memulai koneksi ke WiFi

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);  // Tunggu sampai terhubung
        ESP_LOGI(initTag, "Connecting...");
    }

    ESP_LOGI(initTag, "Connected to WiFi");
    ESP_LOGI(initTag, "IP Address: %s", WiFi.localIP().toString());
}

void ModbusClientTask(void *parameter) {
    const uint16_t maxToken = 100;
    static unsigned long token = random(0, maxToken);  // generate token from random number
    for (;;) {
        ESP_LOGI(tagModbus, "===Get Modbus Data===");
        ESP_LOGI(tagModbus, "[MODBUSTCP] Heap memory before : %d bytes", esp_get_free_heap_size());
        TCPClient.setTarget(IPAddress(IP_CLIENT_1, IP_CLIENT_2, IP_CLIENT_3, IP_4_SERVER), MODBUS_TCP_PORT, MODBUS_TCP_TIMEOUT);
        Error err = TCPClient.addRequest((uint32_t)token, MODBUS_TCP_SERVER_ID, READ_INPUT_REGISTER, startAddress, countAddress);
        if (err != SUCCESS) {
            ModbusError erm(err);
        }
        ESP_LOGI(tagModbus, "[MODBUSTCP] Heap memory after  : %d bytes", esp_get_free_heap_size());
        vTaskDelay(pdMS_TO_TICKS(6 * SECOND_TIME));
    }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    connectToWiFi();
    IPAddress wIP = WiFi.localIP();
    Serial.printf("WIFi IP address: %u.%u.%u.%u\n", wIP[0], wIP[1], wIP[2], wIP[3]);

    // Modbus Client
    TCPClient.onDataHandler(&callbackDataModbus);
    TCPClient.onErrorHandler(&callbackErrorModbus);
    TCPClient.begin();

    xTaskCreate(  // task for mqtt
        ModbusClientTask,
        "ModbusClientTask",
        8 * ONE_KB_SIZE,
        NULL,
        2,
        NULL);
}

void loop() {
    // put your main code here, to run repeatedly:
}
