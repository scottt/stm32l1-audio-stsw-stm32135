-include gcc-arm-embedded.mk

# STM32_MCU_CFLAGS, see stm32l1xx.h
STM32SW_DIR := ../..

# Only STM32L1XX_HD has the SDIO interface
STM32_MCU_CFLAGS := -DSTM32L1XX_HD

# STM32_USB-FS-Device_Lib spells MDP as MD_PLUS
STM32_USBLIB_MCU_CFLAGS := -DSTM32L1XX_MD_PLUS
# FIXME: defining USE_STM32L152_EVAL, which is an eval board, to suite STM32_USB-FS-Device_Lib_V4.0.0
# We're really using an STM32L1XX_MDP chip on a custom board
STM32_USBLIB_MCU_CFLAGS += -DUSE_STM32L152D_EVAL

LINKER_SCRIPT := ./TrueSTUDIO/STM32L152D-EVAL_Audio_Demo/STM32L152ZD_FLASH.ld
LIBRARIES_DIR := $(STM32SW_DIR)/Libraries
UTILITIES_DIR := $(STM32SW_DIR)/Utilities
BOARD_DIR := $(UTILITIES_DIR)/STM32_EVAL/STM32L152D_EVAL
BOARD_COMMON_DIR := $(UTILITIES_DIR)/STM32_EVAL/Common
THIRD_PARTY_DIR := $(UTILITIES_DIR)/Third_Party
STARTUP_ASM_SRC := $(LIBRARIES_DIR)/CMSIS/Device/ST/STM32L1xx/Source/Templates/TrueSTUDIO/startup_stm32l1xx_mdp.s

PROJ_NAME := audio

CFLAGS := -g3 -Os -mcpu=cortex-m4 -mthumb -fno-common

CFLAGS += $(STM32_MCU_CFLAGS) \
	  $(STM32_USBLIB_MCU_CFLAGS)

CFLAGS += -DUSE_STDPERIPH_DRIVER \
	  -DUSE_FULL_ASSERT=1 \
          -DUSB_USE_EXTERNAL_PULLUP \
          -DUSE_TIMEOUT_USER_CALLBACK

LDFLAGS := '-T$(LINKER_SCRIPT)'

# CHIP_PERIPH: chip level peripherals
CHIP_PERIPH_DIR := $(LIBRARIES_DIR)/STM32L1xx_StdPeriph_Driver
CHIP_PERIPH_SRCS := $(wildcard $(CHIP_PERIPH_DIR)/src/*.c)

CHIP_PERIPH_INCLUDE_DIR := $(CHIP_PERIPH_DIR)/inc
INCLUDE_DIRS += $(CHIP_PERIPH_INCLUDE_DIR)

# STM32_USB-FS-Device_Driver: USB stack
USB_STACK_DIR := $(LIBRARIES_DIR)/STM32_USB-FS-Device_Driver
USB_STACK_SRCS := $(wildcard $(USB_STACK_DIR)/src/*.c)
USB_STACK_INCLUDE_DIR := $(USB_STACK_DIR)/inc
INCLUDE_DIRS += $(USB_STACK_INCLUDE_DIR)

CMSIS_DEVICE_DIR := $(LIBRARIES_DIR)/CMSIS/Device/ST/STM32L1xx
CMSIS_DEVICE_INCLUDE_DIR := $(CMSIS_DEVICE_DIR)/Include
INCLUDE_DIRS += $(CMSIS_DEVICE_INCLUDE_DIR)

CMSIS_INCLUDE_DIR := $(LIBRARIES_DIR)/CMSIS/Include
INCLUDE_DIRS += $(CMSIS_INCLUDE_DIR)

# BOARD: board level peripherals
BOARD_SRCS := $(wildcard $(BOARD_DIR)/*.c $(BOARD_COMMON_DIR)/*.c)
# filter out unused board level peripheral helper code that won't build
BOARD_SRCS := $(filter-out ../../Utilities/STM32_EVAL/Common/lcd_log.c, $(BOARD_SRCS))

BOARD_INCLUDE_DIR := $(BOARD_DIR) $(BOARD_COMMON_DIR)
INCLUDE_DIRS += $(BOARD_INCLUDE_DIR)

#./Utilities/Third_Party/FatFs_vR0.08a/ff.h
FATFS_DIR := $(THIRD_PARTY_DIR)/FatFs_vR0.08a
FATFS_SRCS := $(wildcard $(FATFS_DIR)/*.c)
FATFS_INCLUDE_DIR := $(FATFS_DIR)
INCLUDE_DIRS += $(FATFS_INCLUDE_DIR)

# SYSCALLS: functions for the C library, e.g. _exit()
SYSCALLS_SRCS := TrueSTUDIO/syscalls.c
SYSCALLS_OBJS := $(SYSCALLS_SRCS:.c=.o)

LIB_SRCS := $(CHIP_PERIPH_SRCS) $(BOARD_SRCS) $(FATFS_SRCS) $(USB_STACK_SRCS)
LIB_OBJS := $(LIB_SRCS:.c=.o)

PROJ_SRCS := $(wildcard src/*.c)
INCLUDE_DIRS += inc

PROJ_OBJS := $(PROJ_SRCS:.c=.o) $(STARTUP_ASM_SRC:.s=.o)
OBJS := $(PROJ_OBJS) $(LIB_OBJS) $(SYSCALLS_OBJS)

CFLAGS += $(addprefix -I, $(INCLUDE_DIRS))
DERIVED_FILES := $(OBJS) libstm32l1.a $(PROJ_NAME).bin $(PROJ_NAME).elf

.PHONY: all clean dfu-install
all: libstm32l1.a $(PROJ_NAME).bin
libstm32l1.a: $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

$(PROJ_NAME).elf: LDFLAGS +=-L. -lstm32l1
$(PROJ_NAME).elf: libstm32l1.a $(PROJ_OBJS) $(SYSCALLS_OBJS)
	$(CC) $(CFLAGS) $(PROJ_OBJS) $(LDFLAGS) $(SYSCALLS_OBJS) -o $@

$(PROJ_NAME).bin: $(PROJ_NAME).elf
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f $(DERIVED_FILES)

dfu-intsall: $(PROJ_NAME).bin
	dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x8000000  -D $<

include debugger.mk
include source-browsing.mk
