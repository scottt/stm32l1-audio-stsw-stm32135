/**
  ******************************************************************************
  * @file    waveplayer.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    24-January-2012
  * @brief   This file includes the wave player driver for the STM32L152D_EVAL demo.
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

/** @defgroup WAVEPLAYER
* @brief    This file includes the wave player driver for the STM32L152D_EVAL
*           demo.
* @{
*/

/** @defgroup WAVEPLAYER_Private_Types
* @{
*/
typedef enum
{
  LittleEndian,
  BigEndian
}Endianness;


/**
* @}
*/

/** @defgroup WAVEPLAYER_Private_Defines
* @{
*/
#define REC_FREQ 31250
#define LCD_HX8347D        0x0047 /* LCD ID */

/**
* @}
*/


/** @defgroup WAVEPLAYER_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup WAVEPLAYER_Private_Variables
* @{
*/
WAVE_FormatTypeDef WAVE_Format;
static __IO ErrorCode WaveFileStatus = Unvalid_RIFF_ID;
__IO uint32_t WaveDataLength = 0;
extern __IO uint32_t WaveCounter;
static __IO uint32_t SpeechDataOffset = 0x00;
static uint32_t wavelen = 0;
extern FIL F;
extern UINT BytesWritten;
extern UINT BytesRead;

#if defined (__CC_ARM) /*!< ARM Compiler */
__align(4) 
uint8_t Buffer1[_MAX_SS];
#elif defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
uint8_t Buffer1[_MAX_SS];
#elif defined (__GNUC__) /*!< GNU Compiler */
uint8_t Buffer1[_MAX_SS] __attribute__ ((aligned (4)));
#elif defined (__TASKING__) /*!< TASKING Compiler */ 
__align(4)
uint8_t Buffer1[_MAX_SS];
#endif /* __CC_ARM */

uint16_t Buffer2[_MAX_SS];
uint16_t Buffer12[_MAX_SS];
uint8_t  BufferHeader[512];
extern FATFS fs;
extern DIR dir;
__IO uint8_t BufferSwitch = 1;
__IO uint8_t volumelevel = 70, volumelevelp;
extern __IO uint8_t ProductIndex;
__IO uint8_t WavePlayerStatus = 0,WavePlayer2Status = 0;
__IO uint8_t KeyStatee = NOKEY;
__IO uint8_t  VoiceRecStop = 0;                /* Stop Voice Recorder  Flag*/
__IO uint32_t SelectedMaxRecordDataLength = 0x00;
extern __IO uint8_t DOWNBoutonPressed;             /* Stop wave record enable */
extern __IO uint8_t selectPlayer;
extern uint8_t NumberOfFiles;
extern Table_TypeDef2 PlayList[MAX_FILES];;
extern uint8_t WaveSelectIndex ;

extern const uint8_t* SlideAddr[14];
extern const uint8_t* WAVESlideAddr[14] ;



__IO uint32_t FwdBwdStep = 0;
const uint32_t SlideSpeech[14] =
{
  0, 788480, 1714176, 2021888, 2746880, 4255744, 5049856, 6132736,
  6874112, 7742976, 8635904, 9624064, 10711040, 11457536
};
/**
* @}
*/

/** @defgroup WAVEPLAYER_Private_FunctionPrototypes
* @{
*/
static uint32_t ReadUnit(uint8_t *buffer, uint8_t idx, uint8_t NbrOfBytes, Endianness BytesFormat);

/**
* @}
*/

/** @defgroup WAVEPLAYER_Private_Functions
* @{
*/

/**
* @brief  Wave player Initialization
* @param  None
* @retval None
*/
void WavePlayer_Init(void)
{
  /* Set I2S as  audio interface */
  EVAL_AUDIO_SetAudioInterface(AUDIO_INTERFACE_I2S);
  
  
  if(selectPlayer == 1)
  {  
    /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOs...) */ 
    EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, volumelevel , WAVE_Format.SampleRate/4);
  }
  else
  {
    /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOs...) */ 
    EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, volumelevel , WAVE_Format.SampleRate);
  }
}
/**
* @brief  Returns the Wave file status.
* @param  None
* @retval Wave file status.
*/
ErrorCode Get_WaveFileStatus(void)
{
  return (WaveFileStatus);
}

/**
* @brief  Start wave playing
* @param  None
* @retval None
*/
uint8_t WavePlayer_Start(uint8_t slideindex)
{
  
  WaveFileStatus = Unvalid_RIFF_ID;
  
  
  /* Read the Speech wave file status */
  WaveFileStatus = WavePlayer_WaveParsing((const char*) WAVESlideAddr[slideindex], &wavelen);
  Storage_OpenReadFile(239, 319, (const char*)SlideAddr[slideindex]);
  
  /* Set I2S as  audio interface */
  EVAL_AUDIO_SetAudioInterface(AUDIO_INTERFACE_I2S);
  
  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOs...) */ 
  EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO,  85 , WAVE_Format.SampleRate);
  
  if (WaveFileStatus == Valid_WAVE_File)  /* the .WAV file is valid */
  {
    /* Set WaveDataLenght to the Speech wave length */
    WaveDataLength = WAVE_Format.DataSize;
  }
  else
  {
    return NOKEY;
  }
  
  f_lseek(&F, SpeechDataOffset);
  
  
  /* reload samples in Buffer2 */ 
  f_read (&F, Buffer12, _MAX_SS, &BytesRead);
  
  I2S_Cmd(CODEC_I2S, ENABLE);
  
  KeyStatee = 0;
  while ((WaveDataLength > 0) && (KeyStatee != RIGHT) && (KeyStatee != LEFT) && (KeyStatee != DOWN))
  {   
    
    /* reload samples in Buffer1 */ 
    f_read (&F, Buffer2, _MAX_SS, &BytesRead);
    
    if (WaveDataLength) WaveDataLength -= _MAX_SS;
    if (WaveDataLength < _MAX_SS) WaveDataLength = 0;
    
    while((BufferSwitch == 2) && (KeyStatee != RIGHT) && (KeyStatee != LEFT)&& (KeyStatee != DOWN))
    {
    }
    
    /* reload samples in Buffer1 */ 
    f_read (&F, Buffer12, _MAX_SS, &BytesRead);
    
    if (WaveDataLength) WaveDataLength -= _MAX_SS;
    if (WaveDataLength < _MAX_SS) WaveDataLength = 0;
    
    while((BufferSwitch == 1)&& (KeyStatee != RIGHT) && (KeyStatee != LEFT) && (KeyStatee != DOWN))
    {       
    }
  }
  
  /* Disable the I2S interrupt vector */ 
  SPI_I2S_ITConfig( SPI2, SPI_I2S_IT_TXE, DISABLE);
  I2S_Cmd(CODEC_I2S, DISABLE); 
  
  if((KeyStatee == RIGHT) || (KeyStatee == LEFT) || (KeyStatee == DOWN))
  {
    return 1;
  }
  
  return 0;
}

/**
* @brief  Reads Joyesticks from demoboard.
* @param  None
* @retval Return RIGHT, LEFT, SEL, UP, DOWN or NOKEY
*/
uint8_t Menu_ReadJoyes(void)
{
  /* "right" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    return RIGHT;
  }
  /* "left" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    return LEFT;
  }
  /* "up" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_UP))
  {
    return UP;
  }
  /* "down" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    return DOWN;
  }
  /* "sel" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_SEL))
  {
    return SEL;
  }
  
  /* No key is pressed */
  else
  {
    return NOKEY;
  }
}

/**
* @brief  Stop wave playing
* @param  None
* @retval None
*/
void WavePlayer_Stop(void)
{
  EVAL_AUDIO_Stop(CODEC_PDWN_HW);
}

/**
* @brief  Pause wave playing
* @param  None
* @retval None
*/
void WavePlayer_RePlay(void)
{
  
  SPI_I2S_ITConfig( SPI2, SPI_I2S_IT_TXE, ENABLE);
  
  I2S_Cmd(SPI2, ENABLE);
  
}

/**
* @brief  Pause wave playing
* @param  None
* @retval None
*/
void WavePlayer_Pause(void)
{
  
  /* Disable the I2S interrupt vector */ 
  SPI_I2S_ITConfig( SPI2, SPI_I2S_IT_TXE, DISABLE);
  
  I2S_Cmd(SPI2, DISABLE);
}

/**
* @brief  Decrements the played wave data length.
* @param  None
* @retval Current value of  WaveDataLength variable.
*/
void WavePointerUpdate(uint32_t value)
{
  f_lseek(&F, value);
}

/**
* @brief  Decrements the played wave data length.
* @param  None
* @retval Current value of  WaveDataLength variable.
*/
uint32_t Decrement_WaveDataLength(void)
{
  if (WaveDataLength != 0x00)
  {
    WaveDataLength--;
  }
  return (WaveDataLength);
}


/**
* @brief  Decrements the played wave data length.
* @param  None
* @retval Current value of  WaveDataLength variable.
*/
void Set_WaveDataLength(uint32_t value)
{
  WaveDataLength = value;
}

/**
* @brief  Checks the format of the .WAV file and gets information about
*   the audio format. This is done by reading the value of a
*   number of parameters stored in the file header and comparing
*   these to the values expected authenticates the format of a
*   standard .WAV  file (44 bytes will be read). If  it is a valid
*   .WAV file format, it continues reading the header to determine
*   the  audio format such as the sample rate and the sampled data
*   size. If the audio format is supported by this application,
*   it retrieves the audio format in WAVE_Format structure and
*   returns a zero value. Otherwise the function fails and the
*   return value is nonzero.In this case, the return value specifies
*   the cause of  the function fails. The error codes that can be
*   returned by this function are declared in the header file.
* @param  None
* @retval Zero value if the function succeed, otherwise it return
*         a nonzero value which specifies the error code.
*/
ErrorCode WavePlayer_WaveParsing(const char* WavName, uint32_t *FileLen)
{
  uint32_t temp = 0x00;
  uint32_t extraformatbytes = 0;
  
  f_mount(0, &fs);
  
  f_open (&F, WavName, FA_READ);
  
  f_read (&F, Buffer1, _MAX_SS, &BytesRead);
  
  /* Read chunkID, must be 'RIFF'  ----------------------------------------------*/
  temp = ReadUnit((uint8_t*)Buffer1, 0, 4, BigEndian);
  if (temp != CHUNK_ID)
  {
    return(Unvalid_RIFF_ID);
  }
  
  /* Read the file length ----------------------------------------------------*/
  WAVE_Format.RIFFchunksize = ReadUnit((uint8_t*)Buffer1, 4, 4, LittleEndian);
  
  /* Read the file format, must be 'WAVE' ------------------------------------*/
  temp = ReadUnit((uint8_t*)Buffer1, 8, 4, BigEndian);
  if (temp != FILE_FORMAT)
  {
    return(Unvalid_WAVE_Format);
  }
  
  /* Read the format chunk, must be'fmt ' --------------------------------------*/
  temp = ReadUnit((uint8_t*)Buffer1, 12, 4, BigEndian);
  if (temp != FORMAT_ID)
  {
    return(Unvalid_FormatChunk_ID);
  }
  /* Read the length of the 'fmt' data, must be 0x10 -------------------------*/
  temp = ReadUnit((uint8_t*)Buffer1, 16, 4, LittleEndian);
  if (temp != 0x10)
  {
    extraformatbytes = 1;
  }
  /* Read the audio format, must be 0x01 (PCM) -------------------------------*/
  WAVE_Format.FormatTag = ReadUnit((uint8_t*)Buffer1, 20, 2, LittleEndian);
  if (WAVE_Format.FormatTag != WAVE_FORMAT_PCM)
  {
    return(Unsupporetd_FormatTag);
  }
  
  /* Read the number of channels, must be 0x01 (Mono) ------------------------*/
  WAVE_Format.NumChannels = ReadUnit((uint8_t*)Buffer1, 22, 2, LittleEndian);
  
  /* Read the Sample Rate ----------------------------------------------------*/
  WAVE_Format.SampleRate = ReadUnit((uint8_t*)Buffer1, 24, 4, LittleEndian);
  
  /* Read the Byte Rate ------------------------------------------------------*/
  WAVE_Format.ByteRate = ReadUnit((uint8_t*)Buffer1, 28, 4, LittleEndian);
  
  /* Read the block alignment ------------------------------------------------*/
  WAVE_Format.BlockAlign = ReadUnit((uint8_t*)Buffer1, 32, 2, LittleEndian);
  
  /* Read the number of bits per sample --------------------------------------*/
  WAVE_Format.BitsPerSample = ReadUnit((uint8_t*)Buffer1, 34, 2, LittleEndian);
  
  SpeechDataOffset = 36;
  /* If there is Extra format bytes, these bytes will be defined in "Fact Chunk" */
  if (extraformatbytes == 1)
  {
    /* Read th Extra format bytes, must be 0x00 ------------------------------*/
    temp = ReadUnit((uint8_t*)Buffer1, 36, 2, LittleEndian);
    if (temp != 0x00)
    {
      return(Unsupporetd_ExtraFormatBytes);
    }
    /* Read the Fact chunk, must be 'fact' -----------------------------------*/
    temp = ReadUnit((uint8_t*)Buffer1, 38, 4, BigEndian);
    if (temp != FACT_ID)
    {
      return(Unvalid_FactChunk_ID);
    }
    /* Read Fact chunk data Size ---------------------------------------------*/
    temp = ReadUnit((uint8_t*)Buffer1, 42, 4, LittleEndian);
    
    SpeechDataOffset += 10 + temp;
  }
  /* Read the Data chunk, must be 'data' ---------------------------------------*/
  temp = ReadUnit((uint8_t*)Buffer1, SpeechDataOffset, 4, BigEndian);
  SpeechDataOffset += 4;
  if (temp != DATA_ID)
  {
    return(Unvalid_DataChunk_ID);
  }
  
  /* Read the number of sample data ------------------------------------------*/
  WAVE_Format.DataSize = ReadUnit((uint8_t*)Buffer1, SpeechDataOffset, 4, LittleEndian);
  SpeechDataOffset += 4;
  WaveCounter =  SpeechDataOffset;
  return(Valid_WAVE_File);
}

/**
* @brief  Start wave playing
* @param  None
* @retval None
*/
uint8_t WavePlayerMenu_Start(const char* FileName, uint32_t *FileLen)
{
  uint8_t tmp1,tmp2 = 0;
  uint8_t str[25];
  WaveFileStatus = Unvalid_RIFF_ID;
  
  /* Read the Speech wave file status */
  WaveFileStatus = WavePlayer_WaveParsing((const char*)FileName, &wavelen);
  
  /* Iinitialize */
  WavePlayer_Init();
  
  Display_Wave_Information(PlayList, WAVE_Format.SampleRate , WaveSelectIndex);
  
  sprintf ((char*)str, "Volume : %d            ", volumelevel ); 
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str);     
  
  
  if (WaveFileStatus == Valid_WAVE_File)  /* the .WAV file is valid */
  {
    /* Set WaveDataLenght to the Speech wave length */
    WaveDataLength = WAVE_Format.DataSize;
  }
  else
  {
    return NOKEY;
  }
  
  KeyStatee = NOKEY;
  
  if(selectPlayer == 1)
  {
    f_lseek(&F, 512);
  }
  else
  {
    f_lseek(&F, SpeechDataOffset);
  } 
  
  
  /* reload samples in Buffer2 */ 
  f_read (&F, Buffer12, _MAX_SS, &BytesRead);
  
  BufferSwitch = 1;
  
  I2S_Cmd(CODEC_I2S, ENABLE);
  
  LCD_SetTextColor(LCD_COLOR_MAGENTA);
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLACK);
  LCD_DrawRect(LCD_LINE_7, 310, 16, 300);
  
  WavePlayer2Status = 1;
  STM_EVAL_PBInit(BUTTON_UP,BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_DOWN,BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_SEL,BUTTON_MODE_EXTI); 
  STM_EVAL_PBInit(BUTTON_RIGHT,BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_LEFT,BUTTON_MODE_EXTI);
  
  KeyStatee = NOKEY;
  
  volumelevelp = volumelevel;
  sprintf ((char*)str, "Volume : %d         ", volumelevel ); 
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str); 
  
  while ((WaveDataLength != 0) && (KeyStatee != RIGHT))
  {  
    
    if(volumelevelp != volumelevel)
    {
      volumelevelp = volumelevel;
      sprintf ((char*)str, "Volume : %d         ", volumelevel ); 
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *) str);
      volumelevelp = volumelevel;
    }
    WavePlayer2Status = 1;
    
    /* reload samples in Buffer1 */ 
    f_read (&F, Buffer2, _MAX_SS, &BytesRead);
    
    if (WaveDataLength) WaveDataLength -= _MAX_SS;
    if (WaveDataLength < _MAX_SS) WaveDataLength = 0;
    
    while(BufferSwitch == 2)
    {
      
      tmp1 = (uint8_t) ((uint32_t)((WAVE_Format.DataSize - WaveDataLength) * 100) / WAVE_Format.DataSize);
      
      if(tmp1 != tmp2)
      {
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetTextColor(LCD_COLOR_MAGENTA);
        LCD_DrawLine(LCD_LINE_7, 310 - ((tmp1) * 3), 16, LCD_DIR_VERTICAL);
      }  
      
      tmp2 = tmp1;
    }
    
    /* reload samples in Buffer1 */ 
    f_read (&F, Buffer12, _MAX_SS, &BytesRead);
    
    if (WaveDataLength) WaveDataLength -= _MAX_SS;
    if (WaveDataLength < _MAX_SS) WaveDataLength = 0;
    
    while(BufferSwitch == 1)
    {   
      /* Update the Scorll bar */
      tmp1 = (uint8_t) ((uint32_t)((WAVE_Format.DataSize - WaveDataLength) * 100) / WAVE_Format.DataSize);
      if(tmp1 != tmp2)
      {
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetTextColor(LCD_COLOR_MAGENTA);
        LCD_DrawLine(LCD_LINE_7, 310 - ((tmp1) * 3), 16, LCD_DIR_VERTICAL);
      }
      tmp2 = tmp1;
    }
  }
  
  Demo_IntExtOnOffConfig(DISABLE);
  STM_EVAL_PBInit(BUTTON_LEFT,BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_RIGHT,BUTTON_MODE_GPIO);
  
  EVAL_AUDIO_DeInit();
  I2S_Cmd(SPI2,DISABLE);
  /* Display Image */
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile(239, 319, "STFILES/Music.BMP");  
  
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Black);
  LCD_SetTextColor(White);
  /* Display the Titles */
  LCD_DisplayStringLine(LCD_LINE_0, "     Wave Player    ");
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK);
  
  LCD_DrawFullRect(32,182, 165, 200);
  
  LCD_SetBackColor(LCD_COLOR_WHITE);
  
  LCD_DrawFullRect(35,179, 158, 194);
  
  Media_List_Display(PlayList, NumberOfFiles, WaveSelectIndex);
  WavePlayerStatus = 0;
  WavePlayer2Status = 0;
  return RIGHT;
}

/**
* @brief  Reads a number of bytes from the SPI Flash and reorder them in Big
*         or little endian.
* @param  NbrOfBytes: number of bytes to read.
*         This parameter must be a number between 1 and 4.
* @param  ReadAddr: external memory address to read from.
* @param  Endians: specifies the bytes endianness.
*         This parameter can be one of the following values:
*             - LittleEndian
*             - BigEndian
* @retval Bytes read from the SPI Flash.
*/
static uint32_t ReadUnit(uint8_t *buffer, uint8_t idx, uint8_t NbrOfBytes, Endianness BytesFormat)
{
  uint32_t index = 0;
  uint32_t temp = 0;
  
  for (index = 0; index < NbrOfBytes; index++)
  {
    temp |= buffer[idx + index] << (index * 8);
  }
  
  if (BytesFormat == BigEndian)
  {
    temp = __REV(temp);
  }
  return temp;
}

/**
* @brief  Controls the wave player application LCD display messages
* @param  None.
* @retval None.
*/
uint8_t LCD_Update(void)
{
return 0;
}


/**
* @brief  Get next data sample callback
* @param  None
* @retval Next data sample to be sent
*/
uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
  static uint32_t count = 0;
  uint16_t data = 0;
  
  if (BufferSwitch ==2)
  {  
    /* Get the next sample to be sent */
    data = Buffer2[count] ; 
    
  }
  else
  {
    /* Get the next sample to be sent */
    data = Buffer12[count]; 
  }
  
  if ((++count) >= (_MAX_SS/2))
  {
    count = 0;
    if (BufferSwitch ==2)
    {
      BufferSwitch = 1 ;
    }
    else
    {
      BufferSwitch = 2;
    }
  }
  
  
  return (data);
}

/*--------------------------------
Callbacks implementation:
the callbacks prototypes are defined in the stm32l152d_eval_audio_codec.h file
and their implementation should be done in the user coed if they are needed.
Below some examples of callback implementations.
--------------------------------------------------------*/
/**
* @brief  Calculates the remaining file size and new position of the pointer.
* @param  None
* @retval None
*/
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
  /* Calculate the remaining audio data in the file and the new size 
  for the DMA transfer. If the Audio files size is less than the DMA max 
  data transfer size, so there is no calculation to be done, just restart 
  from the beginning of the file ... */
  /* Check if the end of file has been reached */
  
#ifdef AUDIO_MAL_MODE_NORMAL  
  if (BufferSwitch ==2)
  {
    /* Replay from the beginning */
    EVAL_AUDIO_Play((uint16_t*)Buffer2, _MAX_SS);
    BufferSwitch = 1;
  } 
  else
  {
    /* Replay from the beginning */
    EVAL_AUDIO_Play((uint16_t*)Buffer12, _MAX_SS);
    BufferSwitch = 2;
  }
#else /* #ifdef AUDIO_MAL_MODE_CIRCULAR */
  
  /* Display message on the LCD screen */
  LCD_DisplayStringLine(Line8, " All Buffer Reached ");   
  
#endif /* AUDIO_MAL_MODE_CIRCULAR */
}

/**
* @brief  Manages the DMA FIFO error interrupt.
* @param  None
* @retval None
*/
void EVAL_AUDIO_Error_CallBack(void* pData)
{
  /* Display message on the LCD screen */
  LCD_SetBackColor(Red);
  LCD_DisplayStringLine(Line8, "     DMA  ERROR     ");
  
  /* Stop the program with an infinite loop */
  while (1)
  {}
  
  /* could also generate a system reset to recover from the error */
  /* .... */
}

/**
* @brief  Manages the DMA FIFO error interrupt.
* @param  None
* @retval None
*/
void EVAL_ADC_Config(void* pData)
{
  
 
}  

/**
* @brief  Initialize the wave header file
* @param  None
* @retval None
*/
void WaveRec_HeaderInit(void)
{

}

/**
* @brief  Initialize wave recording
* @param  None
* @retval None
*/
void WaveRec_Init(void)
{

}

/**
* @brief  wait until Voice recording time finish or  a stop record action occurs
* @param  None
* @retval None
*/
void WaveRec_Recording(void)
{

}

/**
* @brief  Start wave recording
* @param  None
* @retval None
*/
void WaveRec_Start(void)
{
 
}


/**
* @brief  I2S interrupt management
* @param  None
* @retval None
*/
void EVAL_Audio_I2S_IRQHandler(void)
{
  uint16_t tmp_var =0 ;
  /* Check on the I2S TXE flag */  
  if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE) != RESET)
  { 
    tmp_var = EVAL_AUDIO_GetSampleCallBack();
    /* Send dummy data on I2S to avoid the underrun condition */
    
    if(selectPlayer == 1)
    {  
      /* Send the data 2 times in case of mono wave */
      SPI_I2S_SendData(CODEC_I2S,(uint16_t)(((tmp_var)  << 1) - 0x134) ); 
      SPI_I2S_SendData(CODEC_I2S,(uint16_t)(((tmp_var)  << 1) - 0x134) );  
    }
    else
    {
      /* Send the data only one time in case of stereo wave */
      SPI_I2S_SendData(CODEC_I2S, tmp_var); 
    }
  }
}


/**
* @brief  Display Wave Information On the LCD
* @param   wavetable:  The Wave play list.
* @param  samplingfrequency : Sampling frequency for the selected wave.
* @param  selectedwaveindex   : Selected wave index.
* @retval None
*/
void Display_Wave_Information(Table_TypeDef2 wavetable[], uint32_t samplingfrequency, uint8_t selectedwaveindex)
{   
  uint8_t i = 0;
  uint8_t sampling[6];
  
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font12x12);
  
  LCD_SetBackColor(LCD_COLOR_BLACK);
  /* Initialize table */
  LCD_SetTextColor(White);
  
  /* Check if the LCD is HX8347D Controller */
  if(LCD_ReadReg(0x00) == LCD_HX8347D)
  {
    /* Display wave Name */
    for (i=0;i<12;i++)
    {
      /* Display char on the LCD */
      LCD_DisplayChar(60 , (150 + (12 *i)), wavetable[selectedwaveindex].tab[i]);  
    } 
  }
  else
  { 
    /* Display wave Name */
    for (i=0;i<12;i++)
    {
      /* Display char on the LCD */
      LCD_DisplayChar(60 , (170 - (12 *i)), wavetable[selectedwaveindex].tab[i]);  
    } 
  }
  
  /* Display sampling frequency */
  sampling[0] = (uint8_t) ((samplingfrequency / 100000) + 0x30);
  sampling[1] = (uint8_t) (((samplingfrequency % 100000) /10000) + 0x30);
  sampling[2] = (uint8_t) (((samplingfrequency % 10000) /1000) + 0x30);
  sampling[3] = (uint8_t) (((samplingfrequency % 1000) /100) + 0x30);
  sampling[4] = (uint8_t) (((samplingfrequency % 100) /10) + 0x30);
  sampling[5] = (uint8_t) (((samplingfrequency % 10)) + 0x30);
  
  /* Check if the LCD is HX8347D Controller */
  if(LCD_ReadReg(0x00) == LCD_HX8347D)
  {
    for(i = 0 ; i < 6 ; i++)
    {  
      if(( sampling[0] == 0x30 ) && (i == 0))
      {
        /* Initialize table */
        LCD_SetTextColor(Black);
        LCD_DisplayChar(72 , (150 + (12 *i)), sampling[i]); 
      }
      else
      {
        /* Initialize table */
        LCD_SetTextColor(White);
        LCD_DisplayChar(72 , (150 + (12 *i)), sampling[i]); 
      }  
    }
    
    /* Display char */
    LCD_DisplayChar(72 , 222, 'H'); 
    LCD_DisplayChar(72 , 234, 'z'); 
    
    /* Display 16bits Stereo */
    LCD_DisplayChar(84 , 150, '1'); 
    LCD_DisplayChar(84 , 162, '6'); 
    LCD_DisplayChar(84 , 174, 'b'); 
    LCD_DisplayChar(84 , 186, 'i'); 
    LCD_DisplayChar(84 , 198, 't'); 
    LCD_DisplayChar(84 , 210, 's'); 
    LCD_DisplayChar(84 , 222, ','); 
    LCD_DisplayChar(84 , 234, 'S'); 
    LCD_DisplayChar(84 , 246, 't'); 
    LCD_DisplayChar(84 , 258, 'e'); 
    LCD_DisplayChar(84 , 270, 'r'); 
    LCD_DisplayChar(84 , 282, 'e');
    LCD_DisplayChar(84 , 292, '0'); 
  }
  else
  {
    for(i = 0 ; i < 6 ; i++)
    {  
      if(( sampling[0] == 0x30 ) && (i == 0))
      {
        /* Initialize table */
        LCD_SetTextColor(Black);
        LCD_DisplayChar(72 , (170 - (12 *i)), sampling[i]); 
      }
      else
      {
        /* Initialize table */
        LCD_SetTextColor(White);
        LCD_DisplayChar(72 , (170 - (12 *i)), sampling[i]); 
      }  
    }
    /* Display char */
    LCD_DisplayChar(72 , 98, 'H'); 
    LCD_DisplayChar(72 , 86, 'z'); 
    
    /* Display 16bits Stereo */
    LCD_DisplayChar(84 , 170, '1'); 
    LCD_DisplayChar(84 , 158, '6'); 
    LCD_DisplayChar(84 , 146, 'b'); 
    LCD_DisplayChar(84 , 134, 'i'); 
    LCD_DisplayChar(84 , 122, 't'); 
    LCD_DisplayChar(84 , 110, 's'); 
    LCD_DisplayChar(84 , 98, ','); 
    LCD_DisplayChar(84 , 86, 'S'); 
    LCD_DisplayChar(84 , 74, 't'); 
    LCD_DisplayChar(84 , 62, 'e'); 
    LCD_DisplayChar(84 , 50, 'r'); 
    LCD_DisplayChar(84 , 38, 'e');
    LCD_DisplayChar(84 , 26, '0'); 
  }
    
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font16x24);
}


/**
* @brief  Displays the Media Play list.
* @param   WaveTable:  table contains the Media files list .
* @param  NumbWaves : specifies the number of Media files .
* @param  SelectedWaveIndex : the index of the selected file.
* @retval None
*/
void Media_List_Display(Table_TypeDef2 WaveTable[], uint8_t NumbWaves, uint8_t SelectedWaveIndex)
{   
  uint8_t i = 0, j = 0;
  
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font12x12);
  for(j=0; j<NumbWaves; j++)
  {
    if (SelectedWaveIndex == j)
    {
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Initialize table */
      LCD_SetTextColor(White);
    }
    else
    {
      LCD_SetBackColor(LCD_COLOR_WHITE);
      /* Initialize table */
      LCD_SetTextColor(Black);
    }
    /* Check if the LCD is HX8347D Controller */
    if(LCD_ReadReg(0x00) == LCD_HX8347D)
    {
      for (i=0;i<12;i++)
      {
        /* Display char on the LCD */
        LCD_DisplayChar(40 + (13 *j), (150 + (12 *i)), WaveTable[j].tab[i]);
        
      }
    }
    else
    {
      for (i=0;i<12;i++)
      {
        /* Display char on the LCD */
        LCD_DisplayChar(40 + (13 *j), (170 - (12 *i)), WaveTable[j].tab[i]);
        
      }
    }
  }
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Initialize table */
  LCD_SetTextColor(Black);
  
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font16x24);
}
#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
uint32_t Codec_TIMEOUT_UserCallback(void)
{
  /* Display message on the LCD screen */
  LCD_DisplayStringLine(Line8, "  CODEC I2C  ERROR  ");  
  
  /* Block communication and all processes */
  while (1)
  {   
  }
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

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
