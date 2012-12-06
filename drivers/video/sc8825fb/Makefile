#
# (C) Copyright 2000-2007
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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#


include $(TOPDIR)/config.mk

LIB	:= $(obj)libsc8825fb.o

#ifdef CONFIG_LCD
COBJS-$(CONFIG_TIGER) += sprdfb_main.o sprdfb_panel.o sprdfb_dispc.o sprdfb_mcu.o sprdfb_rgb.o sprdfb_mipi.o sprdfb_i2c.o sprdfb_spi.o sprdfb_dsi.o mipi_dsih_api.o mipi_dsih_dphy.o mipi_dsih_hal.o

COBJS-$(CONFIG_FB_LCD_S6D0139) += lcd_s6d0139.o
COBJS-$(CONFIG_FB_LCD_NT35516_MCU) += lcd_nt35516_mcu.o
COBJS-$(CONFIG_FB_LCD_NT35516_MIPI) += lcd_nt35516_mipi.o
COBJS-$(CONFIG_FB_LCD_NT35516_RGB_I2C) += lcd_nt35516_rgb_i2c.o
COBJS-$(CONFIG_FB_LCD_NT35516_RGB_SPI) += lcd_nt35516_rgb_spi.o
COBJS-$(CONFIG_FB_LCD_NT35510_MIPI) += lcd_nt35510_mipi.o
COBJS-$(CONFIG_FB_LCD_NT35512_MIPI) += lcd_nt35512_mipi.o
COBJS-$(CONFIG_FB_LCD_OTM8018B_MIPI) += lcd_otm_8018b_mipi.o
#endif

COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
