#
# Makefile for DWC_otg Highspeed USB controller driver
#

#PWD		:= $(shell pwd)
#PORTLIB		:= $(PWD)/../dwc_common_port

include $(TOPDIR)/config.mk

LIB	:= $(obj)libdwc_otg.o
#CFLAGS	+= -DDEBUG 
CFLAGS	+= -DDWC_LINUX
CPPFLAGS	+= -DDWC_LINUX

# Use one of the following flags to compile the software in host-only or
# device-only mode.
#EXTRA_CFLAGS        += -DDWC_HOST_ONLY
CFLAGS	+= -DDWC_DEVICE_ONLY

CFLAGS	+= -Dlinux -DDWC_HS_ELECT_TST
#EXTRA_CFLAGS	+= -DDWC_EN_ISOC
#EXTRA_CFLAGS   	+= $(CFI)

#obj-y		+= dwc_otg.o

#dwc_otg-objs	:= dwc_otg_driver.o
#dwc_otg-objs	+= dwc_otg_cil.o dwc_otg_cil_intr.o
#dwc_otg-objs	+= dwc_otg_pcd_linux.o dwc_otg_pcd.o dwc_otg_pcd_intr.o
#dwc_otg-objs	+= dwc_common_linux.o


COBJS-y += dwc_otg_driver.o
COBJS-y += dwc_otg_cil.o dwc_otg_cil_intr.o
COBJS-y += dwc_otg_pcd_linux.o dwc_otg_pcd.o dwc_otg_pcd_intr.o
COBJS-y += dwc_common_linux.o

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
