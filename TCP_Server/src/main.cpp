#include "main.hpp"

#ifdef MODBUS_SERVER
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

// @brief Callback modbus tcp if have message request.
ModbusMessage callbackServer(ModbusMessage request) {
    const char tcpTag[12] = "TCP-SERVER";
    respondOutput.clear();
    // Start Task
    ESP_LOGI(tcpTag, "-----EXAMPLE MODBUS TCP SERVER-----");
    modbusServer.getInformationRequest(request, &addr, &lenAddr);

    // Check Adress and Length Valid/not ?
    if ((addr > respondModbusLen) || ((addr + lenAddr) > respondModbusLen)) {
        respondOutput.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);  // No. Return error response
        return respondOutput;
    }

    xEventGroupSetBits(ModbusHandle, 1);  // send to task ProcessRespondModbus
    // Waiting data from task GeneratorRespondModbus
    EventBits_t bitHandler = xEventGroupWaitBits(ModbusHandle, 2, pdTRUE, pdFALSE, portMAX_DELAY);
    if (bitHandler & 2) {
        modbusServer.getReplyServer(respondOutput);
        ESP_LOGI(tcpTag, "RAM : %d", ESP.getFreeHeap());
        return respondOutput;
    }
}

// @brief Generator Output modbus message response.
void ProcessRespondModbus(void *pvParameters) {
    uint16_t exampleRespondGenerator[respondModbusLen] = {3, 1, 2, 3};
    for (;;) {
        EventBits_t bitTask = xEventGroupWaitBits(ModbusHandle, 1, pdTRUE, pdFALSE, portMAX_DELAY);
        if (bitTask & 1) {
            // Process generate response to send callbackServer
            modbusServer.resultTCP(&respondOutput, exampleRespondGenerator, respondModbusLen, 0x01, 0x04);

            // Send event & end atask
            xEventGroupSetBits(ModbusHandle, 2);
            xEventGroupClearBits(ModbusHandle, 2);
            ESP_LOGI("GeneratorRespondModbus", "RAM : %d", ESP.getFreeHeap());
        }
    }
}
#endif

void setup() {
    // put your setup code here, to run once:
    Serial.begin(ESP32_BAUDRATE);
    ESP_LOGW(initTag, "--INIT START--");
    ESP_LOGW(initTag, "Heap Start Setup = %d", ESP.getFreeHeap());
    connectToWiFi();

#ifdef MODBUS_SERVER
    // rewrite mac adress device
    const uint8_t macRawLen = 6, macLen = 13;
    uint8_t macRaw[macRawLen];
    esp_efuse_mac_get_default(macRaw);
    esp_read_mac(macRaw, ESP_MAC_WIFI_STA);
    char macAddr[macLen] = "";
    snprintf(macAddr, macLen, "%02x%02x%02x%02x%02x%02x", macRaw[0], macRaw[1], macRaw[2], macRaw[3], macRaw[4], macRaw[5]);

    // Init Modbus Server
    ESP_LOGW(initTag, "---MODBUS TCP/IP SERVER---");
    const uint8_t MY_SERVER(1);
    if (modbusServer.startDns(&macAddr[0])) {
        ESP_LOGI(initTag, "Succesfully start DNS");
    } else {
        ESP_LOGE(initTag, "Failed to start DNS; macAddr");
    }
    MBserver.registerWorker(MY_SERVER, READ_INPUT_REGISTER, &callbackServer);
    MBserver.start(portTCP, maxClient, timeoutTCP);
    ModbusHandle = xEventGroupCreate();

    // create task process data modbus
    xTaskCreatePinnedToCore(
        ProcessRespondModbus,   /* Task function. */
        "ProcessRespondModbus", /* name of task. */
        ONE_KB_SIZE * 4,        /* Stack size of task */
        NULL,                   /* parameter of the task */
        1,                      /* priority of the task */
        &TaskModbusServer,      /* Task handle to keep track of created task */
        1);                     /* pin task to core 1 */

#endif

    ESP_LOGI(initTag, "Heap End Setup = %d", ESP.getFreeHeap());
    ESP_LOGW(initTag, "***INITIALIZATION COMPLETE***");
}

void loop() {
    // put your main code here, to run repeatedly:
}