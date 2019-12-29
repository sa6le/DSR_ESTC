#ifndef MAIN_H__
#define MAIN_H__

#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>

void configure_leds(void);
void configure_buttons(void);
void configure_timers(void);
void SetSysClockTo168(void);


#endif