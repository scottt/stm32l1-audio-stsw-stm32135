/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    24-January-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * FOR MORE INFORMATION PLEASE READ CAREFULLY THE LICENSE AGREEMENT FILE
  * LOCATED IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L152D_EVAL_Demo
  * @{
  */

/** @defgroup MAIN
  * @brief    Main program body
  * @{
  */

/** @defgroup MAIN_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup MAIN_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Variables
  * @{
  */
static __IO uint32_t TimingDelay = 0;
static __IO uint32_t LedShowStatus = 0;
static __IO uint32_t DOWNStatus = 0x00, UPStatus = 0x00, SELStatus = 0x00;
static __IO uint32_t StandbyModeStatus = 0x00;
/**
  * @}
  */

/** @defgroup MAIN_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Functions
  * @{
  */

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

 int main(void)
{
  /* Initialize the Demo */
  Demo_Init();
 /* Write block of 512 bytes on awddress 0 */
  
  /* The DOWNStatus, SELStatus or UPStatus will be set in stm32l1xx_it.c 
    when the joystick button(DOWN, SEL or UP) is pressed */
  while (1)
  {
    if (DOWNStatus != 0x00)
    {
      /* Reset DOWNStatus value */
      DOWNStatus = 0;
      /* External Interrupt Disable */
      Demo_IntExtOnOffConfig(DISABLE);
      /* Execute DOWN Function */
      Menu_DownFunc();
      /* External Interrupt Enable */
      Demo_IntExtOnOffConfig(ENABLE);
    }
    else if (SELStatus != 0x00)
    {
      /* Reset SELStatus value */
      SELStatus = 0;
      /* External Interrupt Disable */
      Demo_IntExtOnOffConfig(DISABLE);
      /* Execute SEL Function */
      Menu_SelFunc();
      /* External Interrupt Enable */
      Demo_IntExtOnOffConfig(ENABLE);
    }
    else if (UPStatus != 0x00)
    {
      /* Reset UPStatus value */
      UPStatus = 0;
      /* External Interrupt Disable */
      Demo_IntExtOnOffConfig(DISABLE);
      /* Execute UP Function */
      Menu_UpFunc();
      /* External Interrupt Enable */
      Demo_IntExtOnOffConfig(ENABLE);
    }
  }
}

/**
  * @brief  Initializes the demonstration application.
  * @param  None
  * @retval None
  */
void Demo_Init(void)
{ 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);
	  
  RTC_OutputConfig(RTC_Output_Disable, RTC_OutputPolarity_High); 
  
  /* Check if the StandBy flag is set */
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {/* System resumed from STANDBY mode */
	
    /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);
    
    StandbyModeStatus = 0x01;
  }
  
  /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB |
                        RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD |
                        RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOG |
                        RCC_AHBPeriph_GPIOH | RCC_AHBPeriph_GPIOF, ENABLE);

  /* SYSCFG and ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG | RCC_APB2Periph_ADC1 , ENABLE);

  /* SPI2 and COMP Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB1Periph_COMP, ENABLE); 

  /*------------------- Resources Initialization -----------------------------*/
  /* Initialize the LCD */
  STM32L152D_LCD_Init();      
    
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);

  if(StandbyModeStatus == 0x01)
  {
    IDD_Measurement_DisplayStandby();
  }
  
  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);


  /* GPIO Configuration */
  Demo_GPIOConfig();

  /* Interrupt Configuration */
  Demo_InterruptConfig();

  /* Configure the systick */
  Demo_SysTickConfig();

  /*------------------- Drivers Initialization -------------------------------*/
  /* Initialize the LEDs toogling */
  Demo_LedShowInit();

  /* Initialize the Low Power application */
//  LowPower_Init();
  
  /* If HSE is not detected at program startup */
  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
    /* Generate NMI exception */
    SCB->ICSR |= SCB_ICSR_NMIPENDSET;
  }

  /* Checks the availability of the bitmap files */
  Demo_CheckBitmapFilesStatus(); 
  
  /* Display the STM32 introduction */
  Menu_STM32Intro();
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);

  /* Enable Leds toggling */
  Demo_LedShow(ENABLE);

  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);

  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  /* Reset the StandbyModeStatus */
  StandbyModeStatus = 0x00;
   
  /* Display the main menu icons */
  Menu_ShowMenuIcons();
}

/**
  * @brief  Configures the used IRQ Channels and sets their priority.
  * @param  None
  * @retval None
  */
void Demo_InterruptConfig(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;


  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);

  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_Init(&NVIC_InitStructure); 
  
  /* Enable the TIM6 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM2 UP Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the TIM2 UP Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  

  /* Enable the TIM3 Interrupt for capture compare event */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM4 Interrupt to trigger the ADC */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
 /* DMA1 channel1 IRQChannel enable */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI15_10 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Re-configure and enable I2C1 error interrupt to have the higher priority */
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
void Demo_SysTickConfig(void)
{
  /* Setup SysTick Timer for 10 ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Enables or disables EXTI for the menu navigation keys :
  *         EXTI lines 11, 13 and 8 which correpond respectively
  *         to "UP", "SEL" and "DOWN".
  * @param  NewState: New state of the navigation keys. This parameter
  *                   can be: ENABLE or DISABLE.
  * @retval None
  */
void Demo_IntExtOnOffConfig(FunctionalState NewState)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);

  /* Disable the EXTI line 11, 13 and 8 on falling edge */
  if (NewState == DISABLE)
  {
    EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line13 | EXTI_Line8 |  EXTI_Line7 | EXTI_Line6;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    /* Enable the EXTI9_5 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  }
  /* Enable the EXTI line 11, 13 and 8 on falling edge */
  else
  {
    /* Clear the the EXTI line 11, 13 and 8  interrupt pending bit */
    EXTI_ClearITPendingBit( EXTI_Line11 | EXTI_Line13 | EXTI_Line8);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line =  EXTI_Line11 | EXTI_Line13 | EXTI_Line8;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    /* Enable the EXTI9_5 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
   /* Enable the EXTI15_10 Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
  }
}

/**
  * @brief  Configures the COMP to adjust the LCD glass contrast with the
  * luminosity range.
  * @param  None
  * @retval None
  */
void Demo_COMPConfig(void)
{
  NVIC_InitTypeDef     NVIC_InitStructure;
  EXTI_InitTypeDef     EXTI_InitStructure;
  COMP_InitTypeDef     COMP_InitStructure;
  
  /* Close the routing interface Input Output switches */
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_GR6_1, ENABLE);
  
  /* COMP2 init */
  /* In this example, the lower threshold is set to 3VREFINT/4 ~ 3 * 1.22 / 4 ~ 0.915 V
     but can be changed to other available possibilities */
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_3_4VREFINT;
  COMP_InitStructure.COMP_OutputSelect = COMP_OutputSelect_None;
  COMP_InitStructure.COMP_Speed = COMP_Speed_Slow;
  COMP_Init(&COMP_InitStructure);

  /* Delay to Comparator startup time 32MHz* 20us = 640 */
  Delay(0xFFFF);

  /* Check if COMP2 output level is high */
  if (COMP_GetOutputLevel(COMP_Selection_COMP2) != COMP_OutputLevel_Low)
  {
    /* COMP2 init */
    /* In this example, the lower threshold is set to VREFINT ~ 1.22 V
    but can be changed to other available possibilities */
    COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_VREFINT;
    COMP_Init(&COMP_InitStructure);

    /* Delay to Comparator startup time 32MHz* 20us = 640 */
    Delay(0xFFFF);
    
    /* Check if COMP2 output level is high */
    if (COMP_GetOutputLevel(COMP_Selection_COMP2) != COMP_OutputLevel_Low)
    {
    }
  }

  /* Configure and enable External interrupt */
  EXTI_InitStructure.EXTI_Line = EXTI_Line22;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure and enable COMP interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = COMP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the different GPIO ports pins.
  * @param  None
  * @retval None
  */
void Demo_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure Joystick pins: PG.06, PG.07, PG.08, PG.11 and PG.13 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Configure IDD_WAKEUP pin: PA.0 as input floating */
  GPIO_InitStructure.GPIO_Pin = IDD_WAKEUP_PIN;
  GPIO_Init(IDD_WAKEUP_GPIO_PORT, &GPIO_InitStructure);

  /* LEFT Button */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource6);
  /* RIGHT Button */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource7);
  /* DOWN Button */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource8);
  /* UP Button */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource11);
  /* SEL Button */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource13);
  /* KEY/WAKEUP Button */
  SYSCFG_EXTILineConfig(KEY_BUTTON_EXTI_PORT_SOURCE, KEY_BUTTON_EXTI_LINE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length (time base 10 ms).
  * @retval None
  */
void Demo_Delay(__IO uint32_t nCount)
{
  TimingDelay = nCount;
  /* Enable the SysTick Counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE;
  while (TimingDelay != 0);
  /* Disable the SysTick Counter */
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
  /* Clear the SysTick Counter */
  SysTick->VAL = (uint32_t)0x0;
}

/**
  * @brief  Inserts a delay time while no joystick RIGHT, LEFT and SEL
  *         are pressed.
  * @param  nTount: specifies the delay time length (time base 10 ms).
  * @retval Pressed Key. This value can be: NOKEY, RIGHT, LEFT or SEL.
  */
uint32_t Demo_DelayJoyStick(__IO uint32_t nTime)
{
  __IO uint32_t keystate = 0;
  TimingDelay = nTime;

  while ((TimingDelay != 0) && (keystate != RIGHT) && (keystate != LEFT) && (keystate != SEL))
  {
    keystate = Menu_ReadKey();
  }

  return keystate;
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void Demo_DecrementTimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

/**
  * @brief  Configure the leds pins as output pushpull: LED1, LED2, LED3
  *         and LED4
  * @param  None
  * @retval None
  */
void Demo_LedShowInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);

  /* Configure LED pins: PD.03, PD.07, PG.14 and PG.15 as output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  /* Time Base configuration 100ms*/
  TIM_TimeBaseStructure.TIM_Prescaler = 1000;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0x0C80;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Channel 1, 2, 3 and 4 Configuration in Timing mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0x0;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
}

/**
  * @brief  Enables or disables LED1, LED2, LED3 and LED4 toggling.
  * @param  None
  * @retval None
  */
void Demo_LedShow(FunctionalState NewState)
{
  /* Enable LEDs toggling */
  if (NewState == ENABLE)
  {
    LedShowStatus = 1;
    /* Enable the TIM Update Interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    /* TIM counter enable */
    TIM_Cmd(TIM2, ENABLE);
  }
  /* Disable LEDs toggling */
  else
  {
    LedShowStatus = 0;
    /* Disable the TIM Update Interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
    /* TIM counter disable */
    TIM_Cmd(TIM2, DISABLE);
  }
}

/**
  * @brief  Get the LedShowStatus value.
  * @param  None
  * @retval LedShowStatus Value.
  */
uint32_t Demo_Get_LedShowStatus(void)
{
  return LedShowStatus;
}

/**
  * @brief  Checks the bitmap files availability and display a warning
  *         message if these files doesn't exit.
  * @param  None
  * @retval None
  */
void Demo_CheckBitmapFilesStatus(void)
{
  /* Checks if the Bitmap files are loaded */
  if (Menu_CheckBitmapFiles() != 0)
  {
    /* Double checking for Demo to make sure micro SD can be correctly read*/
    if (Menu_CheckBitmapFiles() != 0)
    {
       /* Double checking for Demo to make sure micro SD can be correctly read*/
       if (Menu_CheckBitmapFiles() != 0)
        {
          /* Set the LCD Back Color */
          LCD_SetBackColor(LCD_COLOR_BLUE);
          /* Set the LCD Text Color */
          LCD_SetTextColor(LCD_COLOR_WHITE);
          LCD_DisplayStringLine(LCD_LINE_0, "      Warning       ");
          LCD_DisplayStringLine(LCD_LINE_1, "No loaded Bitmap    ");
          LCD_DisplayStringLine(LCD_LINE_2, "files. Demo can't be");
          LCD_DisplayStringLine(LCD_LINE_3, "executed.           ");
          LCD_DisplayStringLine(LCD_LINE_4, "Please be sure that ");
          LCD_DisplayStringLine(LCD_LINE_5, "all files are       ");
          LCD_DisplayStringLine(LCD_LINE_6, "correctly programmed");
          LCD_DisplayStringLine(LCD_LINE_7, "in the MicroSD Card ");
          LCD_DisplayStringLine(LCD_LINE_8, "then restart Demo   ");

         /* Deinitializes the RCC */
          RCC_DeInit();

         /* Demo Can't Start */
         while (1)
        {}
      }  
    }
  }
}
/**
  * @brief  Sets the DOWNStatus variable
  * @param  None
  * @retval None
  */
void Set_DOWNStatus(void)
{
  DOWNStatus = 1;
}

/**
  * @brief Sets the UPStatus variable
  * @param  None
  * @retval None
  */
void Set_UPStatus(void)
{
  UPStatus = 1;
}

/**
  * @brief Sets the SELStatus variable
  * @param  None
  * @retval None
  */
void Set_SELStatus(void)
{
  SELStatus = 1;
}

/**
  * @brief  Initializes Low Power application.
  * @param  None
  * @retval None
  */
void LowPower_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);

  /* Enable Clock Security System(CSS) */
  RCC_ClockSecuritySystemCmd(ENABLE);

  /* EXTI configuration *********************************************************/
  EXTI_ClearITPendingBit(EXTI_Line16);
  EXTI_InitStructure.EXTI_Line = EXTI_Line16; /* PVD output */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* NVIC configuration *********************************************************/
  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure the PVD to Level 1 : 2.1 V */
  PWR_PVDLevelConfig(PWR_PVDLevel_1);

  /* Enable PVD */
  PWR_PVDCmd(ENABLE);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, "   Assert Function  ");
  /* Infinite loop */
  while (1)
  {}
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
