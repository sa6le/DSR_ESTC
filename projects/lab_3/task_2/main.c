#include "main.h"
uint16_t time;

#define PWM_FREQUENCY_HZ      50

static int configure_leds(void)
{
    GPIO_InitTypeDef gpio_init;
    GPIO_StructInit(&gpio_init);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

     GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1); 

    gpio_init.GPIO_Pin   = GPIO_Pin_10; 
    gpio_init.GPIO_Mode  = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio_init);
    return 0;
}

static int configure_timers(void)
{
    /* Timer  */
    TIM_TimeBaseInitTypeDef tim_init;

    TIM_TimeBaseStructInit(&tim_init);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    tim_init.TIM_Prescaler         = 168; /* One second resolution */
    tim_init.TIM_CounterMode       = TIM_CounterMode_Up;
    tim_init.TIM_Period            = (uint16_t)(100000 / PWM_FREQUENCY_HZ - 1);
    tim_init.TIM_ClockDivision     = TIM_CKD_DIV1;
    tim_init.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &tim_init);

    /* Second timer to track brigtness change */
    TIM_TimeBaseStructInit(&tim_init);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    tim_init.TIM_Prescaler         = 168;
    tim_init.TIM_Period            = 999;
    tim_init.TIM_ClockDivision     = TIM_CKD_DIV1;
    tim_init.TIM_RepetitionCounter = 0;
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_TimeBaseInit(TIM2, &tim_init);

    return 0;
}

static int configure_interrupts(void)
{
    NVIC_InitTypeDef timer_nvic_init_structure;

    timer_nvic_init_structure.NVIC_IRQChannel           = TIM2_IRQn;
    timer_nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0;
    timer_nvic_init_structure.NVIC_IRQChannelSubPriority        = 1;
    timer_nvic_init_structure.NVIC_IRQChannelCmd           = ENABLE;
    NVIC_Init(&timer_nvic_init_structure);

    return 0;
}

static int configure_pwm(void)
{
    TIM_OCInitTypeDef tim_oc_init;

    TIM_OCStructInit(&tim_oc_init);

    tim_oc_init.TIM_OCMode      = TIM_OCMode_PWM1;
    tim_oc_init.TIM_Pulse       = (uint16_t)(100000 / PWM_FREQUENCY_HZ - 1);
    tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init.TIM_OCPolarity  = TIM_OCPolarity_Low;

    TIM_OC3Init(TIM1, &tim_oc_init);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    return 0;
}

static int start_timers(void)
{

    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	time++;

	uint16_t pwm_value = (cos(time * 6.28 / 2000) + 1) / 2.0f * (100000 / PWM_FREQUENCY_HZ - 1);

	TIM_SetCompare3(TIM1, pwm_value);
    }
}
void SetSysClock168(void)
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

     RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

     while (RCC_GetSYSCLKSource() != 0x08)
     {
     }

  }
}

int main(void)
{
    SetSysClock168();
    configure_timers();
    configure_interrupts();
    configure_pwm();

    configure_leds();
    start_timers();

    for (;;);
}
