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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
 *  by the SysConfig tool.
 */

#include "ti_drivers_config.h"

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations */
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_SYSCTL_CLK_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_UART_reset(CONFIG_UART_0_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_UART_enablePower(CONFIG_UART_0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{
    DL_GPIO_initDigitalOutput(GPIO_GRP_0_PIN_0_IOMUX);

    DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
}

SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setPowerPolicyRUN0SLEEP0();
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
}

SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_CLK_init(void)
{
    while ((DL_SYSCTL_getClockStatus() & (DL_SYSCTL_CLK_STATUS_LFOSC_GOOD)) !=
           (DL_SYSCTL_CLK_STATUS_LFOSC_GOOD)) {
        ;
    }
}
/*
 *  =============================== DMA ===============================
 */
#include <ti/drivers/dma/DMAMSPM0.h>
const uint_least8_t CONFIG_DMA_0               = 0;
const uint_least8_t DMA_Count                  = CONFIG_DMA_COUNT;
DMAMSPM0_Object DMAObject[CONFIG_DMA_CH_COUNT] = {
    {.dmaTransfer =
            {
                .txTrigger              = DMA_UART0_TX_TRIG,
                .txTriggerType          = DL_DMA_TRIGGER_TYPE_EXTERNAL,
                .rxTrigger              = DMA_UART0_RX_TRIG,
                .rxTriggerType          = DL_DMA_TRIGGER_TYPE_EXTERNAL,
                .transferMode           = DL_DMA_SINGLE_TRANSFER_MODE,
                .extendedMode           = DL_DMA_NORMAL_MODE,
                .destWidth              = DL_DMA_WIDTH_BYTE,
                .srcWidth               = DL_DMA_WIDTH_BYTE,
                .destIncrement          = DL_DMA_ADDR_INCREMENT,
                .dmaChannel             = 0,
                .dmaTransferSource      = NULL,
                .dmaTransferDestination = NULL,
                .enableDMAISR           = false,
            }},
};

static const DMAMSPM0_HWAttrs DMAMSP0HWAttrs[CONFIG_DMA_COUNT] = {
    {
        .dmaIsrFxn          = NULL,
        .intPriority        = DEFAULT_DMA_PRIORITY,
        .roundRobinPriority = 0,
    },
};
const DMAMSPM0_Cfg DMAMSPM0_Config[CONFIG_DMA_COUNT] = {
    {
        &DMAMSP0HWAttrs[CONFIG_DMA_0],
        &DMAObject[CONFIG_DMA_0],
    },
};

/*
 *  =============================== UART ===============================
 */
#include <ti/drivers/uart/UARTMSPM0G1X0X_G3X0X.h>

const uint_least8_t CONFIG_UART_0 = 0;
const uint_least8_t CONFIG_UART_1 = 1;
const uint_least8_t UART_count    = CONFIG_UART_COUNT;

static const UARTMSP_HWAttrs UARTMSPHWAttrs[CONFIG_UART_COUNT] = {
    {
        .regs          = UART0,
        .irq           = UART0_INT_IRQn,
        .rxPin         = IOMUX_PINCM6, /* 17 */      // 22 I want this to be PA31 (PINCM6)
        .rxPinFunction = IOMUX_PINCM6_PF_UART0_RX,   // 22
        .txPin         = IOMUX_PINCM3, /* 16 */      // 21
        .txPinFunction = IOMUX_PINCM3_PF_UART0_TX,   // 21 I want this to be PA28 (PINCM3)
        .mode          = DL_UART_MODE_NORMAL,
        .direction     = DL_UART_DIRECTION_TX_RX,
        .flowControl   = DL_UART_FLOW_CONTROL_NONE,
        .clockSource   = DL_UART_CLOCK_BUSCLK,
        .clockDivider  = DL_UART_CLOCK_DIVIDE_RATIO_4,
        .rxIntFifoThr  = DL_UART_RX_FIFO_LEVEL_ONE_ENTRY,
        .txIntFifoThr  = DL_UART_TX_FIFO_LEVEL_EMPTY,
    },
    {
        .regs          = UART1,
        .irq           = UART1_INT_IRQn,
        .rxPin         = IOMUX_PINCM24, /* PB7 */     // 24 I want this to be PB7 (PINCM24)
        .rxPinFunction = IOMUX_PINCM24_PF_UART1_RX,   // 24
        .txPin         = IOMUX_PINCM23, /* PB6 */     // 23
        .txPinFunction = IOMUX_PINCM23_PF_UART1_TX,   // 23 I want this to be PB6 (PINCM23)
        .mode          = DL_UART_MODE_NORMAL,
        .direction     = DL_UART_DIRECTION_TX_RX,
        .flowControl   = DL_UART_FLOW_CONTROL_NONE,
        .clockSource   = DL_UART_CLOCK_BUSCLK,
        .clockDivider  = DL_UART_CLOCK_DIVIDE_RATIO_4,
        .rxIntFifoThr  = DL_UART_RX_FIFO_LEVEL_ONE_ENTRY,
        .txIntFifoThr  = DL_UART_TX_FIFO_LEVEL_EMPTY,
    },
};

UART_Data_Object UARTObject[CONFIG_UART_COUNT] = {
    {
        .object =
            {
                .supportFxns        = &UARTMSPSupportFxns,
                .buffersSupported   = true,
                .eventsSupported    = false,
                .callbacksSupported = true,
                .dmaSupported       = true,
                .noOfDMAChannels    = 1,
            },
        .buffersObject =
            {
                .rxBufPtr  = rxBuffer,
                .txBufPtr  = txBuffer,
                .rxBufSize = sizeof(rxBuffer),
                .txBufSize = sizeof(txBuffer),
            },
    },
    {
         .object =
             {
                 .supportFxns        = &UARTMSPSupportFxns,
                 .buffersSupported   = true,
                 .eventsSupported    = false,
                 .callbacksSupported = true,
                 .dmaSupported       = true,
                 .noOfDMAChannels    = 1,
             },
         .buffersObject =
             {
                 .rxBufPtr  = rx1Buffer,
                 .txBufPtr  = tx1Buffer,
                 .rxBufSize = sizeof(rx1Buffer),
                 .txBufSize = sizeof(tx1Buffer),
             },
    }
};

const UART_Config UART_config[CONFIG_UART_COUNT] = {
    {
        &UARTObject[CONFIG_UART_0],
        &UARTMSPHWAttrs[CONFIG_UART_0],
    },
    {
        &UARTObject[CONFIG_UART_1],
        &UARTMSPHWAttrs[CONFIG_UART_1],
    },
};

/*****************************************************************************************************************/

void UART0_IRQHandler(void)
{
    UARTMSP_interruptHandler((UART_Handle) &UART_config[CONFIG_UART_0]);
}

void UART1_IRQHandler(void)
{
    UARTMSP_interruptHandler((UART_Handle) &UART_config[CONFIG_UART_1]);
}

void UARTMSP_eventCallback(
    UART_Handle handle, uint32_t event, uint32_t data, void *userArg)
{
}
