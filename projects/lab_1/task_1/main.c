
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>

void delay(uint32_t value);
void SetSysClockTo84(void);

#define SWITCH_DELAY    ((uint32_t)2000000)

void delay(uint32_t value)
{
  for (uint32_t i=0; i<value;i++)
  {
    __NOP();	  
  }	  

}

int main(void)
{ 


  SetSysClockTo84();

  GPIO_InitTypeDef GPIO_InitStructure;


  /* Enable peripheral clock for LEDs and buttons port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  while (1)
  {
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);

  if (RCC_Clocks.SYSCLK_Frequency == 84000000 )
  {
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
  }
  else
  {
    GPIO_SetBits(GPIOD, GPIO_Pin_14);	  
  }
  }

}

void SetSysClockTo84(void)
{
  int HSEStartUpStatus;	
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig( RCC_HSE_ON);
  
  
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    
     RCC_PLLConfig(RCC_PLLSource_HSE, 8,336,8,4); // ВОТ ТУТ ПОМЕНЯЛ HSI на HSE и выставил в stm32f4xx 8 000 000 HZ 8 336 4 4
     // SYSCLK 168 AHB= 1..2 
          
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

