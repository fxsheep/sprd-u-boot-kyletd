#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(SOC).o

COBJS	= generic.o timer.o reset.o ldo.o adi_drv.o mfp_sprd.o check_reboot.o gpio_phy.o gpio.o chg_phy.o vibrator.o mmu.o adc.o rtc-sprd.o
SOBJS += mmu_asm.o
ifndef CONFIG_SKIP_LOWLEVEL_INIT
COBJS   += lowlevel_init.o sdram.o remap.o sdram_prod.o mmu.o chip_cfg.o 
SOBJS   = cpu_spec.o mmu_asm.o
endif

ifdef CONFIG_HW_WATCHDOG
COBJS  += watchdog.o watchdog_phy_v3.o watchdog_hal.o
endif

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS) )

all:	$(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
