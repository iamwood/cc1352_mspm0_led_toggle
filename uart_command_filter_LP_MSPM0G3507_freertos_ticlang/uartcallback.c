/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 *  ======== UARTecho.c ========
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX Header files */
#include <semaphore.h>

#include "ti_drivers_config.h"

static sem_t appSema;
static volatile size_t numBytesRead;

uint8_t gBuffer[CONFIG_UART_BUFFER_LENGTH] = {0};
static volatile UART_Handle uartReceiver;
static volatile UART_Handle uartSender;
UART_Params uartReceiverParams;
UART_Params uartSenderParams;
static const uint8_t frameFlag = 0x7e;
void receiverReadCallbackFxn(UART_Handle handle, void *buffer, size_t count, void *userArg,
    int_fast16_t status);
void callbackFxnTx(UART_Handle handle, void *buffer, size_t count,
    void *userArg, int_fast16_t status);
/*
 *  ======== callbackFxn ========
 */
void receiverReadCallbackFxn(UART_Handle handle, void *buffer, size_t count, void *userArg,
    int_fast16_t status)
{
    if (status != UART_STATUS_SUCCESS) {
        /* RX error occured in UART_read() */
        while (1) {
        }
    }

    numBytesRead = count;
    sem_post(&appSema);
}

void senderWriteCallbackFxn(UART_Handle handle, void *buffer, size_t count, void *userArg,
    int_fast16_t status)
{
    if (status != UART_STATUS_SUCCESS) {
        /* RX error occured in UART_write() */
        while (1) {
        }
    }

    numBytesRead = count;
}
/*
 *  ======== callbackFxnTx ========
 */
void callbackFxnTx(UART_Handle handle, void *buffer, size_t count,
    void *userArg, int_fast16_t status)
{
    if (status != UART_STATUS_SUCCESS) {
        /* RX error occured in UART_read() */
        while (1) {
        }
    }
    /* if TX DMA is enabled and semaphore is not release by rx callback then enable below function */
    /* sem_post(&appSema); */
}
void uartInitReceiver(void)
{
    /* Initialise it with default parameters */
    UART_Params_init(&uartReceiverParams);
    uartReceiverParams.readMode     = UART_Mode_CALLBACK;
    uartReceiverParams.baudRate     = CONFIG_UART_BAUD_RATE;
    uartReceiverParams.readCallback = receiverReadCallbackFxn;

    uartReceiver = UART_open(CONFIG_UART_0, &uartReceiverParams);
    if (uartReceiver == NULL) {
        /* UART_open failed */
        __BKPT();
    }
}
void uartInitSender(void)
{
    /* Initialize it with default parameters */
    UART_Params_init(&uartSenderParams);
    uartSenderParams.readMode     = UART_Mode_CALLBACK;
    uartSenderParams.baudRate     = CONFIG_UART_BAUD_RATE;
    uartSenderParams.readCallback = senderWriteCallbackFxn;

    uartSender = UART_open(CONFIG_UART_1, &uartSenderParams);
    if (uartSender == NULL) {
        /* UART_open failed */
        __BKPT();
    }
}
void sendFrame(UART_Handle uartHandle, const void *frameBuffer, size_t frameSize)
{
    UART_write(uartHandle, &frameFlag, 1, NULL);
    UART_write(uartHandle, frameBuffer, frameSize, NULL);
    UART_write(uartHandle, &frameFlag, 1, NULL);
}

/*
 *  ======== mainThread ========
 */
void mainThread(void *arg0)
{
    size_t rdCount          = 0;
    size_t wrCount          = 0;
    bool reading = true;
    uint8_t inputLength = 0;
    char inputBuffer[50];
    const char echoPrompt[] = "Echoing characters:\r\n";
    int32_t semStatus;
    uint32_t status   = UART_STATUS_SUCCESS;
    static char input = 0;
    const char newlineOutput[] = "\r\n";
    char commandStrings[3][7] = {"on", "off", "toggle"};
    uint8_t turnOnCommand[]  = {0x81, 0x02, 0x36, 0xf9, 0xf7};
    uint8_t turnOffCommand[] = {0x81, 0x21, 0x45};
    uint8_t toggleCommand[]  = {0x82, 0x33, 0x24, 0x73};

    /* Create semaphore */
    semStatus = sem_init(&appSema, 0, 0);

    if (semStatus != 0) {
        /* Error creating semaphore */
        while (1) {
        }
    }

    uartInitReceiver();
    uartInitSender();

    /* Turn on user LED to indicate successful initialization */
    DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);

    UART_write(uartReceiver, &echoPrompt, sizeof(echoPrompt), NULL);

    /* Loop forever echoing */
    while (1) {
        numBytesRead = 0;

        reading = true;

        while (reading) {
            status = UART_read(uartReceiver, &input, 1, NULL);
            sem_wait(&appSema);
            if (input == '\r') {
                reading = false;
                UART_write(uartReceiver, &newlineOutput, sizeof(newlineOutput), NULL);
            } else {
                inputBuffer[inputLength] = input;
                inputLength++;

                status = UART_write(uartReceiver, &input, 1, NULL);
                if (status != UART_STATUS_SUCCESS) {
                    /* UART_write() failed */
                    while (1) {
                    }
                }
            }
        }

        if (inputLength == 2 && memcmp(inputBuffer, commandStrings[0], 2) == 0) {
            sendFrame(uartSender, &turnOnCommand, sizeof(turnOnCommand));
        } else if (inputLength == 3 && memcmp(inputBuffer, commandStrings[1], 3) == 0) {
            sendFrame(uartSender, &turnOffCommand, sizeof(turnOffCommand));
        } else if (inputLength == 6 && memcmp(inputBuffer, commandStrings[2], 6) == 0) {
            sendFrame(uartSender, &toggleCommand, sizeof(toggleCommand));
        }

//        sendFrame(uartSender, &inputBuffer, inputLength);

//        UART_write(uartSender, &frameFlag, 1, NULL);
//        UART_write(uartSender, &inputBuffer, inputLength, NULL);
//        UART_write(uartSender, &frameFlag, 1, NULL);

        inputLength = 0;
    }
}
