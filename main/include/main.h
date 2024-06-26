/***
** Created by Aleksey Volkov on 21.12.2019.
***/

#ifndef HV_CC_LED_DRIVER_RTOS_MAIN_H
#define HV_CC_LED_DRIVER_RTOS_MAIN_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#define FIRMWARE "1.11"
#define HARDWARE "ESP32-PICO-D4"

esp_err_t upgrade_firmware();

#endif //HV_CC_LED_DRIVER_RTOS_MAIN_H
