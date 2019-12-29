#include <stm32f4xx.h>


void delay(uint32_t value);
void SetSysClockTo168(void);

#define SWITCH_DELAY    ((uint32_t)2000000)

void delay(uint32_t value)
{
   for (uint32_t i=0; i<value;i++)
   {
      __NOP();
   }

}


static int configure_leds(void)
{
   GPIO_InitTypeDef leds_init_structure;
   /* Enable clocking for LEDS */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   leds_init_structure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
   leds_init_structure.GPIO_Mode = GPIO_Mode_OUT;
   leds_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
   leds_init_structure.GPIO_OType = GPIO_OType_PP; 
   GPIO_Init(GPIOA, &leds_init_structure);
   GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);

   return 0;
}


static int configure_timers(void)
{
   /* Timer  */
   TIM_TimeBaseInitTypeDef timer_init_structure;
   /* Initialize peripheral clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   /* Initialize timer */
   timer_init_structure.TIM_Prescaler     = 168;  //because 168MHz
   timer_init_structure.TIM_CounterMode   = TIM_CounterMode_Up;
   timer_init_structure.TIM_Period        = 1000000;   
   timer_init_structure.TIM_ClockDivision = TIM_CKD_DIV1; 
   timer_init_structure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit(TIM2, &timer_init_structure);
   /* Start timer */
   TIM_Cmd(TIM2, ENABLE);

   return 0;
}




int main(void)
{ 

   SetSysClockTo168();

   uint32_t timer_value = 0;
   int8_t   led_counter   = 0;
   int8_t   led_direction = 1;

   configure_leds();
   configure_timers();


   while(1)
   {
      timer_value = TIM_GetCounter(TIM2);
      if (timer_value == 0)
      {
         GPIO_ResetBits(GPIOA, GPIO_Pin_8 << led_counter);
      }
      else if (timer_value == 1000000) //timper
      {
         GPIO_SetBits(GPIOA, GPIO_Pin_8 << led_counter);
         led_counter = (3 + (led_counter - led_direction) % 3) % 3;
      }

      
   }


}

//HSE
void SetSysClockTo168(void)
{
  int HSEStartUpStatus;
  RCC_DeInit();

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


     RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);


     while (RCC_GetSYSCLKSource() != 0x08)
     {
     }

  }
}