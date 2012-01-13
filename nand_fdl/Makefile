#
# (C) Copyright 2009 DENX Software Engineering
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundatio; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk
include $(TOPDIR)/nand_fdl/board/$(BOARDDIR)/config.mk
include $(TOPDIR)/board/$(BOARDDIR)/config.mk
sinclude $(TOPDIR)/include/idh_config.mk

gcclibdir := $(shell dirname `$(CC) -print-libgcc-file-name`)

LDSCRIPT= $(TOPDIR)/nand_fdl/board/$(BOARDDIR)/u-boot.lds
FDL1_LDFLAGS	= -Bstatic -T $(LDSCRIPT) $(PLATFORM_LDFLAGS)
FDL2_LDFLAGS	= -Bstatic -T $(LDSCRIPT) $(PLATFORM_LDFLAGS)
AFLAGS	+= -I $(TOPDIR)/include/asm/arch
CFLAGS	+= -I $(TOPDIR)/include/asm/arch

FDL2_PATH = $(TOPDIR)/nand_fdl/fdl-2
FDL1_PATH = $(TOPDIR)/nand_fdl/fdl-1
COM_PATH = $(TOPDIR)/nand_fdl/common

FDL2_SOBJS_LIST = init.o
FDL1_SOBJS_LIST = init.o

COM_COBJS	= dl_engine.o fdl_crc.o packet.o fdl_channel.o fdl_stdio.o mcu_command.o usb_boot.o drv_usb.o virtual_com.o
COM_OBJS = $(addprefix $(COM_PATH)/src/,$(COM_COBJS))

FDL2_COBJS_LIST = migrate.o fdl2_signature.o fdl_main.o fdl_nand.o flash_command.o parsemtdparts.o
FDL1_COBJS_LIST = board_init.o chip_init.o fdl_command.o fdl_main.o fdl_stdio.o sdram_init.o sdram_prod.o
FDL1_COBJS = $(addprefix $(FDL1_PATH)/src/,$(FDL1_COBJS_LIST))
FDL1_SOBJS = $(addprefix $(FDL1_PATH)/src/,$(FDL1_SOBJS_LIST))
FDL1_OBJS = $(FDL1_COBJS) $(FDL1_SOBJS)
FDL2_COBJS = $(addprefix $(FDL2_PATH)/src/,$(FDL2_COBJS_LIST))
FDL2_SOBJS = $(addprefix $(FDL2_PATH)/src/,$(FDL2_SOBJS_LIST))
FDL2_OBJS = $(FDL2_COBJS) $(FDL2_SOBJS)

MTD_LIB = $(TOPDIR)/drivers/mtd/libmtd.o 
#ARM_LIB = $(TOPDIR)/arch/arm/lib/libarm.a
NAND_LIB = $(TOPDIR)/drivers/mtd/nand/libnand.o
YAFFS2_LIB = $(TOPDIR)/fs/yaffs2/libyaffs2.o
COMMON_OBJ = dlmalloc.o idh_layout.o dload_op.o
COMMON_LIB = $(addprefix $(TOPDIR)/common/,$(COMMON_OBJ))
DRIVERS_LIB = $(TOPDIR)/drivers/serial/serial_sc8800x.o
ARCH_LIB = 
ifndef CONFIG_SC8810
CPU_LIB = $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800g/timer.o $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800g/chip_cfg.o $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800g/chip_phy_v3.o $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800g/boot_mode.o $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800g/watchdog_hal.o $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800g/watchdog_phy_v3.o $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800g/adi_drv.o $(TOPDIR)/nand_spl/board/$(BOARDDIR)/mmu.o $(TOPDIR)/nand_spl/board/$(BOARDDIR)/mmu_asm.o $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800g/rtc-sprd.o
else

CPU_LIB = $(TOPDIR)/arch/arm/cpu/armv7/sc8810/timer.o $(TOPDIR)/arch/arm/cpu/armv7/sc8810/chip_cfg.o $(TOPDIR)/arch/arm/cpu/armv7/sc8810/chip_phy_v3.o $(TOPDIR)/arch/arm/cpu/armv7/sc8810/boot_mode.o $(TOPDIR)/arch/arm/cpu/armv7/sc8810/watchdog_hal.o $(TOPDIR)/arch/arm/cpu/armv7/sc8810/watchdog_phy_v3.o $(TOPDIR)/arch/arm/cpu/armv7/sc8810/adi_drv.o $(TOPDIR)/nand_spl/board/$(BOARDDIR)/mmu.o $(TOPDIR)/nand_spl/board/$(BOARDDIR)/mmu_asm.o 
endif

GENERIC_LIB = $(TOPDIR)/lib/libgeneric.o
UBOOT_LIBS = $(MTD_LIB) $(ARM_LIB) $(NAND_LIB) $(YAFFS2_LIB) $(COMMON_LIB) $(CPU_LIB) $(GENERIC_LIB) $(DRIVERS_LIB) $(ARCH_LIB)

#ALL_UBOOT_LIBS = $(addprefix $(obj),$(__LIBS))
#UBOOT_LIBS = $(ALL_UBOOT_LIBS)

FDL1_LNDIR	:= $(OBJTREE)/nand_fdl/fdl-1/src
FDL2_LNDIR	:= $(OBJTREE)/nand_fdl/fdl-2/src

fdlobj	:= $(OBJTREE)/nand_fdl/
fdl1_obj	:= $(fdlobj)fdl-1/src/
fdl2_obj	:= $(fdlobj)fdl-2/src/

ALL	= $(fdlobj)fdl2.bin $(fdlobj)fdl1.bin

#all:	$(obj).depend $(ALL)
#
#fdl1: $(obj).depend $(fdlobj)fdl1.bin
#fdl2: $(obj).depend $(fdlobj)fdl2.bin
all:	 $(ALL)

fdl1:  $(fdlobj)fdl1.bin
fdl2:  $(fdlobj)fdl2.bin

$(fdlobj)fdl1.bin: $(fdlobj)fdl1
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(fdlobj)fdl2.bin:	$(fdlobj)fdl2
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(fdlobj)fdl1: $(FDL1_OBJS) $(COM_OBJS) $(UBOOT_LIBS)
	cd $(FDL1_LNDIR) && $(LD) $(FDL1_LDFLAGS) -Ttext $(FDL1_TEXT_BASE) \
		--start-group $(FDL1_COBJS_LIST) $(FDL1_SOBJS_LIST) $(COM_OBJS) \
			$(UBOOT_LIBS) --end-group \
		-Map $(fdlobj)fdl1.map \
		-o $@ $(PLATFORM_LIBS)
#-L$(gcclibdir) -lgcc 

$(fdlobj)fdl2: $(FDL2_OBJS) $(COM_OBJS) $(UBOOT_LIBS) 
	cd $(FDL2_LNDIR) && $(LD) $(FDL2_LDFLAGS) -Ttext $(FDL2_TEXT_BASE) \
		--start-group $(FDL2_COBJS_LIST) $(FDL2_SOBJS_LIST) $(COM_OBJS) \
			$(UBOOT_LIBS) --end-group \
		-Map $(fdlobj)fdl2.map \
		-o $@ $(PLATFORM_LIBS)
#-L$(gcclibdir) -lgcc 

.PHONY:test
test:
	@echo $(TOPDIR)
	@echo ---------------------------------------
	@echo $(CFLAGS)
	@echo ---------------------------------------
	@echo $(AFLAGS)
	@echo ---------------------------------------
	
	
		

#########################################################################

ifndef CONFIG_IDH_BUILD
$(COM_OBJS):%.o:%.c
	$(CC) $(CFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(FDL1_COBJS):%.o:%.c
	$(CC) $(CFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(FDL1_SOBJS):%.o:%.S
	$(CC) $(AFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(FDL2_COBJS):%.o:%.c
	$(CC) $(CFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(FDL2_SOBJS):%.o:%.S
	$(CC) $(AFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
endif
# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

.PHONY:clean
clean:
	rm -f *.bin *.map fdl2 fdl1

#########################################################################
