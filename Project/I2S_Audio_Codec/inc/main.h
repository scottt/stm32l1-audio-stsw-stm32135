/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    24-January-2012
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l152d_eval.h"
#include "stm32l152d_eval_lcd.h"
#include "stm32l152d_eval_i2c_ee.h"
#include "stm32l152d_eval_sdio_sd.h"
#include "stm32l152d_eval_audio_codec.h"  
#include "stm32l1xx_clock_config.h"
#include "waveplayer.h"
#include "menu.h"
#include "storage.h"
#include "mass_storage.h"
#include "iddmeasurement.h"
#include "ff.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32l1xx_it.h"

#include "usb_type.h"
#include "usb_core.h"
#include "usb_pwr.h"

#include <string.h>


/** @addtogroup STM32L152D_EVAL_Demo
  * @{
  */

/** @addtogroup MAIN
  * @{
  */

/** @defgroup MAIN_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Exported_Constants
  * @{
  */
#define MAX_FILES 12  
/**
  * @}
  */

/** @defgroup MAIN_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Exported_Functions
  * @{
  */
void Demo_Init(void);
void Demo_InterruptConfig(void);
void Demo_SysTickConfig(void);
void Demo_IntExtOnOffConfig(FunctionalState NewState);
void Demo_GPIOConfig(void);
void Demo_COMPConfig(void);
void Demo_Delay(__IO uint32_t nCount);
uint32_t Demo_DelayJoyStick(__IO uint32_t nTime);
void Demo_DecrementTimingDelay(void);
void Delay(__IO uint32_t nCount);
void Demo_LedShowInit(void);
void Demo_LedShow(FunctionalState NewState);
uint32_t Demo_Get_LedShowStatus(void);
void Demo_CheckBitmapFilesStatus(void);
void Set_DOWNStatus(void);
void Set_UPStatus(void);
void Set_SELStatus(void);
void LowPower_Init(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
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
