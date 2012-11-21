
include  $(TOPDIR)/config.mk
include  $(TOPDIR)/nand_fdl/board/$(BOARDDIR)/config.mk
include  $(TOPDIR)/board/$(BOARDDIR)/config.mk
sinclude $(TOPDIR)/include/idh_config.mk

gcclibdir    := $(shell dirname `$(CC) -print-libgcc-file-name`)

LDSCRIPT      = $(TOPDIR)/nand_fdl/board/$(BOARDDIR)/fdl1.lds
FDL1_LDFLAGS  = -Bstatic -T $(LDSCRIPT) $(PLATFORM_LDFLAGS)

AFLAGS       += -I $(OBJTREE)/include/asm/arch
CFLAGS       += -I $(OBJTREE)/include/asm/arch

FDL1_PATH     = $(TOPDIR)/nand_fdl/fdl-1
COM_PATH      = $(TOPDIR)/nand_fdl/common

FDL1_COBJ_FILE= migrate.o fdl_main.o fdl_command.o
FDL1_SOBJ_FILE= init.o
FDL1_COBJS    = $(addprefix $(obj),$(FDL1_COBJ_FILE))
FDL1_SOBJS    = $(addprefix $(obj),$(FDL1_SOBJ_FILE))
FDL1_OBJS     = $(FDL1_COBJS) $(FDL1_SOBJS)

ARCH_LIB      =
DRIVERS_LIB   = $(OBJTREE)/drivers/serial/serial_sc8800x.o
GENERIC_LIB   = 

COM_OBJ_FILE  = dl_engine.o fdl_crc.o packet.o fdl_channel.o fdl_stdio.o usb_boot.o drv_usb.o virtual_com.o string.o 
COM_OBJS      = $(addprefix $(obj),$(COM_OBJ_FILE))

CPU_OBJ_FILE  = chip_cfg.o mmu_asm.o mmu.o
ifdef CONFIG_SC8825
CPU_OBJ_FILE += sdram.o
endif

ifdef CONFIG_SC7710G2
CPU_OBJ_FILE += sdram_sc7710g2.o sdram_prod_sc7710g2.o
endif

CPU_LIB       = $(addprefix $(obj),$(CPU_OBJ_FILE))

UBOOT_LIBS    = $(ARM_LIB) $(COMMON_LIB) $(GENERIC_LIB) $(DRIVERS_LIB)

FDL1_LNDIR   := $(OBJTREE)/nand_fdl/fdl-1

fdlobj       := $(OBJTREE)/nand_fdl/
fdl1_obj     := $(fdlobj)fdl-1/src/

ALL = $(fdlobj)fdl1.bin

all:$(ALL)

fdl1:$(ALL)

$(ALL) : $(fdlobj)fdl1.axf
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(fdlobj)fdl1.axf: $(FDL1_OBJS) $(COM_OBJS)  $(CPU_LIB) $(UBOOT_LIBS)
	@mkdir -p $(FDL1_LNDIR)
	cd $(FDL1_LNDIR) && $(LD) $(FDL1_LDFLAGS) \
		$(FDL1_COBJ_FILE) $(FDL1_SOBJ_FILE) $(COM_OBJ_FILE) $(CPU_OBJ_FILE) $(UBOOT_LIBS) \
		-Map $(fdlobj)fdl1.map \
		-o $@ $(PLATFORM_LIBS)

#########################################################################

ifndef CONFIG_IDH_BUILD
$(obj)%.o:$(SRCTREE)/nand_fdl/common/src/%.c
	$(CC) $(CFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(obj)%.o:$(SRCTREE)/nand_fdl/fdl-1/src/%.c
	$(CC) $(CFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(obj)%.o:$(SRCTREE)/nand_fdl/fdl-1/src/%.S
	$(CC) $(AFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(obj)%.o:$(SRCTREE)/arch/$(ARCH)/cpu/$(CPU)/$(SOC)/%.S
	$(CC) $(AFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
$(obj)%.o:$(SRCTREE)/arch/$(ARCH)/cpu/$(CPU)/$(SOC)/%.c
	$(CC) $(AFLAGS) -I $(FDL1_PATH)/inc -I $(COM_PATH)/inc -c -o $@ $<
endif

include $(SRCTREE)/rules.mk

sinclude $(obj).depend

.PHONY:clean
clean:
	rm -f *.bin *.map fdl1.axf

#########################################################################
