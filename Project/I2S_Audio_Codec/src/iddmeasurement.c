/**
  ******************************************************************************
  * @file    iddmeasurement.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    24-January-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the IDD measurement circuit for the STM32L152D-EVAL
  *          demonstration:
  *           + STM32L Low Power Modes selection
  *           + STM32L ADC peripheral configuration and computation
  *           + IDD Measurement Circuit Bias measurement
  *           + STM32L System and Demonstration Save and Restore   
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

/** @defgroup IDD_MEASUREMENT
  * @brief    This file includes the IDD measurement driver for the STM32L152D-EVAL
  *           demonstration.
  * @{
  */

/** @defgroup IDD_MEASUREMENT_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup IDD_MEASUREMENT_Private_Defines
  * @{
  */
#define DATA_EEPROM_BIAS_ADDR  0x08080000

#if   defined ( __TASKING__ )
#define __NOP()  __nop()
#endif

/**
  * @}
  */

/** @defgroup IDD_MEASUREMENT_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup IDD_MEASUREMENT_Private_Variables
  * @{
  */

uint8_t Str[20];
__IO uint8_t LowPowerStatus = 0x00;

__IO uint32_t RCC_AHBENR = 0, RCC_APB2ENR = 0, RCC_APB1ENR = 0;


uint32_t GPIOA_MODER = 0, GPIOB_MODER = 0, GPIOC_MODER = 0, GPIOD_MODER = 0,
         GPIOE_MODER = 0xA8000000, GPIOH_MODER = 0x10, GPIOF_MODER = 0, GPIOG_MODER = 0;

uint32_t GPIOA_PUPDR = 0x64000000, GPIOB_PUPDR = 0x00000100, GPIOC_PUPDR = 0, GPIOD_PUPDR = 0,
                       GPIOE_PUPDR = 0, GPIOH_PUPDR = 0, GPIOF_PUPDR = 0, GPIOG_PUPDR = 0;



#define RCC_APB2Periph_CLOCK    RCC_APB2Periph_USART1 | RCC_APB2Periph_ADC1 | RCC_APB2Periph_SPI1 |\
                                RCC_APB2Periph_SYSCFG | RCC_APB2Periph_TIM9 | RCC_APB2Periph_TIM10 |\
                                RCC_APB2Periph_TIM11

#define RCC_APB1Periph_CLOCK    RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 |\
                                RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM7 | RCC_APB1Periph_WWDG |\
                                RCC_APB1Periph_SPI2 | RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |\
                                RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2 | \
                                RCC_APB1Periph_DAC  | RCC_APB1Periph_LCD  | RCC_APB1Periph_COMP |\
                                RCC_APB1Periph_USB


#define RCC_AHBPeriph_CLOCK    RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOD |\
                               RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |\
                               RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG |\
                               RCC_AHBPeriph_CRC | RCC_AHBPeriph_FLITF | RCC_AHBPeriph_DMA1 |\
                               RCC_AHBPeriph_SRAM
/**
  * @}
  */

/** @defgroup IDD_MEASUREMENT_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup IDD_MEASUREMENT_Private_Functions
  * @{
  */

/**
  * @brief  This function configures the system to be clocked by external High-Speed
  *         oscillator (HSE) with the PLL at 32MHz for current consumption 
  *         measurement purpose.
  *         Run Mode at 32MHz (HSE+PLL) from FLASH:
  *         ======================================
  *           - System Running at 32MHz
  *           - Flash 1 wait state  
  *           - Voltage Range 1
  *           - Code running from Internal FLASH
  *           - All peripherals OFF
  *           - VDD from 1.65V to 3.6V
  *           - Current Consumption ~12mA
  * @param  None
  * @retval None
  */
void IDD_Measurement_Run_HCLKTo32(void)
{

}

/**
  * @brief  This function configures the system to be clocked by internal Multi-Speed
  *         oscillator (MSI) at 1MHz for current consumption measurement purpose.
  *         Run Mode at 1MHz (MSI) from FLASH
  *         =================================
  *           - System Running at MSI (~1MHz)
  *           - Flash 0 wait state  
  *           - Voltage Range 2
  *           - Code running from Internal FLASH
  *           - All peripherals OFF
  *           - VDD from 1.65V to 3.6V
  *           - Current Consumption ~0.6mA
  * @param  None
  * @retval None
  */
void IDD_Measurement_Run_HCLKToMSI_1MHz(void)
{
 
}

/**
  * @brief  This function configures the system to enter Low Power Run mode for
  *         current consumption measurement purpose.
  *         The maximum clock when the system is in Low Power Run mode is ~128KHz.
  *         This mode can only be entered when Voltage Range 2 is selected. 
  *         Low Power Run Mode from FLASH
  *         =============================
  *           - System Running at MSI (~32KHz)
  *           - Flash 0 wait state  
  *           - Voltage Range 2
  *           - Code running from Internal FLASH
  *           - All peripherals OFF
  *           - RTC could be ON or OFF  
  *           - VDD from 1.65V to 3.6V
  *           - Current Consumption ~25uA
  * @param  None
  * @retval None
  */
void IDD_Measurement_RunLP(void)
{
 
}

/**
  * @brief  This function configures the system to enter Sleep mode for
  *         current consumption measurement purpose.
  *         Sleep Mode
  *         ==========  
  *            - System Running at HSI (16MHz)
  *            - Flash 1 wait state  
  *            - Voltage Range 2
  *            - Code running from Internal FLASH
  *            - Current Consumption ~1mA 
  *            - Wakeup using EXTI Line (Key Button PA.00)   
  * @param  None
  * @retval None
  */
void IDD_Measurement_Sleep(void)
{
}

/**
  * @brief  This function configures the system to enter Low Power Sleep mode for
  *         current consumption measurement purpose.
  *         The maximum clock when the system is in Low Power Run mode is ~128KHz.
  *         This mode can only be entered when Voltage Range 2 is selected. 
  *         Low Power Sleep Mode
  *         ====================   
  *           - System Running at MSI (~32KHz)
  *           - Flash 0 wait state  
  *           - Voltage Range 2
  *           - Code running from Internal FLASH
  *           - All peripherals OFF
  *           - RTC could be ON or OFF  
  *           - VDD from 1.65V to 3.6V
  *           - Current Consumption ~4.07uA
  *           - Wakeup using EXTI Line (Key Button PA.00)
  * @param  None
  * @retval None
  */
void IDD_Measurement_SleepLP(void)
{
}

/**
  * @brief  This function configures the system to enter Stop mode for current 
  *         consumption measurement purpose.
  *         STOP Mode
  *         =========   
  *           - RTC could be ON or OFF
  *           - Regulator in LP mode
  *           - LSI, HSI and HSE OFF
  *           - No IWDG
  *           - Current Consumption ~0.5uA
  * @param  None
  * @retval None
  */
void IDD_Measurement_Stop(void)
{

}

/**
  * @brief  This function configures the system to enter Standby mode for 
  *         current consumption measurement purpose.
  *         STANDBY Mode
  *         ============
  *           - RTC could be ON or OFF
  *           - IWDG and LSI OFF
  *           - Current Consumption ~0.3uA 
  *           - Automatic Wakeup using RTC clocked by LSE (after 4s)
  * @param  None
  * @retval None
  */
void IDD_Measurement_EnterStandby(void)
{
 
}

/**
  * @brief  Enter the MCU selected low power modes.
  * @param  lpmode: selected MCU low power modes. This parameter can be one of the
  *         following values:
  *              @arg STM32L_RUN: Run mode at 32MHz.
  *              @arg STM32L_RUN_1M: Run mode at 1MHz.
  *              @arg STM32L_RUN_LP: Low power Run mode at 32KHz.  
  *              @arg STM32L_SLEEP: Sleep mode at 16MHz.
  *              @arg STM32L_SLEEP_LP: Low power Sleep mode at 32KHz.
  *              @arg STM32L_STOP: Stop mode with or without RTC.
  *              @arg STM32L_STANDBY: Standby mode with or without RTC.
  * @param  RTCState: RTC peripheral state during low power modes. This parameter
  *         is valid only for STM32L_RUN_LP, STM32L_SLEEP_LP, STM32L_STOP and
  *         STM32L_STANDBY. This parameter can be one of the following values:
  *              @arg RTC_STATE_ON: RTC peripheral is ON during low power modes.
  *              @arg RTC_STATE_OFF: RTC peripheral is OFF during low power modes.
  * @param  CalibrationState: Bias Calibration mode selection state during low 
  *         power modes. 
  *         This parameter can be one of the following values:
  *              @arg BIAS_CALIB_OFF: Bias Calibration mode is selected during 
  *                   low power modes.
  *              @arg BIAS_CALIB_ON: Bias Calibration mode isn't selected during 
  *                   low power modes.
  * @retval None
*/
void IDD_Measurement(uint32_t lpmode, uint8_t RTCState, uint8_t CalibrationState)
{
 
}

/**
  * @brief  ADC configuration for automatic IDD measurement.
  * @param  None
  * @retval None
  */
void IDD_Measurement_ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOA Clock */  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure IDD Measurement pin (ADC Channelxx) as analog input -----------*/
  GPIO_InitStructure.GPIO_Pin = IDD_MEASUREMENT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init(IDD_MEASUREMENT_GPIO_PORT, &GPIO_InitStructure);
  
/* ADC1 configuration --------------------------------------------------------*/
  /* Enable HSI clock for ADC clock */
  RCC_HSICmd(ENABLE);

  /*!< Wait till HSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {}
    
/* Enable ADC clock ----------------------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

/* de-initialize ADC ---------------------------------------------------------*/
  ADC_DeInit(ADC1);

/*  ADC configured as follows:
  - NbrOfChannel = 1 - ADC_Channel_1b
  - Mode = Single ConversionMode(ContinuousConvMode Enabled)
  - Resolution = 12Bits
  - Prescaler = /1
  - Sampling time 192 */

  /* ADC Configuration */
  ADC_BankSelection(ADC1, ADC_Bank_B);
  
  /* ADC Configuration */
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel4 configuration */
  ADC_RegularChannelConfig(ADC1, IDD_MEASUREMENT_ADC_CHANNEL, 1, ADC_SampleTime_192Cycles);

  ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);

  ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  /* Wait until ADC1 ON status */
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
  {
  }
}

/**
  * @brief  Internal VREF measurement using ADC Channel 17.
  * @param  None
  * @retval Internal VREF ADC converted value.
  */ 
uint16_t IDD_Measurement_ADC_Supply(void)
{
  uint16_t adcdata = 0x0, i = 0x00;

  /* ADC1 configuration ------------------------------------------------------*/
  IDD_Measurement_ADC_Config();
 
  ADC_TempSensorVrefintCmd(ENABLE);

  /* ADC1 regular channel 17 for VREF configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_192Cycles);

  /* Start ADC conversion by software */
  ADC_SoftwareStartConv(ADC1);  

  /* Initialize result */
  for(i = 4; i > 0; i--)
  {
    /* Wait until end-of-conversion */
    while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
    {
    }

    /* Read ADC conversion result */
    adcdata += ADC_GetConversionValue(ADC1);
  }
	
  /* Deinitialize ADC */
  ADC_TempSensorVrefintCmd(DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
  
  return (adcdata >> 2);
}

/**
  * @brief  Compute the VDD voltage using the VREF ADC converted value.
  * @param  None   
  * @retval VDD voltage value.
  */
uint32_t IDD_Measurement_VDD(void)
{
  uint16_t adcdata;
  
  /* Read the VREF value on ADC converted value. */
  adcdata = IDD_Measurement_ADC_Supply();	
  
  /* Compute the VDD in mV using the theorical VREF value */
  adcdata = (1224 * 4096) / adcdata;
	
  return adcdata;
}

/**
  * @brief  Read the ADC converted value.
  * @param  None
  * @retval ADC converted value.
  */
uint32_t IDD_Measurement_ADC_ReadValue(void)
{
  uint16_t adcvalue = 0x0, i = 0x00;

  /* ADC1 configuration ------------------------------------------------------*/
  IDD_Measurement_ADC_Config();

  /* Start ADC1 Conversion using Software trigger */
  ADC_SoftwareStartConv(ADC1);
     
  for (i = 4; i > 0; i--)
  {  
    /* Wait until ADC Channel 15 end of conversion */
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    {}
    /* Read ADC conversion result */
    adcvalue += ADC_GetConversionValue(ADC1);
  }
  return  (adcvalue >> 2);
}

/**
  * @brief  Compute the IDD value using the ADC converted value and format its
  *         display.
  * @param  CurrentData: Voltage ADC converted value.
  * @param  mode: Current STM32L selected low power modes and adapt the corresponding
  *         computation formula. This parameter can be one of the following values:
  *             @arg STM32L_MODE_RUN: used for STM32L_RUN, STM32L_RUN_1M and
  *                  STM32L_SLEEP low power modes.        
  *             @arg STM32L_MODE_LP: used for STM32L_RUN_LP, STM32L_SLEEP_LP,
  *                  STM32L_STOP and STM32L_STANDBY low power modes.    
  * @retval None
  */
void IDD_Measurement_ADC_DisplayValue(uint32_t CurrentData, uint32_t mode)
{
  uint32_t vdd, idd;
  
  if((mode != STM32L_MODE_RUN) && (mode != STM32L_MODE_BIAS))
  {
    /* Substract the Bias current value already stored in the DATA EEPROM */  
    if (CurrentData > (*(__IO uint16_t *)DATA_EEPROM_BIAS_ADDR))
    {
      CurrentData -= (*(__IO uint16_t *)DATA_EEPROM_BIAS_ADDR);  
    }
  }   
  /* Calculate voltage value in uV over capacitor C25 for IDD measurement */
  vdd = (uint32_t)((uint32_t)CurrentData * (uint32_t)ADC_CONVERT_RATIO);
  
  /* Calculate the current consumption */
  idd = (uint32_t) (uint32_t)((vdd * EVAL_RESISTOR_RATIO ) / EVAL_MAX9938_GAIN);      

  if(mode != STM32L_MODE_RUN)
  {
    idd *= 1000;
    idd /= (EVAL_RESISTOR_R81 + EVAL_RESISTOR_R62 + EVAL_RESISTOR_STT5PF20V);
  }
  else
  {
    idd /= EVAL_RESISTOR_R62;
  }
  
  IDD_Measurement_Convert(Str, idd, mode);  
}

/**
  * @brief  Convert the IDD current value to a string.
  * @param  ptr: pointer the global string holding text to be displayed.
  * @param  idd: IDD measured value.
  * @param  mode: Current STM32L selected low power modes and adapt the corresponding
  *         computation formula. This parameter can be one of the following values:
  *             @arg STM32L_MODE_RUN: used for STM32L_RUN, STM32L_RUN_1M and
  *                  STM32L_SLEEP low power modes.        
  *             @arg STM32L_MODE_LP: used for STM32L_RUN_LP, STM32L_SLEEP_LP,
  *                  STM32L_STOP and STM32L_STANDBY low power modes.    
  * @retval None
  */
void IDD_Measurement_Convert(uint8_t *ptr, uint32_t idd, uint32_t mode)
{
  uint8_t current1000, current100, current10, current1, current01, current001, digit = 'u';
  
  /* x current value */
  current1000 = (uint8_t)(idd / 100000);
  /* Hundreds current value */
  current100 = (uint8_t)((idd % 100000) / 10000);
  /* Tens current value */
  current10 = (uint8_t)((idd % 10000) / 1000);
  /* 1mA current value */
  current1 = (uint8_t)((idd % 1000) / 100);
  /* 0.1 mA current value */
  current01 = (uint8_t)((idd % 100 ) / 10);
  /* 0.01 mA current value */
  current001 =  (uint8_t)(idd % 10);

  if(mode == STM32L_MODE_RUN)
  {
    digit = 'm';
  }
  /* Format the string */
  sprintf ((char*)ptr, "   IDD: %d%d%d,%d%d%d%cA   ", current1000, current100, current10, current1, current01, current001, digit);   
}

/**
  * @brief  Displays the Standby IDD value after woken-up from Standby mode.
  * @param  None
  * @retval None
  */
void IDD_Measurement_DisplayStandby(void)
{   
  while (Menu_ReadKey() != NOKEY)
  {}
 
  IDD_Measurement_ADC_DisplayValue((*(__IO uint16_t *)DATA_EEPROM_STBY_ADDR), STM32L_MODE_LP);
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
    
  LCD_DisplayStringLine(LCD_LINE_1, "    Standby Mode    ");
  LCD_DisplayStringLine(LCD_LINE_3, Str);
  LCD_DisplayStringLine(LCD_LINE_5, "Press JoyStick to   ");
  LCD_DisplayStringLine(LCD_LINE_6, "continue.           ");
  
  while (Menu_ReadKey() == NOKEY)
  {}

  /* Disable ADC1 */
  ADC_Cmd(ADC1, DISABLE);
}

/**
  * @brief  Selects the RTC State during low power modes. RTC could be ON or OFF.
  * @param  None
  * @retval RTC state during low power modes.
  *            - RTC_STATE_ON: RTC ON during low power modes.
  *            - RTC_STATE_OFF: RTC OFF during low power modes.    
  */
uint32_t IDD_Measurement_SelectRTCState(void)
{
return 0;
}

/**
  * @brief  Save Demonstration context (GPIOs Configurations, peripherals,...).
  * @param  None
  * @retval None
  */
void IDD_Measurement_SaveContext(void)
{
 
}

/**
  * @brief  Restore Demonstration context (GPIOs Configurations, peripherals,...).
  * @param  None
  * @retval None
  */
void IDD_Measurement_RestoreContext(void)
{
 
}

/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void IDD_Measurement_SYSCLKConfig_STOP(void)
{

}

/**
  * @brief  Bias measurement using IDD stop mode measurement program.
  * @param  None
  * @retval None
  */
void IDD_Measurement_Calibrate(void)
{

}

/**
  * @brief  Reset the Bias measurement value stored in Data EEPROM.
  * @param  None
  * @retval None
  */
void IDD_Measurement_Bias_Reset(void)
{

}

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

