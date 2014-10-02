/**
  @page I2S_Audio_Codec AN4309 I2S_Audio_Codec Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2009 STMicroelectronics *******************
  * @file     I2S_Audio_Codec/readme.txt 
  * @author   MCD Application Team
  * @version  V2.0.0
  * @date     19/06/2013
  * @brief    The AN4309 application note describes how to use the STM32L1xx 
  *           devices I2S feature to play audio files using an external codec.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Description
 
This application note describes how to use the STM32L1xx devices I2S feature 
to play audio files using an external codec.

@par Directory contents 

+ inc 
    - I2S_Audio_Codec/inc/Waveplayer.h             Waveplayer header file
    - I2S_Audio_Codec/inc/storage.h                Storage header file
    - I2S_Audio_Codec/inc/stm32l1xx_it.h           Interrupt handler header file 
    - I2S_Audio_Codec/inc/stm32l1xx_conf.h         Library configuration header file
    - I2S_Audio_Codec/inc/stm32l1xx_clock_config.h Clock configuration Header file
    - I2S_Audio_Codec/inc/platform_config.h        STM32L152D evaluation board configuration Header file
    - I2S_Audio_Codec/inc/menu.h                   Menu Header file
    - I2S_Audio_Codec/inc/mass_storge.h            Mass storage header file   
    - I2S_Audio_Codec/inc/mass_mal.h               Medium Access Layer header file      
    - I2S_Audio_Codec/inc/main.h 	           Main header file		
    - I2S_Audio_Codec/inc/iddmeasurement.h         Idd measurement header file

+ src 
      
    - I2S_Audio_Codec/src/Waveplayer.c             Waveplayer firmware functions file
    - I2S_Audio_Codec/src/system_stm32l1xx.c       System clock configuration firmware functions file
    - I2S_Audio_Codec/src/storage.c                Storage firmware functions file
    - I2S_Audio_Codec/src/stm32l1xx_it.c           Interrupt handler file 
    - I2S_Audio_Codec/src/stm32l1xx_clock_config.c Clock configuration firmware functions file
    - I2S_Audio_Codec/src/platform_config.c        STM32L152D evaluation board configuration file
    - I2S_Audio_Codec/src/menu.c                   Menu file
    - I2S_Audio_Codec/src/mass_storge.c            Mass storage file   
    - I2S_Audio_Codec/src/mass_mal.c               Medium Access Layer firmware functions file      
    - I2S_Audio_Codec/src/main.c 	           Main Program		
    - I2S_Audio_Codec/src/iddmeasurement.c         Idd measurement file
    - I2S_Audio_Codec/src/diskio.c                 

@par Hardware and Software environment 

  - This example runs on STM32L1xx Devices.
  
  - This example has been tested with STMicroelectronics STM32L152D-EVAL 
    evaluation board and can be easily tailored to any other supported device and development board.

  - STM32L152D-EVAL Set-up 
    Nothing special to do. Plug a headphone in the jack output connector.
      
@note
   SD card needs to be inserted. It needs to content the Media directory files and directories
      which have to be set-up at first before running the demonstration

@par How to use it ?  

 - MDK-ARM 
    - Open the STM32L152-EVAL_Audio_Demo.uvproj project
    - In the build toolbar select the project config:
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 - TrueSTUDO:
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project STM32L152D-EVAL_Audio_Demo to configure the project for STM32 High-density devices.
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Run->Debug (F11) 

- RIDE:
    - Open the Project.rprj project
    - In the configuration toolbar(Project->properties) select the project config:
    - STM32L152D-EVAL_Audio_Demo: to configure the project for STM32L Ultra Low Power High-density devices
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

- EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)


 * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
*/
