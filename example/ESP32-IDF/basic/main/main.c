/**
 **********************************************************************************
 * @file   main.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  example code for MAX44009 Driver (for ESP32-IDF)
 **********************************************************************************
 *
 * Copyright (c) 2023 Mahda Embedded System (MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "MAX44009.h"
#include "MAX44009_platform.h"

static const char *TAG = "example";

void app_main(void)
{
  MAX44009_Handler_t Handler = {0};
  MAX44009_Sample_t  Sample = {0};

  ESP_LOGI(TAG, "MAX44009 Driver Example");

  MAX44009_Platform_Init(&Handler);
  MAX44009_Init(&Handler);
  MAX44009_SetAddressI2C(&Handler, 0);

  while (1)
  {
    MAX44009_ReadSample(&Handler, &Sample);
    ESP_LOGI(TAG, "Light Intensity: %fLux", Sample.Lux);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  MAX44009_DeInit(&Handler);
}
