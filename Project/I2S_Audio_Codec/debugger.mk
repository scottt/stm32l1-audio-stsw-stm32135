OPENOCD := /home/scottt/openocd/bin/openocd

.PHONY: openocd
openocd:
	$(OPENOCD) -f board/stm32f429discovery.cfg

.PHONY: gdb
gdb:
	$(GDB) -q -x preferences.gdb -x openocd.gdb -ex "file $(PROJ_NAME).elf"

.PHONY: install
install: $(PROJ_NAME).bin
	$(error implement make install)

#.PHONY: gdb
#gdb:
#	easierocd-gdb --easierocd-file "$(PROJ_NAME).elf"
#
#.PHONY: install
#install: $(PROJ_NAME).bin
#	easierocd-program "$(PROJ_NAME).elf"
