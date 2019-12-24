#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

#define SWITCH_DELAY (1000000)


static void _dirty_delay()
{
    int i;
    for (i = 0; i < SWITCH_DELAY; i++);
}

void hse(void)
{
  int HSEStartUpStatus;
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig( RCC_HSE_ON);


  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

     RCC_PLLConfig(RCC_PLLSource_HSE,4,336,4,7);

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
void hsi(void)
{
    RCC_DeInit();
    RCC_HSICmd(ENABLE);

    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    RCC_PLLConfig(RCC_PLLSource_HSI, 16,336,16,7);
    // SYSCLK 168 AHB= 1..2 

    RCC_PLLCmd( ENABLE);

     /* Select PLL as system clock source */
    RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
}
//-------------------------------------------------------------------------------------------
int main(void)
{
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
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   
    while (1)
    {
       int state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

      if (!state)
      {
        // RCC_DeInit();
        // RCC_HSICmd(DISABLE);
        hse();
        GPIO_ResetBits(GPIOD,GPIO_Pin_13 );
        GPIO_ResetBits(GPIOD,GPIO_Pin_12 );
       _dirty_delay();
      GPIO_SetBits(GPIOD, GPIO_Pin_12 );
       _dirty_delay();
      }
      else 
      {
      // RCC_DeInit();
      // RCC_HSICmd(ENABLE);
      hsi();
      GPIO_ResetBits(GPIOD,GPIO_Pin_12 );
      GPIO_ResetBits(GPIOD,GPIO_Pin_13);
      _dirty_delay();
      GPIO_SetBits(GPIOD, GPIO_Pin_13);
      _dirty_delay();
      }
    }
}


