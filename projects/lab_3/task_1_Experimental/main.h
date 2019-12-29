#ifndef LEDS_AND_BUTTON_H
#define LEDS_AND_BUTTON_H

#include <stm32f4xx.h>

/* #if defined (USE_STM324xG_EVAL) */
/*   #include "stm324xg_eval.h" */

/* #elif defined (USE_STM32F4DISC_EVAL) */
/*   #include "stm32f4disc_eval.h" */

/* #else */
/*  #error "Please select first the Evaluation board used in your application (in Project Options)" */
/* #endif */
void timer_ini(void);
void TIM2_IRQHandler(void);
void led_board(void);
void init_gpio(void);
void tim_init(void);
void init_it(void);
void EXTI0_IRQHandler(void);
void pwm_power(void);
void gpio_ini(void);

#include <math.h>

#endif  /* LEDS_AND_BUTTON_H */