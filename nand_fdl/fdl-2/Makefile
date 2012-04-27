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
FDL2_LDFLAGS	= -Bstatic -T $(LDSCRIPT) $(PLATFORM_LDFLAGS)
AFLAGS	+= -I $(OBJTREE)/include/asm/arch
CFLAGS	+= -I $(OBJTREE)/include/asm/arch

FDL2_PATH = $(TOPDIR)/nand_fdl/fdl-2
COM_PATH = $(TOPDIR)/nand_fdl/common

EMMC_PATH = $(TOPDIR)/drivers/mmc

ifdef CONFIG_EMMC_BOOT
UEFI_LIB = $(OBJTREE)/disk/libdisk.o
EXT4_LIB = $(OBJTREE)/fs/ext4/libext4fs.o $(OBJTREE)/common/cmd_ext4.o
EMMC_LIB = $(OBJTREE)/drivers/mmc/libmmc.o $(OBJTREE)/arch/arm/cpu/armv7/sc8810/ldo.o
endif

FDL2_SOBJS_LIST = init.o

COM_COBJS	= dl_engine.o fdl_crc.o packet.o fdl_channel.o fdl_stdio.o mcu_command.o usb_boot.o drv_usb.o virtual_com.o
COM_OBJS = $(addprefix $(obj),$(COM_COBJS))

FDL2_COBJS_LIST = migrate.o fdl2_signature.o fdl_main.o fdl_nand.o flash_command.o parsemtdparts.o
ifdef CONFIG_EMMC_BOOT
FDL2_COBJS_LIST += fdl_emmc_command.o fdl_partition.o
endif

FDL2_COBJS = $(addprefix $(obj),$(FDL2_COBJS_LIST))
FDL2_SOBJS = $(addprefix $(obj),$(FDL2_SOBJS_LIST))
FDL2_OBJS = $(FDL2_COBJS) $(FDL2_SOBJS)

MTD_LIB = $(OBJTREE)/drivers/mtd/libmtd.o 
#ARM_LIB = $(OBJTREE)/arch/arm/lib/libarm.a
NAND_LIB = $(OBJTREE)/drivers/mtd/nand/libnand.o
YAFFS2_LIB = $(OBJTREE)/fs/yaffs2/libyaffs2.o
COMMON_OBJ = dlmalloc.o idh_layout.o dload_op.o
COMMON_LIB = $(addprefix $(OBJTREE)/common/,$(COMMON_OBJ))
DRIVERS_LIB = $(OBJTREE)/drivers/serial/serial_sc8800x.o
ARCH_LIB =
ifndef CONFIG_SC8810
CPU_OBJ = timer.o chip_cfg.o chip_phy_v3.o boot_mode.o watchdog_hal.o watchdog_phy_v3.o adi_drv.o mmu.o mmu_asm.o rtc-sprd.o
CPU_LIB = $(addprefix $(obj),$(CPU_OBJ))
else

CPU_OBJ = timer.o chip_cfg.o chip_phy_v3.o boot_mode.o watchdog_hal.o watchdog_phy_v3.o adi_drv.o mmu.o mmu_asm.o
CPU_LIB = $(addprefix $(obj),$(CPU_OBJ))
endif

GENERIC_LIB = $(OBJTREE)/lib/libgeneric.o
ifdef CONFIG_EMMC_BOOT
UBOOT_LIBS = $(MTD_LIB) $(ARM_LIB) $(NAND_LIB) $(YAFFS2_LIB) $(COMMON_LIB) $(CPU_LIB) $(GENERIC_LIB) $(DRIVERS_LIB) $(ARCH_LIB) $(EMMC_LIB) $(UEFI_LIB) $(EXT4_LIB)
else
UBOOT_LIBS = $(MTD_LIB) $(ARM_LIB) $(NAND_LIB) $(YAFFS2_LIB) $(COMMON_LIB) $(CPU_LIB) $(GENERIC_LIB) $(DRIVERS_LIB) $(ARCH_LIB)
endif

#ALL_UBOOT_LIBS = $(addprefix $(OBJTREE),$(__LIBS))
#UBOOT_LIBS = $(ALL_UBOOT_LIBS)

FDL2_LNDIR	:= $(OBJTREE)/nand_fdl/fdl-2

fdlobj	:= $(OBJTREE)/nand_fdl/
fdl2_obj	:= $(fdlobj)fdl-2/src/

ALL	= $(fdlobj)fdl2.bin

all:	 $(ALL)

fdl2:  $(fdlobj)fdl2.bin

$(fdlobj)fdl2.bin:	$(fdlobj)fdl2
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(fdlobj)fdl2: $(FDL2_OBJS) $(COM_OBJS) $(UBOOT_LIBS)
	@mkdir -p $(FDL2_LNDIR)
	cd $(FDL2_LNDIR) && $(LD) $(FDL2_LDFLAGS) -Ttext $(FDL2_TEXT_BASE) \
		--start-group $(FDL2_COBJS_LIST) $(FDL2_SOBJS_LIST) $(COM_OBJS) \
			$(UBOOT_LIBS) --end-group \
		-Map $(fdlobj)fdl2.map \
		-o $@ $(PLATFORM_LIBS)
#-L$(gcclibdir) -lgcc 

#########################################################################

ifndef CONFIG_IDH_BUILD
$(obj)%.o:$(SRCTREE)/nand_fdl/common/src/%.c
	$(CC) $(CFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(obj)%.o:$(SRCTREE)/nand_fdl/fdl-2/src/%.c
	$(CC) $(CFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -I $(EMMC_PATH) -c -o $@ $<
$(obj)%.o:$(SRCTREE)/nand_fdl/fdl-2/src/%.S
	$(CC) $(AFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(obj)%.o:$(SRCTREE)/arch/$(ARCH)/cpu/$(CPU)/$(SOC)/%.S
	$(CC) $(AFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(obj)%.o:$(SRCTREE)/arch/$(ARCH)/cpu/$(CPU)/$(SOC)/%.c
	$(CC) $(AFLAGS) -I $(FDL2_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
endif
# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

.PHONY:clean
clean:
	rm -f *.bin *.map fdl2

#########################################################################
