#ifndef _W5500_PORT_HAL_
#define _W5500_PORT_HAL_

#include "wizchip_conf.h"
#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

#define W5500_SPI_HANDLE    hspi1
#define W5500_CS_PORT       GPIOA
#define W5500_CS_PIN        GPIO_PIN_4
#define W5500_RST_PORT      GPIOB
#define W5500_RST_PIN       GPIO_PIN_11


extern SPI_HandleTypeDef W5500_SPI_HANDLE;

void W5500_ChipInit(void);
void do_tcpc(int sn);
void set_w5500_data(void);

#endif


