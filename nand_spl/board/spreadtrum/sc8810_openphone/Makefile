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
CONFIG_NAND_SPL	= y

include $(TOPDIR)/config.mk
include $(TOPDIR)/nand_spl/board/$(BOARDDIR)/config.mk

LDSCRIPT= $(TOPDIR)/nand_spl/board/$(BOARDDIR)/u-boot.lds
LDFLAGS	= -Bstatic -T $(LDSCRIPT) -Ttext $(CONFIG_SYS_TEXT_BASE) $(PLATFORM_LDFLAGS)
AFLAGS	+= -DCONFIG_PRELOADER -DCONFIG_NAND_SPL
CFLAGS	+= -DCONFIG_PRELOADER -DCONFIG_NAND_SPL -g

SOBJS	= start.o _udivsi3.o _divsi3.o _umodsi3.o cpu_spec.o mmu_asm.o 
COBJS	= nand_boot.o nand_ecc.o sprd_nand.o lowlevel_init.o div0.o adi_drv.o sdram.o mmu.o sdram_prod.o chip_cfg.o serial_sc8800x.o

SRCS	:= $(SRCTREE)/nand_spl/nand_boot_fsl_nfc.c
SRCS	+= $(SRCTREE)/arch/arm/cpu/arm926ejs/start.S
SRCS	+= $(SRCTREE)/arch/arm/cpu/arm926ejs/sc8800g/lowlevel_init.c
SRCS	+= $(SRCTREE)/arch/arm/cpu/arm926ejs/sc8800g/clk_config.c
SRCS    += $(SRCTREE)/arch/arm/cpu/arm926ejs/sc8800g/sdram.c
SRCS    += $(SRCTREE)/arch/arm/cpu/arm926ejs/sc8800g/adi_drv.c
SRCS    += $(SRCTREE)/arch/arm/cpu/arm926ejs/sc8800g/remap.c
SRCS    += $(SRCTREE)/arch/arm/lib/_udivsi3.S
SRCS    += $(SRCTREE)/arch/arm/lib/_divsi3.S
SRCS    += $(SRCTREE)/arch/arm/lib/div0.c

OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
__OBJS	:= $(SOBJS) $(COBJS)
LNDIR	:= $(OBJTREE)/nand_spl/board/$(BOARDDIR)

nandobj	:= $(OBJTREE)/nand_spl/

ALL	= $(nandobj)u-boot-spl $(nandobj)u-boot-spl.bin $(nandobj)u-boot-spl-16k.bin

#all:	$(obj).depend $(ALL)
all:	$(ALL)

$(nandobj)u-boot-spl-16k.bin: $(nandobj)u-boot-spl
	$(OBJCOPY) ${OBJCFLAGS} --pad-to $(PAD_TO) -O binary $< $@

$(nandobj)u-boot-spl.bin:	$(nandobj)u-boot-spl
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(nandobj)u-boot-spl:	$(OBJS) $(LDSCRIPT)
	cd $(LNDIR) && $(LD) $(LDFLAGS) $(__OBJS) \
		-Map $(nandobj)u-boot-spl.map \
		-o $@

#########################################################################

$(obj)%.o:	$(SRCTREE)/arch/arm/cpu/arm926ejs/%.S
	$(CC) $(AFLAGS) -mlittle-endian -c -o $@ $<

$(obj)%.o:	$(SRCTREE)/arch/arm/cpu/arm926ejs/sc8800g/%.c
	$(CC) $(CFLAGS) -mlittle-endian -c -o $@ $<

$(obj)%.o:	$(SRCTREE)/nand_spl/%.c
	$(CC) $(CFLAGS) -mlittle-endian -c -o $@ $<

$(obj)%.o:	$(SRCTREE)/arch/arm/lib/%.S
	$(CC) $(AFLAGS) -mlittle-endian -c -o $@ $<

$(obj)%.o:      $(SRCTREE)/arch/arm/lib/%.c
	$(CC) $(CFLAGS) -mlittle-endian -c -o $@ $<

$(obj)%.o:      $(SRCTREE)/drivers/mtd/nand/%.c
	$(CC) $(CFLAGS) -mlittle-endian -c -o $@ $<

$(obj)%.o:	$(SRCTREE)/arch/arm/cpu/arm926ejs/sc8800g/%.S
	$(CC) $(AFLAGS) -mlittle-endian -c -o $@ $<

$(obj)%.o:	$(SRCTREE)/drivers/serial/%.c
	$(CC) $(CFLAGS) -mlittle-endian -c -o $@ $<
# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
