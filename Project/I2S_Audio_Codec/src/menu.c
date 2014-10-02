/**
  ******************************************************************************
  * @file    menu.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    24-January-2012
  * @brief   This file includes the menu navigation driver for the STM32L152D_EVAL demonstration.
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

/** @defgroup MENU
* @brief    This file includes the menu navigation driver for the STM32L152D_EVAL
*           demonstration.
* @{
*/

/** @defgroup MENU_Private_Types
* @{
*/
typedef void (* tMenuFunc)(void);
typedef struct sMenuItem * tMenuItem;
typedef struct sMenu * tMenu;
/**
* @}
*/


/** @defgroup MENU_Private_Defines
* @{
*/
#define NUMBER_OF_ICONS         12
#define NUMBER_OF_SLIDES        14
#define TEMPERATURE_THYS        31
#define TEMPERATURE_TOS         32

#define MESSAGE1   "   Voice Recording  "
#define MESSAGE2   "    SEL      |     DOWN    " 
#define MESSAGE3   "  START REC  |     EXIT    "


#define MESSAGE8   " REC_WAV      " 
#define MESSAGE9   " FS = 31250 Hz" 

#define LCD_HX8347D        0x0047
/**
* @}
*/

/** @defgroup MENU_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup MENU_Private_Variables
* @{
*/
extern uint8_t  BufferHeader[];
uint8_t MenuItemIndex = 0, nMenuLevel = 0;
uint8_t ItemNumb[MAX_MENU_LEVELS];
uint8_t NumberOfFiles = 0;
uint32_t bmplen = 0;
extern uint8_t Buffer1[];
extern UINT BytesWritten;
extern __IO uint8_t KeyStatee;
extern UINT BytesRead;
char* DirectoryFiles[MAX_FILES];
tMenuItem psMenuItem, psCurrentMenuItem;
tMenu psPrevMenu[MAX_MENU_LEVELS];
tMenu psCurrentMenu;
extern FIL F;
__IO uint8_t selectPlayer = 0;
extern FATFS fs;

uint8_t TempFahrenheitDisplay[]  = "       +abc.d F     ";
int32_t TempValue = 0, TempValueCelsius = 0, TempValueFahrenheit = 0;
__IO uint8_t SMbusAlertOccurred, JumperConfIndex = 0;
extern __IO uint8_t volumelevel;
Table_TypeDef2 PlayList[MAX_FILES];
uint8_t WaveSelectIndex = 0;
int8_t SelectImageIndex = 0;
extern __IO uint8_t ProductPresStatus, IndexSlides;
__IO uint8_t sEEStatus = sEE_FAIL;
__IO uint8_t NumDataRead = 0;
__IO uint16_t CharPosition = 0;
struct sMenuItem
{
  uint8_t* pszTitle;
  tMenuFunc pfMenuFunc;
  tMenuFunc pfUpDownMenuFunc;
  tMenu psSubMenu;
};
struct sMenu
{
  uint8_t* pszTitle;
  tMenuItem psItems;
  uint8_t nItems;
};
const uint8_t* Images1Addr[3] =
{"STFILES/STLOGO.BMP", "STFILES/HELP.BMP", "STFILES/STM32L.BMP"
};
const uint8_t* Images2Addr[2] =
{"STFILES/ALARM.BMP", "STFILES/WATCH.BMP"
};
const uint8_t* IconsAddr[NUMBER_OF_ICONS] =
{
  "STFILES/ICON1.BMP",
  "STFILES/ICON2.BMP",
  "STFILES/ICON3.BMP",
  "STFILES/ICON4.BMP",
  "STFILES/ICON5.BMP",
  "STFILES/ICON6.BMP",
  "STFILES/ICON7.BMP",
  "STFILES/ICON8.BMP",
  "STFILES/ICON9.BMP",
  "STFILES/ICON10.BMP",
  "STFILES/ICON11.BMP",
  "STFILES/ICON12.BMP",
};
const uint8_t* SlideAddr[14] =
{
  "STFILES/SLIDE1.BMP",
  "STFILES/SLIDE2.BMP",
  "STFILES/SLIDE3.BMP",
  "STFILES/SLIDE4.BMP",
  "STFILES/SLIDE5.BMP",
  "STFILES/SLIDE6.BMP",
  "STFILES/SLIDE7.BMP",
  "STFILES/SLIDE8.BMP",
  "STFILES/SLIDE9.BMP",
  "STFILES/SLIDE10.BMP",
  "STFILES/SLIDE11.BMP",
  "STFILES/SLIDE12.BMP",
  "STFILES/SLIDE13.BMP",
  "STFILES/SLIDE14.BMP"
};

const uint8_t* WAVESlideAddr[14] =
{
  "STFILES/WAVE_S1.WAV",
  "STFILES/WAVE_S2.WAV",
  "STFILES/WAVE_S3.WAV",
  "STFILES/WAVE_S4.WAV",
  "STFILES/WAVE_S5.WAV",
  "STFILES/WAVE_S6.WAV",
  "STFILES/WAVE_S7.WAV",
  "STFILES/WAVE_S8.WAV",
  "STFILES/WAVE_S9.WAV",
  "STFILES/WAVE_S10.WAV",
  "STFILES/WAVE_S11.WAV",
  "STFILES/WAVE_S12.WAV",
  "STFILES/WAVE_S13.WAV",
  "STFILES/WAVE_S14.WAV"
    
};

const uint32_t SlidesSpeech[14] =
{
  0, 2166428, 2843436, 4942163, 9131155, 11229882, 14310272, 
  17356812, 18736217, 21952009, 24084586, 27046500, 29077526, 3057540
};

const uint32_t DelayTiming[14] =
{
  2650, 3020, 800, 2480, 4950, 2480, 3640, 3600, 1630, 3800, 2520, 3500, 2400, 1770
};

const uint8_t SlidesCheck[6] =
{
  0x42, 0x4D, 0x42, 0x58, 0x02, 0x00
};
const uint8_t Icons64Check[6] =
{
  0x42, 0x4D, 0x42, 0x20, 0x00, 0x00
};
const uint8_t Icons128Check[6] =
{
  0x42, 0x4D, 0x42, 0x80, 0x00, 0x00
};

#define SCROLL_SPEED  2500 /* Low value gives higher speed */
#define SCROLL_NUM    1
uint8_t LCD_String[8] = " STM32L ";

/*------------------------------ Menu level 4 -------------------------------*/
/*------------------------------ Menu level 3 -------------------------------*/

//struct sMenuItem SettingTimeMenuItems[] =
//{
//  {"       Adjust       ", Calendar_TimeAdjust, Menu_IdleFunc},
//  {"        Show        ", Calendar_TimeShow, Menu_IdleFunc},
//  {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
//};

//struct sMenu TimeMenu =
//{"        Time        ", SettingTimeMenuItems, countof(SettingTimeMenuItems)
//};

struct sMenuItem AudioMenuItems[] =
{
  {"      WavePlayer    ", Menu_WavePlayerFunc, Menu_IdleFunc},
  {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
};

struct sMenu AudioMenu =
{"    Wave Player     ", AudioMenuItems, countof(AudioMenuItems)  
};

/*------------------------------ Menu level 1 -------------------------------*/
struct sMenuItem MainMenuItems[] =
{
  {"     Wave Player    ", Menu_IdleFunc, Menu_IdleFunc, &AudioMenu},
};

struct sMenu MainMenu =
{"     Main menu      ", MainMenuItems, countof(MainMenuItems)
};

/**
* @}
*/

/** @defgroup MENU_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/

/** @defgroup MENU_Private_Functions
* @{
*/

/**
* @brief  Initializes the navigation menu.
* @param  None
* @retval None
*/
void Menu_Init(void)
{
  psCurrentMenu = &MainMenu;
  psPrevMenu[nMenuLevel] = psCurrentMenu;
  psMenuItem = MainMenuItems;
}


/**
* @brief  Displays the current menu.
* @param  None
* @retval None
*/
void Menu_DisplayMenu(void)
{
  uint32_t Line = 0, index = 0;
  
  tMenuItem psMenuItem2;
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Clear the LCD Screen */
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(Line, psCurrentMenu->pszTitle);
  Line += 24;
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  while (!(index >= (psCurrentMenu->nItems)))
  {
    psMenuItem2 = &(psCurrentMenu->psItems[index]);
    LCD_DisplayStringLine(Line, psMenuItem2->pszTitle);
    index++;
    Line += 24;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
}

/**
* @brief  This function is executed when "SEL" push-buttton is pressed.
* @param  None
* @retval None
*/
void Menu_SelFunc(void)
{
  psCurrentMenuItem = psMenuItem;
  if (psMenuItem->psSubMenu != '\0')
  {
    /* Update the current Item by the submenu */
    MenuItemIndex = 0;
    psCurrentMenu = psMenuItem->psSubMenu;
    psMenuItem = &(psCurrentMenu->psItems)[MenuItemIndex];
    Menu_DisplayMenu();
    nMenuLevel++;
    psPrevMenu[nMenuLevel] = psCurrentMenu;
  }
  psCurrentMenuItem->pfMenuFunc();
}


/**
* @brief  This function is executed when any of "UP" push-butttons is pressed.
* @param  None
* @retval None
*/
void Menu_UpFunc(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  if (MenuItemIndex > 0)
  {
    MenuItemIndex--;
  }
  else
  {
    MenuItemIndex = psCurrentMenu->nItems - 1;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/**
* @brief  This function is executed when any of "Down" push-butttons is pressed.
* @param  None
* @retval None
*/
void Menu_DownFunc(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  
  /* Test on the MenuItemIndex value before incrementing it */
  if (MenuItemIndex >= ((psCurrentMenu->nItems) - 1))
  {
    MenuItemIndex = 0;
  }
  else
  {
    MenuItemIndex++;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/**
* @brief  This function is executed when the "RETURN" menu is selected.
* @param  None
* @retval None
*/
void Menu_ReturnFunc(void)
{
  psMenuItem->pfUpDownMenuFunc();
  if (nMenuLevel == 0)
  {
    nMenuLevel++;
  }
  psCurrentMenu = psPrevMenu[nMenuLevel-1];
  psMenuItem = &psCurrentMenu->psItems[0];
  ItemNumb[nMenuLevel] = 0;
  MenuItemIndex = 0;
  nMenuLevel--;
  if (nMenuLevel != 0)
  {
    Menu_DisplayMenu();
  }
  else
  {
    Menu_ShowMenuIcons();
  }
}


/**
* @brief  Reads key from demoboard.
* @param  None
* @retval Return RIGHT, LEFT, SEL, UP, DOWN or NOKEY
*/
uint8_t Menu_ReadKey(void)
{
  /* "right" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == Bit_RESET);
    return RIGHT;
  }
  /* "left" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == Bit_RESET);
    return LEFT;
  }
  /* "up" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_UP))
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == Bit_RESET);
    return UP;
  }
  /* "down" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == Bit_RESET);
    return DOWN;
  }
  /* "sel" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while (STM_EVAL_PBGetState(BUTTON_SEL) == Bit_RESET);
    return SEL;
  }
  /* No key is pressed */
  else
  {
    return NOKEY;
  }
}

/**
* @brief  Idle function.
* @param  None
* @retval None
*/
void Menu_IdleFunc(void)
{
  /* Nothing to execute: return */
  return;
}


/**
* @brief  Show the main menu icon.
* @param  None
* @retval None
*/
void Menu_ShowMenuIcons(void)
{
 

  
  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);
  
  while (Menu_ReadKey() != NOKEY)
  {}
  
  /* Initializes the Menu state machine */
  Menu_Init();
  
  MenuItemIndex = 0;
  
  /* Clear*/
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  
  /* Displays Icons */
//  Menu_DisplayIcons();
  
  /* Disable LCD Window mode */
  LCD_WindowModeDisable();
  
      /* Get the current menu */
			MenuItemIndex = 0;
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    
      Menu_SelFunc();
      Demo_IntExtOnOffConfig(ENABLE);

}

/**
* @brief  Display the STM32 introduction.
* @param  None
* @retval None
*/
void Menu_STM32Intro(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  //LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile(239, 319, "STFILES/STLOGO.BMP");
  
 
  Demo_Delay(100);

  //D_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile(239, 319, "STFILES/STM32L.BMP");
  
  Demo_Delay(100);
  
}




/**
* @brief  Play wave files stored on the SDcard.
* @param  None
* @retval None
*/
void Menu_WavePlayerFunc(void)
{
  EXTI_InitTypeDef     EXTI_InitStructure;
  uint8_t str[20], pressedkey = 0;
  int8_t index = 0;
  
  Demo_IntExtOnOffConfig(DISABLE);
  /* Disable TIM2 to stop LED's toggling before start recording the wave */
  TIM_Cmd(TIM2, DISABLE);
  
  
  LCD_Clear(LCD_COLOR_WHITE);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  LCD_ClearLine(LCD_LINE_0);
  LCD_DisplayStringLine(LCD_LINE_0, "Put the Headphone in");
  LCD_DisplayStringLine(LCD_LINE_1, "CN13 connector and  ");
  LCD_DisplayStringLine(LCD_LINE_2, "press SEL to start  ");
  LCD_SetBackColor(LCD_COLOR_WHITE);
  LCD_ClearLine(LCD_LINE_3);
  LCD_ClearLine(LCD_LINE_4);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_DisplayStringLine(LCD_LINE_4, "Use the UP and DOWN ");
  LCD_DisplayStringLine(LCD_LINE_5, "to select the audio ");
  LCD_DisplayStringLine(LCD_LINE_6, "wave and SEL to play");
//  LCD_SetBackColor(LCD_COLOR_WHITE);
//  LCD_ClearLine(LCD_LINE_7);
   LCD_SetBackColor(LCD_COLOR_BLUE);
	 LCD_ClearLine(LCD_LINE_7);
	 LCD_ClearLine(LCD_LINE_8);
	 LCD_ClearLine(LCD_LINE_9);
//	 LCD_SetBackColor(LCD_COLOR_WHITE);
	 //  LCD_DisplayStringLine(LCD_LINE_8, "USE RIGHT to exit    ");
//  LCD_ClearLine(LCD_LINE_9);
  while (Menu_ReadKey() != SEL)
  {}
  
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Display Image */
  f_mount(0, &fs);
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile(239, 319, "STFILES/Music.BMP");  
  
  
  while (Menu_ReadKey() != NOKEY)
  {}
  
  for (index = 0; index < MAX_FILES; index++)
  {
    DirectoryFiles[index] = malloc(13); /** Initialize the DirectoryFiles pointers (heap) */
  }
  
  NumberOfFiles = Storage_GetDirectoryWaveFiles ("/USER", DirectoryFiles); /** Get the WAV file names on root directory  Maximum 10 files */
  
  
  if (NumberOfFiles == 0)
  {
    for (index = 0; index < MAX_FILES; index++)
    {
      free(DirectoryFiles[index]);
    }
    LCD_Clear(LCD_COLOR_WHITE);
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLACK);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_0, "No wave files...    ");
    LCD_DisplayStringLine(LCD_LINE_1, "Exit:  Push JoyStick");
    while (Menu_ReadKey() == NOKEY)
    {}
    LCD_Clear(LCD_COLOR_WHITE);
    Menu_DisplayMenu();
    
    Demo_IntExtOnOffConfig(ENABLE);
    return;
  }
  
  /*****************************/
  /* Set the LCD Back Color */
  LCD_SetBackColor(Black);
  LCD_SetTextColor(White);
  /* Display the Titles */
  LCD_DisplayStringLine(LCD_LINE_0, "     Wave Player    ");
  index = 0;
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK);
  
  LCD_DrawFullRect(32,182, 165, 200);
  
  LCD_SetBackColor(LCD_COLOR_WHITE);
  
  LCD_DrawFullRect(35,179, 158, 194);
  
  /* Load all files name in PlayList table */
  for(index = 0 ; index < NumberOfFiles ;index++)
  {
    sprintf ((char*)str, "%-13.13s", DirectoryFiles[index]); /** Format the string */
    strcpy ((char *)&PlayList[index].tab[0], (char *)str);
  }
  
  index = 0;
  Media_List_Display(PlayList, NumberOfFiles, 0);
  
  /*******************/
  pressedkey = NOKEY;
  
  while (1)
  {
    pressedkey = Menu_ReadKey();
    if (pressedkey == UP)
    {
      if (index == 0)
      {
        index = NumberOfFiles;
      }
      index--;
      /* Display the playlist */
      Media_List_Display(PlayList, NumberOfFiles, index);
    }
    else if (pressedkey == DOWN)
    {
      if (index == (NumberOfFiles - 1))
      {
        index = -1;
      }
      index++;
      /* Display the playlist */
      Media_List_Display(PlayList, NumberOfFiles, index);
      
    }
    else if (pressedkey == RIGHT)
    {
      break;
    }
    else if (pressedkey == SEL)
    {
      /* Configure and enable External interrupt */
      EXTI_InitStructure.EXTI_Line = EXTI_Line22;
      EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
      EXTI_InitStructure.EXTI_LineCmd = DISABLE;
      EXTI_Init(&EXTI_InitStructure);
      
      LCD_Clear(LCD_COLOR_WHITE);
      
      
      f_mount(0, &fs);
      /* Display Image */
      LCD_SetDisplayWindow(239, 0x13F, 240, 320);
      Storage_OpenReadFile(239, 319, "STFILES/Play.BMP");  
      LCD_SetFont(&Font16x24);
      /* Set the LCD Back Color */
      LCD_SetBackColor(Black);
      LCD_SetTextColor(Magenta);
      /* Display the Titles */
      LCD_DisplayStringLine(LCD_LINE_0, "     Now Playing    ");
      
      /* Displays MESSAGE1 on line 5 */
      LCD_SetFont(&Font12x12);
      /* Set the LCD Back Color */
      LCD_SetBackColor(Blue);
      /* Set the LCD Text Color */
      LCD_SetTextColor(Black); 
      LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE1P);
      /* Set the LCD Back Color */
      LCD_SetBackColor(Cyan);
      LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE2P);
      LCD_SetFont(&Font16x24);
  
      sprintf ((char*)str, "/USER/%-13.13s", DirectoryFiles[index]); /** Format the string */
      WaveSelectIndex = index; 
      
      /* Start playing */
      pressedkey = WavePlayerMenu_Start((const char*)str, &bmplen);
      
      if (pressedkey == NOKEY)
      {
        if (Get_WaveFileStatus() != Valid_WAVE_File)
        {
          /* Set the Back Color */
          LCD_SetBackColor(LCD_COLOR_WHITE);
          
          LCD_ClearLine(LCD_LINE_2);
          /* Set the Back Color */
          LCD_SetBackColor(LCD_COLOR_BLUE);
          /* Set the Text LCD_COLOR_WHITE */
          LCD_SetTextColor(LCD_COLOR_WHITE);
          LCD_ClearLine(LCD_LINE_3);
          LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *) str);
          LCD_DisplayStringLine(LCD_LINE_4, "Wave file is not    ");
          LCD_DisplayStringLine(LCD_LINE_5, "supported.          ");
          LCD_DisplayStringLine(LCD_LINE_6, "Press JoyStick to   ");
          LCD_DisplayStringLine(LCD_LINE_7, "continue.           ");
          /* Set the Back Color */
          LCD_SetBackColor(LCD_COLOR_WHITE);
          LCD_ClearLine(LCD_LINE_8);
          /* Set the Back Color */
          LCD_SetBackColor(LCD_COLOR_BLUE);
          
          while (Menu_ReadKey() != NOKEY)
          {}
          pressedkey = Menu_ReadKey();
          while (pressedkey == NOKEY)
          {
            pressedkey = Menu_ReadKey();
          }
          if (pressedkey == UP)
          {
            if (index == 0)
            {
              index = NumberOfFiles;
            }
            index--;
            LCD_SetBackColor(LCD_COLOR_GREEN);
            LCD_DisplayStringLine(LCD_LINE_9, "                    ");
            sprintf ((char*)str, "/USER/%-13.13s", DirectoryFiles[index]); /** Format the string */
            LCD_DisplayStringLine(LCD_LINE_9, (uint8_t *) str);
          }
          else if (pressedkey == DOWN)
          {
            if (index == (NumberOfFiles - 1))
            {
              index = -1;
            }
            index++;
            LCD_SetBackColor(LCD_COLOR_GREEN);
            LCD_DisplayStringLine(LCD_LINE_9, "                    ");
            sprintf ((char*)str, "/USER/%-13.13s", DirectoryFiles[index]); /** Format the string */
            LCD_DisplayStringLine(LCD_LINE_9, (uint8_t *) str);
          }
          else if (pressedkey == RIGHT)
          {
            index = NumberOfFiles;
          }
          
          
          /* Display Image */
          LCD_SetDisplayWindow(239, 0x13F, 240, 320);
          Storage_OpenReadFile(239, 319, "STFILES/Music.BMP");  
          
          
          /* Set the LCD Back Color */
          LCD_SetBackColor(Black);
          LCD_SetTextColor(White);
          /* Display the Titles */
          LCD_DisplayStringLine(LCD_LINE_0, "     Now Playing    ");
          LCD_SetBackColor(LCD_COLOR_BLUE);
          LCD_SetTextColor(LCD_COLOR_BLACK);
          
          LCD_DrawFullRect(32,182, 165, 200);
          
          LCD_SetBackColor(LCD_COLOR_WHITE);
          
          LCD_DrawFullRect(35,179, 158, 194);
          
          Media_List_Display(PlayList, NumberOfFiles, index);        
          
        }
        else
        {
          index = 1;
        }
      }
      pressedkey = NOKEY;
    } /* SEL Button */
  }/* END WHILE */
  for (index = 0; index < MAX_FILES; index++)
  {
    free(DirectoryFiles[index]);
  }
  
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  
  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  /* Disable TIM2 to stop LED's toggling before start recording the wave */
  TIM_Cmd(TIM2, ENABLE);
  Demo_IntExtOnOffConfig(ENABLE);
}


/**
* @brief  Play Recording files stored on the SDcard.
* @param  None
* @retval None
*/
void Menu_RecPlayerFunc(void)
{

}
/**
* @brief  Display the About menu.
* @param  None
* @retval None
*/
void Menu_AboutFunc(void)
{
  LCD_Clear(LCD_COLOR_WHITE);
  Demo_IntExtOnOffConfig(DISABLE);
  while (Menu_ReadKey() != NOKEY)
  {}
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_ClearLine(LCD_LINE_0);
  LCD_DisplayStringLine(LCD_LINE_1, "   STM32L152D_EVAL  ");
  LCD_DisplayStringLine(LCD_LINE_2, "   Demonstration    ");
  LCD_DisplayStringLine(LCD_LINE_3, "   Version 1.0.0    ");
  LCD_DisplayStringLine(LCD_LINE_4, "   24-January-2012  ");
  LCD_ClearLine(LCD_LINE_5);
  LCD_DisplayStringLine(LCD_LINE_6, "MCD Application Team");
  LCD_DisplayStringLine(LCD_LINE_7, "   COPYRIGHT 2012   ");
  LCD_DisplayStringLine(LCD_LINE_8, " STMicroelectronics ");
  LCD_DisplayStringLine(LCD_LINE_9, "  www.st.com/stm32l ");
  while (Menu_ReadKey() == NOKEY)
  {}
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  Demo_IntExtOnOffConfig(ENABLE);
}

/**
* @brief  Display the About menu.
* @param  None
* @retval None
*/
void Menu_ProductPres(void)
{
  
}

/**
* @brief  Checks if the bitmapfiles (slides + icons) are already loaded in the
*         SD Card FLASH.
* @param  None
* @retval None
*/
uint32_t Menu_CheckBitmapFiles(void)
{
  uint32_t err = 0;
  uint32_t i = 0x300;
  uint8_t j = 0;
  
  LCD_Clear(LCD_COLOR_WHITE);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  if (SD_Detect() == SD_NOT_PRESENT)
  {    
    LCD_DisplayStringLine(LCD_LINE_1, "Please insert SDCard");
    while (SD_Detect() == SD_NOT_PRESENT)
    {}
    LCD_DisplayStringLine(LCD_LINE_1, "   SDCard detected  ");
    Demo_Delay(200);
  }
  
  /*------------------- FAT Initialization -------------------------------*/
  do
  {
    SD_LowLevel_DeInit();
    /** SDCARD Initialisation for FatFs                            */
    err = Storage_Init();
  }
  while ((err != 0) && (i-- != 0));
  
  /* SD Card not formatted */
  if (i == 0)
  {
    LCD_DisplayStringLine(LCD_LINE_1, "SDCard not formatted");
    LCD_DisplayStringLine(LCD_LINE_1, "Reprogram your card.");
    while (1)
    {}
  }
  /* Check icons if available on SD card under STFILES directory*/
  for (j = 0; j < NUMBER_OF_ICONS; j++)
  {
    if (f_open (&F, (const char*)IconsAddr[j], FA_READ))
    {
      return 1;
    }
    
    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    
    if (Menu_Buffercmp((uint8_t *)Icons64Check,  Buffer1, 6) != 0)
    {
      return 2;
    }
  }
  /* Check Slides if available on SD card under STFILES directory*/
  for (j = 0; j < NUMBER_OF_SLIDES; j++)
  {
    if (f_open (&F, (const char*)SlideAddr[j], FA_READ))
    {
      return 3;
    }
    
    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    
    if (Menu_Buffercmp((uint8_t *)SlidesCheck,  Buffer1, 6) != 0)
    {
      return 4;
    }
  }
  
  /* Check Alarm and Watch images if available on SD card under STFILES directory*/
  for (j = 0; j < 2; j++)
  {
    if (f_open (&F, (const char*)Images2Addr[j], FA_READ))
    {
      return 5;
    }
    
    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    
    if (Menu_Buffercmp((uint8_t *)Icons128Check,  Buffer1, 6) != 0)
    {
      return 6;
    }
  }
  /* Check STM32L, HELP and STLOGO images if available on SD card under STFILES directory */
  for (j = 0; j < 3; j++)
  {
    if (f_open (&F, (const char*)Images1Addr[j], FA_READ))
    {
      return 5;
    }
    
    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    
    if (Menu_Buffercmp((uint8_t *)SlidesCheck,  Buffer1, 6) != 0)
    {
      return 6;
    }
  }
  return 0;
}

/**
* @brief  Compares two buffers.
* @param  pBuffer1, pBuffer2: buffers to be compared.
* @param  BufferLength: buffer's length.
* @retval  0: pBuffer1 identical to pBuffer2
*          1: pBuffer1 differs from pBuffer2
*/
uint8_t Menu_Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  
  return 0;
}

/**
* @brief  Displays the temperature in Celsius and fahrenheit degree.
* @param  None
* @retval None
*/
void Menu_ThermometerFun(void)
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
void Rec_Inform_display(void)
{   
  
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font12x12);
  
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Initialize table */
  LCD_SetTextColor(Black);
  
  /* Check if the LCD is HX8347D Controller */
  if(LCD_ReadReg(0x00) == LCD_HX8347D)
  {
    /* Display 16bits Stereo */
    LCD_DisplayChar(60 , 170, 'R'); 
    LCD_DisplayChar(60 , 182, 'E'); 
    LCD_DisplayChar(60 , 194, 'C'); 
    LCD_DisplayChar(60 , 206, '_'); 
    LCD_DisplayChar(60 , 218, 'W'); 
    LCD_DisplayChar(60 , 230, 'A'); 
    LCD_DisplayChar(60 , 242, 'V'); 
    LCD_DisplayChar(60 , 254, 'E'); 
    
    /* Display 8 bits MONO */
    LCD_DisplayChar(84 , 170, '8'); 
    LCD_DisplayChar(84 , 182, 'B'); 
    LCD_DisplayChar(84 , 194, 'i'); 
    LCD_DisplayChar(84 , 206, 't'); 
    LCD_DisplayChar(84 , 218, 's'); 
    LCD_DisplayChar(84 , 230, ','); 
    LCD_DisplayChar(84 , 242, 'M'); 
    LCD_DisplayChar(84 , 254, 'O'); 
    LCD_DisplayChar(84 , 266, 'N'); 
    LCD_DisplayChar(84 , 278, 'O'); 
    
    
    /* Sampling Frequency  */
    LCD_DisplayChar(72 , 170, '3'); 
    LCD_DisplayChar(72 , 182, '1'); 
    LCD_DisplayChar(72 , 194, '2'); 
    LCD_DisplayChar(72 , 206, '5'); 
    LCD_DisplayChar(72 , 218, '0'); 
    LCD_DisplayChar(72 , 230, 'H'); 
    LCD_DisplayChar(72 , 242, 'z'); 
  }
  else
  {
    /* Display 16bits Stereo */
    LCD_DisplayChar(60 , 170, 'R'); 
    LCD_DisplayChar(60 , 158, 'E'); 
    LCD_DisplayChar(60 , 146, 'C'); 
    LCD_DisplayChar(60 , 134, '_'); 
    LCD_DisplayChar(60 , 122, 'W'); 
    LCD_DisplayChar(60 , 110, 'A'); 
    LCD_DisplayChar(60 , 98, 'V'); 
    LCD_DisplayChar(60 , 86, 'E'); 
    
    /* Display 8 bits MONO */
    LCD_DisplayChar(84 , 170, '8'); 
    LCD_DisplayChar(84 , 158, 'B'); 
    LCD_DisplayChar(84 , 146, 'i'); 
    LCD_DisplayChar(84 , 134, 't'); 
    LCD_DisplayChar(84 , 122, 's'); 
    LCD_DisplayChar(84 , 110, ','); 
    LCD_DisplayChar(84 , 98, 'M'); 
    LCD_DisplayChar(84 , 86, 'O'); 
    LCD_DisplayChar(84 , 74, 'N'); 
    LCD_DisplayChar(84 , 62, 'O'); 
    
    
    /* Sampling Frequency  */
    LCD_DisplayChar(72 , 170, '3'); 
    LCD_DisplayChar(72 , 158, '1'); 
    LCD_DisplayChar(72 , 146, '2'); 
    LCD_DisplayChar(72 , 134, '5'); 
    LCD_DisplayChar(72 , 122, '0'); 
    LCD_DisplayChar(72 , 110, 'H'); 
    LCD_DisplayChar(72 , 98, 'z'); 
  }
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font16x24);
}

/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  
  sEEStatus = sEE_FAIL;
  NumDataRead = 0;
  /* Clear Acknowledge failure flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
  
  /* Generates I2C1 communication STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);

  return 0;
}

/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
uint8_t LM75_TIMEOUT_UserCallback(void)
{
  /* Clear Acknowledge failure flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
  
  /* Generates I2C1 communication STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
 return 0;
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
