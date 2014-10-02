/**
  ******************************************************************************
  * @file    storage.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    24-January-2012
  * @brief   This file contains all the functions prototypes for the storage
  *          firmware driver.
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
#ifndef __STORAGE_H
#define __STORAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/** @addtogroup STM32L152D_EVAL_Demo
  * @{
  */

/** @addtogroup STORAGE
  * @{
  */

/** @defgroup STORAGE_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Exported_Functions
  * @{
  */
uint32_t Storage_Init (void);
uint32_t Storage_OpenReadFile(uint8_t Xpoz, uint16_t Ypoz, const char* BmpName);
uint32_t Storage_GetDirectoryBitmapFiles (const char* DirName, char* Files[]);
uint32_t Storage_GetDirectoryWaveFiles (const char* DirName, char* Files[]);
uint32_t Storage_CheckBitmapFile(const char* BmpName, uint32_t *FileLen);
void Storage_ClearWindow(uint8_t Xpoz, uint16_t Ypoz, uint8_t Height, uint16_t Width);
#ifdef __cplusplus
}
#endif

#endif /* __STORAGE_H */
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
