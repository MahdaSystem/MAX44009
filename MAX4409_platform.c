/**
 **********************************************************************************
 * @file   MAX44009_platform.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  MAX44009 Ambient Light sensor driver platform dependent part
 **********************************************************************************
 *
 * Copyright (c) 2021 Hossein.M (MIT License)
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

/* Includes ---------------------------------------------------------------------*/
#include "MAX44009_platform.h"

#if defined(DS1307_PLATFORM_AVR)
#include <avr/io.h>
#elif defined(DS1307_PLATFORM_ESP32_IDF)
#include "sdkconfig.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "main.h"
#include "TR10_I2C.h"
#endif



/**
 ==================================================================================
                        ##### Private Functions #####                              
 ==================================================================================
 */

static void
Platform_Init(void)
{
#if defined(MAX44009_PLATFORM_AVR)
  TWBR = (uint8_t)(MAX44009_CPU_CLK - 1600000) / (2 * 100000);
#elif defined(MAX44009_PLATFORM_ESP32_IDF)
  I2C_Initialize(0, 100000);
#endif
}


static void
Platform_DeInit(void)
{
#if defined(MAX44009_PLATFORM_AVR)
#elif defined(MAX44009_PLATFORM_ESP32_IDF)
  I2C_Uninitialize(0);
#endif
}


static void
Platform_Send(uint8_t Address, uint8_t *Data, uint8_t Len)
{
#if defined(MAX44009_PLATFORM_AVR)
  uint8_t DataCounter = 0;

  TWCR = _BV(TWEN) | _BV(TWSTA) | _BV(TWEA) | _BV(TWINT); // TWI enable *** acknowledge enable
  while (!CHECKBIT(TWCR, TWINT)); // wait until the process ends

  TWDR = Address<<1;                  // set data in data register to sending
  TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT); // TWI enable *** acknowledge enable
  while (!CHECKBIT(TWCR, TWINT));

  for (DataCounter = 0; DataCounter < Len; DataCounter++)
  {
    TWDR = Data[DataCounter];                  // set data in data register to sending
    TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT); // TWI enable *** acknowledge enable
    while (!CHECKBIT(TWCR, TWINT));
  }
  
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO); // send the STOP mode bit

#elif defined(MAX44009_PLATFORM_ESP32_IDF)
  I2C_Send(0, Address, Data, Len);
#endif
}

static void
PlatformReceive(uint8_t Address, uint8_t *Data, uint8_t Len)
{
#if defined(MAX44009_PLATFORM_AVR)
  uint8_t DataCounter = 0;

  TWCR = _BV(TWEN) | _BV(TWSTA) | _BV(TWEA) | _BV(TWINT); // TWI enable *** acknowledge enable
  while (!CHECKBIT(TWCR, TWINT)); // wait until the process ends

  TWDR = (Address<<1) | 0x01;                  // set data in data register to sending
  TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT); // TWI enable *** acknowledge enable
  while (!CHECKBIT(TWCR, TWINT)); // wait until the process ends

  for (DataCounter = 0; DataCounter < Len - 1; DataCounter++)
  {
    TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT); // TWI enable *** acknowledge enable
    while (!CHECKBIT(TWCR, TWINT)); // wait until the process ends
    Data[DataCounter] = TWDR;
  }
  TWCR = _BV(TWEN) | _BV(TWINT); // TWI enable
  while (!CHECKBIT(TWCR, TWINT)); // wait until the process ends
  Data[DataCounter] = TWDR;

  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO); // send the STOP mode bit
#elif defined(MAX44009_PLATFORM_ESP32_IDF)
  I2C_Receive(0, Address, Data, Len);
#endif
}



/**
 ==================================================================================
                         ##### Public Functions #####                              
 ==================================================================================
 */

MAX44009_Result_t
MAX44009_Platform_Init(MAX44009_Handler_t *Handler)
{
  Handler->PlatformInit = Platform_Init;
  Handler->PlatformDeInit = Platform_DeInit;
  Handler->PlatformSend = Platform_Send;
  Handler->PlatformReceive = PlatformReceive;
  return MAX44009_OK;
}
