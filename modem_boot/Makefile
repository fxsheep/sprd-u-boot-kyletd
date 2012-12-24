#
# Makefile for DWC_otg Highspeed USB controller driver
#

#PWD		:= $(shell pwd)
#PORTLIB		:= $(PWD)/../dwc_common_port

include $(TOPDIR)/config.mk

LIB	:= $(obj)bootup.o

COBJS-y += modem_boot.o
COBJS-y += crc16.o
COBJS-y += packet.o
COBJS-y += req_clk_gpio.o
COBJS-$(CONFIG_SPRD_SPI) += spi_channel_api.o
COBJS-y += sdio_channel_api.o
COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))


all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

$(obj)%.o:	$(obj)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
