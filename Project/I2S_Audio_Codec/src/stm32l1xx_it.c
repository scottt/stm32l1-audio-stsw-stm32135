/**
  ******************************************************************************
  * @file    stm32l1xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    24-January-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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

#include "stm32l152d_eval_audio_codec.h" 

/** @addtogroup STM32L152D_EVAL_Demo
* @{
*/

/** @defgroup IT
* @brief Main Interrupt Service Routines.
*        This file provides template for all exceptions handler and peripherals
*        interrupt service routine.
* @{
*/

/** @defgroup IT_Private_Types
* @{
*/
/**
* @}
*/

/** @defgroup IT_Private_Defines
* @{
*/
#define MESSAGE4   "Press Tamper 1 second to  " 
#define MESSAGE5   "Reset all Backup registers" 
#define MESSAGE6   " START  |  RESET  |  EXIT "
#define MESSAGE7   " PAUSE  |  RESET  |  EXIT " 

#define LCD_HX8347D        0x0047
/**
* @}
*/

/** @defgroup IT_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup IT_Private_Variables
* @{
*/
__IO uint8_t Index = 0, Counter1 = 0;
__IO uint32_t WaveCounter = 0;
static __IO uint8_t AlarmStatus = 0;
static __IO uint8_t LedCounter = 0;
extern uint16_t Buffer2[];
extern uint16_t Buffer12[];
extern __IO uint8_t KeyStatee;
uint32_t dmaindex = 0;
extern FIL F;
extern UINT BytesWritten;
extern UINT BytesRead;
extern __IO uint8_t SMbusAlertOccurred;
extern __IO uint32_t  LCDContrast[];
extern __IO uint8_t ContrastIndex;
extern WAVE_FormatTypeDef WAVE_Format;
extern __IO uint8_t LowPowerStatus;
extern __IO uint8_t WavePlayer2Status;
extern __IO uint32_t WaveDataLength;
extern const uint32_t SlideSpeech[14] ;
extern __IO uint8_t volumelevel;
__IO uint32_t BackupIndex = 0;
__IO uint8_t SubSecFrac = 0;
RTC_TimeTypeDef RTC_StampTimeStruct, RTC_TimeStructure;
extern __IO uint8_t StartEvent;
extern __IO uint8_t  VoiceRecStop;            /* Stop Voice Recorder Flag*/
uint8_t* Tab;                                 /* Pointer on Buffer1 or Buffer2  */
uint8_t TabIndex = 0x00;                      /* Variable to choice between Buffer1 and Buffer2 */
__IO uint16_t RecordSample = 0x00;            /* Recorded data index in Buffer1 or Buffer2 */
__IO uint8_t WriteOnTheSDCardStatus = 0x00;
extern __IO uint8_t selectPlayer;
__IO uint16_t SampleNValue = 0;               /* ADC converted value */
__IO uint8_t DOWNBoutonPressed = 0;           /* Stop wave record enable */

/* Define the backup register */
uint32_t BKPDataReg[10] = {  RTC_BKP_DR10, RTC_BKP_DR11, RTC_BKP_DR12, RTC_BKP_DR13, RTC_BKP_DR14, 
RTC_BKP_DR15,RTC_BKP_DR16, RTC_BKP_DR17, RTC_BKP_DR18, RTC_BKP_DR19
};

__IO uint32_t RTCAlarmCount = 0;
__IO uint32_t SecondNumb = 60;
__IO uint32_t CurrentTimeSec = 0;
__IO uint8_t ProductPresStatus = 0, ProductIndex =0, IndexSlides = 0;
extern __IO uint32_t FwdBwdStep;
extern const uint8_t* SlideAddr[14];
/**
* @}
*/


/** @defgroup IT_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/


/** @defgroup IT_Private_Functions
* @{
*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{
  uint32_t tmp = 318, index = 0;
  
  RCC_MSIRangeConfig(RCC_MSIRange_6);
  
  /* Disable LCD Window mode */
  LCD_WindowModeDisable();
  
  /* If HSE is not detected at program startup or HSE clock failed during program execution */
  if ((RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) || (RCC_GetITStatus(RCC_IT_CSS) != RESET))
  {
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_WHITE);
    /* Set the LCD Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    /* Set the LCD Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    /* Display " No Clock Detected  " message */
    LCD_DisplayStringLine(LCD_LINE_0, "No HSE Clock         ");
    LCD_DisplayStringLine(LCD_LINE_1, "Detected. STANDBY    ");
    LCD_DisplayStringLine(LCD_LINE_2, "mode in few seconds. ");
    
    LCD_ClearLine(LCD_LINE_4);
    LCD_DisplayStringLine(LCD_LINE_5, "If HSE Clock         ");
    LCD_DisplayStringLine(LCD_LINE_6, "recovers before the  ");
    LCD_DisplayStringLine(LCD_LINE_7, "time out, a System   ");
    LCD_DisplayStringLine(LCD_LINE_8, "Reset is generated.  ");
    LCD_ClearLine(LCD_LINE_9);
    /* Clear Clock Security System interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_CSS);
    GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_WHITE);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DrawRect(71, 319, 25, 320);
    LCD_SetBackColor(LCD_COLOR_GREEN);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    
    /* Check if the LCD is HX8347D Controller */
    if(LCD_ReadReg(0x00) == LCD_HX8347D)
    {
      tmp = 2 ;
    }
    /* Wait till HSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
      /* Check if the LCD is HX8347D Controller */
      if(LCD_ReadReg(0x00) == LCD_HX8347D)
      {
        if (index == 0xFFFF)
        {
          LCD_DisplayChar(LCD_LINE_3, tmp, 0x20);
          tmp += 16;
          index = 0;
        }
        index++;
        /* Enters the system in STANDBY mode */
        if (tmp > 304)
        {
          LCD_SetBackColor(LCD_COLOR_BLUE);
          LCD_ClearLine(LCD_LINE_3);
          LCD_ClearLine(LCD_LINE_4);
          LCD_ClearLine(LCD_LINE_5);
          LCD_ClearLine(LCD_LINE_6);
          LCD_DisplayStringLine(LCD_LINE_7, " MCU in STANDBY Mode");
          LCD_DisplayStringLine(LCD_LINE_8, "To exit press Wakeup");
          /* Request to enter STANDBY mode */
          PWR_EnterSTANDBYMode();
        }
      }
      else
      {
        if (index == 0xFFFF)
        {
          LCD_DisplayChar(LCD_LINE_3, tmp, 0x20);
          tmp -= 16;
          index = 0;
        }
        
        index++;
        /* Enters the system in STANDBY mode */
        if (tmp < 16)
        {
          LCD_SetBackColor(LCD_COLOR_BLUE);
          LCD_ClearLine(LCD_LINE_3);
          LCD_ClearLine(LCD_LINE_4);
          LCD_ClearLine(LCD_LINE_5);
          LCD_ClearLine(LCD_LINE_6);
          LCD_DisplayStringLine(LCD_LINE_7, " MCU in STANDBY Mode");
          LCD_DisplayStringLine(LCD_LINE_8, "To exit press Wakeup");
          /* Request to enter STANDBY mode */
          PWR_EnterSTANDBYMode();
        }
      }  
    }
    
    /* Generate a system reset */
    NVIC_SystemReset();
  }
}

/**
* @brief  This function handles PVD Handler.
* @param  None
* @retval None
*/
void PVD_IRQHandler(void)
{
  EXTI_ClearITPendingBit(EXTI_Line16);
  
  if (PWR_GetFlagStatus(PWR_FLAG_PVDO) != RESET)
  {  
    /* VDD < PVD level */
    /* Adapt the System Clock to 16MHz and the Voltage Range to Range 2 */
    SetHCLKTo16();
    SystemCoreClockUpdate();
    TIM_SetAutoreload(TIM6, SystemCoreClock/22050);
    Demo_SysTickConfig();
    Demo_LedShowInit();
    Demo_LedShow(ENABLE);
  }
  else
  { 
    /* VDD > PVD level */
    /* Adapt the System Clock to 32MHz and the Voltage Range to Range 1 */
    SetHCLKTo32();
    SystemCoreClockUpdate();
    TIM_SetAutoreload(TIM6, SystemCoreClock/WAVE_Format.SampleRate);
    Demo_SysTickConfig();
    Demo_LedShowInit();
    Demo_LedShow(ENABLE);
  }  
}

/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles Memory Manage exception.
* @param  None
* @retval None
*/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles Bus Fault exception.
* @param  None
* @retval None
*/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles Usage Fault exception.
* @param  None
* @retval None
*/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles SVCall exception.
* @param  None
* @retval None
*/
void SVC_Handler(void)
{
}

/**
* @brief  This function handles Debug Monitor exception.
* @param  None
* @retval None
*/
void DebugMon_Handler(void)
{
}

/**
* @brief  This function handles PendSVC exception.
* @param  None
* @retval None
*/
void PendSV_Handler(void)
{
}

/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/
void SysTick_Handler(void)
{
  /* Decrement the TimingDelay variable */
  Demo_DecrementTimingDelay();
}


/******************************************************************************/
/*            STM32L1xx Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  LowPowerStatus = 0x01;

  if (EXTI_GetITStatus(IDD_WAKEUP_EXTI_LINE) != RESET)
  {
    /* Clear the EXTI Line 0 */
    EXTI_ClearITPendingBit(IDD_WAKEUP_EXTI_LINE);
  }
}

/**
* @brief  This function handles External lines 9 to 5 interrupt request.
* @param  None
* @retval None
*/
void EXTI9_5_IRQHandler(void)
{

 /************************* Audio Player Demo ****************/
  if ((WavePlayer2Status != 0))
  {
    if ((EXTI_GetITStatus(EXTI_Line8) != RESET))
    {
      EXTI_ClearITPendingBit(EXTI_Line8);
      volumelevel--;  /* Control volume */
      if (volumelevel <= 0) 
      { 
        volumelevel = 0;
      }
      /* Configure the new volume */
      EVAL_AUDIO_VolumeCtl(volumelevel);      
    }
    else if ((EXTI_GetITStatus(EXTI_Line6) != RESET))
    {
      EXTI_ClearITPendingBit(EXTI_Line6);
      if(selectPlayer == 1)
      {
        f_lseek(&F, 512);
      }
      else
      {
        f_lseek(&F, 36);
      } 
      
      LCD_SetTextColor(LCD_COLOR_MAGENTA);
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLACK);
      LCD_ClearLine(LCD_LINE_7);
      LCD_DrawRect(LCD_LINE_7, 310, 16, 300);
      
      /* Set WaveDataLenght to the Speech wave length */
      WaveDataLength = WAVE_Format.DataSize;
      /* reload samples in Buffer2 */ 
      f_read (&F, Buffer12, _MAX_SS, &BytesRead);
      
    }
    else if ((EXTI_GetITStatus(EXTI_Line7) != RESET))
    {
      EXTI_ClearITPendingBit(EXTI_Line7);
      /* Exit the application */
      KeyStatee = RIGHT;
    }
  }
  else
  {
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
      while (STM_EVAL_PBGetState(BUTTON_DOWN) == Bit_RESET)
      {
      }
      /* DOWN function */
      Set_DOWNStatus();
      if (VoiceRecStop == RESET)
      {
        DOWNBoutonPressed = 0x01;
      }
      
      /* Clear the EXTI Line 8 */
      EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if ((EXTI_GetITStatus(SD_DETECT_EXTI_LINE) != RESET) && (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) != RESET)  )
    {
      
      /* Configures system clock after wake-up from STOP: enable HSE, PLL and select PLL
      as system clock source (HSE and PLL are disabled in STOP mode) */
      IDD_Measurement_SYSCLKConfig_STOP();
      
      /* Disable TIM6 */
      TIM_Cmd(TIM6, DISABLE);
      
      LCD_Clear(LCD_COLOR_WHITE); 
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_RED);
      LCD_DisplayStringLine(LCD_LINE_2, "Err: SDCard Removed ");
      LCD_DisplayStringLine(LCD_LINE_3, "Please check SD Card");
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DisplayStringLine(LCD_LINE_5, "Press JoyStick Up to");
      LCD_DisplayStringLine(LCD_LINE_6, "Restart the Demo... ");
      
      /* Wait until no key is pressed */
      while (Menu_ReadKey() != UP)
      {}
      /* Generate System Reset to load the new option byte values */
      NVIC_SystemReset();
    }
  }
  EXTI_ClearITPendingBit(RIGHT_BUTTON_EXTI_LINE);  
  EXTI_ClearITPendingBit(LEFT_BUTTON_EXTI_LINE);
  EXTI_ClearITPendingBit(DOWN_BUTTON_EXTI_LINE);
}

/**
* @brief  This function handles TIM2 overflow and update interrupt request.
* @param  None
* @retval None
*/
void TIM2_IRQHandler(void)
{

}
/**
* @brief  This function handles External lines 15 to 10 interrupt request.
* @param  None
* @retval None
*/
void EXTI15_10_IRQHandler(void)
{
  uint8_t KeyState = NOKEY;

  /* Check On the wavePlayer application */
  if ((WavePlayer2Status != 0))
  {
    /* Check UP button */
    if ((EXTI_GetITStatus(EXTI_Line11) != RESET))
    {   
      /* Clear the EXTI Line 11 */
      EXTI_ClearITPendingBit(EXTI_Line11);
      volumelevel++;
      /* Increase the Vol*/
      if (volumelevel >= 100) 
      { 
        volumelevel = 100;
      }
      
      /* Configure the new volume level */
      EVAL_AUDIO_VolumeCtl(volumelevel);
    }
    /* Pause/Re-start Playing*/
    else if ((EXTI_GetITStatus(EXTI_Line13) != RESET))
    {
      
      EVAL_AUDIO_VolumeCtl(0);
      
      EVAL_AUDIO_DeInit();
      
      WavePlayer_Pause();
      
      GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_RESET);
      Demo_Delay(10);
      LCD_SetBackColor(Black);
      LCD_SetTextColor(Magenta);
      /* Display the Titles */
      LCD_DisplayStringLine(LCD_LINE_0, "    STOP Playing    ");
      KeyState = NOKEY;
      /* Press on Sel button to restart */
      while ((KeyState != SEL) && (KeyState != RIGHT))
      {
        Demo_Delay(10);
        KeyState = Menu_ReadJoyes(); 
      }
      if ((KeyState == RIGHT))
      {
        KeyStatee = RIGHT;
      }
      LCD_DisplayStringLine(LCD_LINE_0, "     Now Playing    ");
      /* Re-Configure the Codec */
      WavePlayer_Init();
      WavePlayer_RePlay();
      /* Clear the EXTI line */
      EXTI_ClearITPendingBit(EXTI_Line13);
      
    }
  }
 
  else
  {
    if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
      /* UP function */
      while (STM_EVAL_PBGetState(BUTTON_UP) == Bit_RESET);
      Set_UPStatus();
      /* Clear the EXTI Line 11 */
      EXTI_ClearITPendingBit(EXTI_Line11);
    }
    
    if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
      /* SEL function */
      while (STM_EVAL_PBGetState(BUTTON_SEL) == Bit_RESET);
      /* SEL function */
      Set_SELStatus();
      /* Clear the EXTI Line 13 */
      EXTI_ClearITPendingBit(EXTI_Line13);
    }
  }
  
}

/**
* @brief  This function handles I2C1 Error interrupt request.
* @param  None
* @retval None
*/
void I2C1_ER_IRQHandler(void)
{
  /* Check on I2C1 SMBALERT flag and clear it */
  if (I2C_GetITStatus(LM75_I2C, I2C_IT_SMBALERT))
  {
    I2C_ClearITPendingBit(LM75_I2C, I2C_IT_SMBALERT);
    SMbusAlertOccurred++;
  }
  
  /* Check on I2C1 Time out flag and clear it */
  if (I2C_GetITStatus(LM75_I2C, I2C_IT_TIMEOUT))
  {
    I2C_ClearITPendingBit(LM75_I2C, I2C_IT_TIMEOUT);
  }
  
  /* Check on I2C1 Arbitration Lost flag and clear it */
  if (I2C_GetITStatus(LM75_I2C, I2C_IT_ARLO))
  {
    I2C_ClearITPendingBit(LM75_I2C, I2C_IT_ARLO);
  }
  
  /* Check on I2C1 PEC error flag and clear it */
  if (I2C_GetITStatus(LM75_I2C, I2C_IT_PECERR))
  {
    I2C_ClearITPendingBit(LM75_I2C, I2C_IT_PECERR);
  }
  
  /* Check on I2C1 Overrun/Underrun error flag and clear it */
  if (I2C_GetITStatus(LM75_I2C, I2C_IT_OVR))
  {
    I2C_ClearITPendingBit(LM75_I2C, I2C_IT_OVR);
  }
  
  /* Check on I2C1 Acknowledge failure error flag and clear it */
  if (I2C_GetITStatus(LM75_I2C, I2C_IT_AF))
  {
    I2C_ClearITPendingBit(LM75_I2C, I2C_IT_AF);
  }
  
  /* Check on I2C1 Bus error flag and clear it */
  if (I2C_GetITStatus(LM75_I2C, I2C_IT_BERR))
  {
    I2C_ClearITPendingBit(LM75_I2C, I2C_IT_BERR);
  }
}



/**
* @brief  This function handles TIM3 global interrupt request.
* @param  None
* @retval None
*/
void TIM3_IRQHandler(void)
{ 
 
}

/**
* @brief  This function handles TIM4 global interrupt request.
* @param  None
* @retval None
*/
void TIM4_IRQHandler(void)
{
  
  if (WriteOnTheSDCardStatus == ENABLE)
  {
    /* Load SD Card with the actual buffer Tab */
    f_write (&F, Tab, _MAX_SS, &BytesWritten);
    /* Increment wave size by 512*/
    WaveCounter += _MAX_SS;
    
    WriteOnTheSDCardStatus = DISABLE;
  }
  if (DOWNBoutonPressed == 0x01)
  {
    /* To exit the wave record demo */
    VoiceRecStop = SET;
    /* Reset the Up bouton variable status */
    DOWNBoutonPressed = 0x00;
  }
  
  /* Clear TIM4 interrupt */
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
  
  /* Start TIM4 */
  TIM_Cmd(TIM4, DISABLE);
}

/**
* @brief  This function handles USB Low Priority interrupts requests.
* @param  None
* @retval None
*/
void USB_LP_IRQHandler(void)
{

}

/**
* @brief  This function handles SDIO global interrupt request.
* @param  None
* @retval None
*/
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}


/**
* @brief  This function handles DMA2 Stream3 or DMA2 Stream6 global interrupts
*         requests.
* @param  None
* @retval None
*/
void SD_SDIO_DMA_IRQHANDLER(void)
{
  /* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
  SD_ProcessDMAIRQ();
}

/**
* @brief  This function handles Tamper&Stamp pin interrupt request.
* @param  None
* @retval None
*/
void TAMPER_STAMP_IRQHandler(void)
{
 
}

/**
* @brief  This function handles RTC Alarm interrupt (A and B) request.
* @param  None
* @retval None
*/
void RTC_Alarm_IRQHandler(void)
{

}

/**
* @brief  Displays the current Time on the LCD.
* @param   Line:  the Line where to display the Current time .
*   This parameter can be one of the following values:
*     @arg Linex: where x can be 0..9
* @param  Color_x : specifies the Background Color.
* @param  table   : the Current time and sub second.
* @retval None
*/
void REC_Time_display(Table_TypeDef3 table )
{   
  uint8_t i = 0;
  
  /* Initialize table */
  LCD_SetTextColor(White);
  
  /* Check if the LCD is HX8347D Controller */
  if(LCD_ReadReg(0x00) == LCD_HX8347D)
  {
    for (i=0;i<8;i++)
    {
      /* Display char on the LCD */
      LCD_DisplayChar(180, (10 + (20 *i)), table.tab[i]);
      
    }
  }
  else
  {
    for (i=0;i<8;i++)
    {
      /* Display char on the LCD */
      LCD_DisplayChar(180, (310 - (20 *i)), table.tab[i]);
      
    }  
  }
}

/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_STM32L1XX_HD.s).                                            */
/******************************************************************************/
/**
* @brief  This function handles PPP interrupt request.
* @param  None
* @retval None
*/
/*void PPP_IRQHandler(void)
{
}*/


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
