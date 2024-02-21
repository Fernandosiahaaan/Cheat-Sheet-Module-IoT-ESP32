#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "driver/uart.h"
#include "esp_bt.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatt_common_api.h"
#include "esp_gatt_defs.h"
#include "esp_gattc_api.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"

#define TAG "BLE_SCAN"
uint16_t countData = 0;

static esp_ble_scan_params_t ble_scan_params = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval = 0x50,
    .scan_window = 0x30};

void increase_ble_tx_power() {
    esp_err_t errRc;

    // Set default BLE TX power level to P9 (9 dBm)
    errRc = esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
    if (errRc != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set default BLE TX power level: %s", esp_err_to_name(errRc));
        return;
    }

    // Set BLE advertising TX power level to P9 (9 dBm)
    errRc = esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
    if (errRc != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set BLE advertising TX power level: %s", esp_err_to_name(errRc));
        return;
    }

    // Set BLE scanning TX power level to P9 (9 dBm)
    errRc = esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN, ESP_PWR_LVL_P9);
    if (errRc != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set BLE scanning TX power level: %s", esp_err_to_name(errRc));
        return;
    }

    ESP_LOGI(TAG, "BLE TX power levels increased to P9 (9 dBm)");
}

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
    switch (event) {
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
        ESP_LOGI(TAG, "Scan parameters set, start scanning for 10 seconds...");
        esp_ble_gap_start_scanning(10);
        break;
    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
        if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(TAG, "Scan start failed, error status = %d", param->scan_start_cmpl.status);
        }
        break;
    case ESP_GAP_BLE_SCAN_RESULT_EVT: {
        esp_ble_gap_cb_param_t* scan_result = (esp_ble_gap_cb_param_t*)param;
        if (scan_result->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
            // Extract advertisement data
            uint8_t* adv_data = scan_result->scan_rst.ble_adv;
            uint8_t adv_data_len = scan_result->scan_rst.adv_data_len;

            // Search for device name in advertisement data
            uint8_t* name_data = esp_ble_resolve_adv_data(adv_data, ESP_BLE_AD_TYPE_NAME_CMPL, &adv_data_len);
            if (!name_data) {
                // Try to find short device name if complete name not found
                name_data = esp_ble_resolve_adv_data(adv_data, ESP_BLE_AD_TYPE_NAME_SHORT, &adv_data_len);
            }

            // Print device information if name found
            if (name_data) {
                char device_name[32];  // Assuming maximum name length of 32 bytes
                memcpy(device_name, name_data, adv_data_len);
                device_name[adv_data_len] = '\0';
                countData += 1;
                ESP_LOGI(TAG, "[%d] BLE device found, name=%s, addr_type=%d, mac-addr=%02x:%02x:%02x:%02x:%02x:%02x, rssi=%d",
                         countData,
                         device_name,
                         scan_result->scan_rst.ble_addr_type,
                         scan_result->scan_rst.bda[0], scan_result->scan_rst.bda[1],
                         scan_result->scan_rst.bda[2], scan_result->scan_rst.bda[3],
                         scan_result->scan_rst.bda[4], scan_result->scan_rst.bda[5],
                         scan_result->scan_rst.rssi);
            }
        }
        break;
    }
    case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
        if (param->scan_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(TAG, "Scan stop failed, error status = %d", param->scan_stop_cmpl.status);
        } else {
            ESP_LOGI(TAG, "Scan stopped");
        }
        break;
    default:
        break;
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "--EXAMPLE BLE SCAN--");
    esp_err_t ret;
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    nvs_flash_init();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth controller initialization failed, error code: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth controller enable failed, error code: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid initialization failed, error code: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid enable failed, error code: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "Starting BLE scan...");
    ret = esp_ble_gap_set_scan_params(&ble_scan_params);
    if (ret) {
        ESP_LOGE(TAG, "Set scan parameters failed, error code: %s", esp_err_to_name(ret));
        return;
    }
    increase_ble_tx_power();
    esp_ble_gap_register_callback(esp_gap_cb);

    // Wait indefinitely
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}