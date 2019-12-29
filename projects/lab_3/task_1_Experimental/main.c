/*


Experimental version



*/

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include "main.h"


#define SWITCH_DELAY    ((uint32_t)2000000)

void delay(uint32_t value)
{
  for (uint32_t i=0; i<value;i++)
  {
    __NOP();	  
  }	  

}

void gpio_strct(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;   
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);                                                     
         
}

void pwm(void)   
{
	TIM_TimeBaseInitTypeDef TIM_Time_user;
	GPIO_InitTypeDef GPIO_Init_LED;
	TIM_OCInitTypeDef PWM_ini;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_Time_user.TIM_Prescaler = 168;
	TIM_Time_user.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Time_user.TIM_Period = 1000;
	TIM_Time_user.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM1, &TIM_Time_user);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_Init_LED.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_Init_LED);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
	
	PWM_ini.TIM_OCMode = TIM_OCMode_PWM1;
	PWM_ini.TIM_OutputState = TIM_OutputState_Enable;
	PWM_ini.TIM_Pulse = 0;
	PWM_ini.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	TIM_OC1Init(TIM1, &PWM_ini);	
	TIM_OC2Init(TIM1, &PWM_ini);
	TIM_OC3Init(TIM1, &PWM_ini);
	
	
	TIM_Cmd(TIM1, ENABLE);


	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
}
void SetSysClockTo168(void)
{
  int HSEStartUpStatus;
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig( RCC_HSE_ON);


  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

     RCC_PLLConfig(RCC_PLLSource_HSE,4,336,4,4);

     RCC_PLLCmd( ENABLE);

        /* Wait till PLL is ready */
     while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
     {
     }

     /* Select PLL as system clock source */
     RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

     /* Wait till PLL is used as system clock source */
     while (RCC_GetSYSCLKSource() != 0x08)
     {
     }

  }
}

int main(void)
{
  	

  	gpio_strct();
	pwm();
	
	//uint8_t i = 0;
	uint16_t c = 0;
	uint16_t time;


	while (1) {
	


	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 0)
	{
		time++;

		if (c == 0)
		{
			/*
				В данном случае яркость управляется одной кнопкой, изменяя период "20000" можно настроить гладкое изменение яркости или скачкообразное
				(светодиод будет моргать с помощью ШИМ). Изменяя pwm freq от 0 до 100 можно регулировать максимальную яркость, pwm freq = 50 => диод 
				достигает яркости в 50%
			*/
			uint16_t pwm_value1 = (cos(time * 6.28 / 20000) + 1) / 2.0f * (10000 / 50 - 1);
			// pwm_value1 = pwm_value1 +1;
			// if (pwm1 >= 1000) pwm_value1 = 0;
			TIM_SetCompare1(TIM1, pwm_value1);
		 }
		

	}
}
}



