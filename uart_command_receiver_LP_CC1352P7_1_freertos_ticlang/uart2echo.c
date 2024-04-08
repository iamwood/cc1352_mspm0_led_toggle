/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== uart2echo.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART2.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    uint8_t input;
    uint8_t inputLength = 0;
    uint8_t inputBuffer[50];
    const char echoPrompt[] = "Echoing characters:\r\n";
    const char newlinePart[] = "\r\n";
    UART2_Handle uart;
    UART2_Handle uartFrameEcho;
    UART2_Params uartParams;
    size_t bytesRead;
    size_t bytesWritten = 0;
    bool waitingForFrame = true;
    bool readingFrame = true;
    uint32_t status     = UART2_STATUS_SUCCESS;
    uint8_t turnOnCommand[]  = {0x81, 0x02, 0x36, 0xf9, 0xf7};
    uint8_t turnOffCommand[] = {0x81, 0x21, 0x45};
    uint8_t toggleCommand[]  = {0x82, 0x33, 0x24, 0x73};

    /* Call driver init functions */
    GPIO_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Create a UART where the default read and write mode is BLOCKING */
    UART2_Params_init(&uartParams);
    uartParams.baudRate = 115200;

    uart = UART2_open(CONFIG_UART2_0, &uartParams);
    uartFrameEcho = UART2_open(CONFIG_UART2_1, &uartParams);

    if (uart == NULL || uartFrameEcho == NULL)
    {
        /* UART2_open() failed */
        while (1) {}
    }

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Loop forever echoing */
    while (1)
    {
        while (waitingForFrame) {
            status = UART2_read(uart, &input, 1, &bytesRead);

            if (status != UART2_STATUS_SUCCESS) {
                /* UART2_read() failed */
                while (1) {}
            }

            if (input == 0x7e) {
                waitingForFrame = false;
            }
        }

        while (readingFrame) {
            status = UART2_read(uart, &input, 1, NULL);
            if (status != UART2_STATUS_SUCCESS) {
                /* UART2_read() failed */
                while (1) {}
            }
            if (input == 0x7e) {
                readingFrame = false;
            } else {
                inputBuffer[inputLength] = input;
                inputLength++;
            }
        }

        UART2_write(uartFrameEcho, inputBuffer, inputLength, NULL);

        if (inputLength == sizeof(turnOnCommand) && memcmp(inputBuffer, turnOnCommand, sizeof(turnOnCommand)) == 0) {
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
        } else if (inputLength == sizeof(turnOffCommand) && memcmp(inputBuffer, turnOffCommand, sizeof(turnOffCommand)) == 0) {
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
        } else if (inputLength == sizeof(toggleCommand) && memcmp(inputBuffer, toggleCommand, sizeof(toggleCommand)) == 0) {
            GPIO_toggle(CONFIG_GPIO_LED_0);
        }

        inputLength = 0;
        readingFrame = true;
        waitingForFrame = true;
    }
}
