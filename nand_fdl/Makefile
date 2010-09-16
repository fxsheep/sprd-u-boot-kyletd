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

gcclibdir := $(shell dirname `$(CC) -print-libgcc-file-name`)

LDSCRIPT= $(TOPDIR)/nand_fdl/board/$(BOARDDIR)/u-boot.lds
LDFLAGS	= -Bstatic -T $(LDSCRIPT) $(PLATFORM_LDFLAGS)
AFLAGS	+= 
CFLAGS	+= 

FDL2_PATH = $(TOPDIR)/nand_fdl/fdl-2
FDL1_PATH = $(TOPDIR)/nand_fdl/fdl-1
COM_PATH = $(TOPDIR)/nand_fdl/common

FDL2_SOBJS_LIST = init.o
FDL1_SOBJS_LIST = init.o

COM_COBJS	= dl_engine.o drv_usb.o fdl_crc.o packet.o sio_drv.o usb_boot.o virtual_com.o 
COM_OBJS = $(addprefix $(COM_PATH)/src/,$(COM_COBJS))

FDL2_COBJS_LIST = migrate.o dma_drv.o fdl_main.o fdl_nand.o flash_command.o mcu_command.o nand_controller.o readnandbuffer.o
FDL1_COBJS_LIST = board_init.o chip_init.o fdl_command.o fdl_main.o fdl_stdio.o sdram_init.o
FDL1_COBJS = $(addprefix $(FDL1_PATH)/src/,$(FDL1_COBJS_LIST))
FDL1_SOBJS = $(addprefix $(FDL1_PATH)/src/,$(FDL1_SOBJS_LIST))
FDL1_OBJS = $(FDL1_COBJS) $(FDL1_SOBJS)
FDL2_COBJS = $(addprefix $(FDL2_PATH)/src/,$(FDL2_COBJS_LIST))
FDL2_SOBJS = $(addprefix $(FDL2_PATH)/src/,$(FDL2_SOBJS_LIST))
FDL2_OBJS = $(FDL2_COBJS) $(FDL2_SOBJS)

MTD_LIB = $(TOPDIR)/drivers/mtd/libmtd.a 
#ARM_LIB = $(TOPDIR)/arch/arm/lib/libarm.a
NAND_LIB = $(TOPDIR)/drivers/mtd/nand/libnand.a
COMMON_LIB = $(TOPDIR)/common/dlmalloc.o 
CPU_LIB = $(TOPDIR)/arch/arm/cpu/arm926ejs/sc8800x/timer.o
GENERIC_LIB = $(TOPDIR)/lib/libgeneric.a
UBOOT_LIBS = $(MTD_LIB) $(ARM_LIB) $(NAND_LIB) $(COMMON_LIB) $(CPU_LIB) $(GENERIC_LIB)

#ALL_UBOOT_LIBS = $(addprefix $(obj),$(__LIBS))
#UBOOT_LIBS = $(ALL_UBOOT_LIBS)

LNDIR	:= $(OBJTREE)/nand_fdl

fdlobj	:= $(OBJTREE)/nand_fdl/

ALL	= $(fdlobj)fdl2.bin $(fdlobj)fdl1.bin

all:	$(obj).depend $(ALL)

fdl1: $(obj).depend $(fdlobj)fdl1.bin
fdl2: $(obj).depend $(fdlobj)fdl2.bin

$(fdlobj)fdl1.bin: $(fdlobj)fdl1
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(fdlobj)fdl2.bin:	$(fdlobj)fdl2
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(fdlobj)fdl1: $(FDL1_OBJS) $(COM_OBJS) $(UBOOT_LIBS)
	cd $(LNDIR) && $(LD) $(LDFLAGS) -Ttext $(FDL1_TEXT_BASE) \
		--start-group $^ --end-group \
		-Map $(fdlobj)fdl1.map \
		-o $@ 
#-L$(gcclibdir) -lgcc 

$(fdlobj)fdl2: $(FDL2_OBJS) $(COM_OBJS) $(UBOOT_LIBS)
	cd $(LNDIR) && $(LD) $(LDFLAGS) -Ttext $(FDL2_TEXT_BASE) \
		--start-group $^ --end-group \
		-Map $(fdlobj)fdl2.map \
		-o $@ $(PLATFORM_LIBS)
#-L$(gcclibdir) -lgcc 
		

#########################################################################

$(COM_OBJS):%.o:%.c
	$(CC) $(CFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(FDL1_COBJS):%.o:%.c
	$(CC) $(CFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(FDL1_SOBJS):%.o:%.s
	$(CC) $(SFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(FDL2_COBJS):%.o:%.c
	$(CC) $(CFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(FDL2_SOBJS):%.o:%.s
	$(CC) $(SFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
