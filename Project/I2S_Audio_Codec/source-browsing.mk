.PHONY: tags
tags:
	ctags --fields=+l -R . -R $(PROJ_SRCS) $(LIB_SRCS) $(SYSCALLS_SRCS) $(INCLUDE_DIRS)

include-dirs:
	@echo $(INCLUDE_DIRS) > $@

cflags-basic:
	@echo $(STM32_MCU_CFLAGS) > $@

DERIVED_FILES += tags
