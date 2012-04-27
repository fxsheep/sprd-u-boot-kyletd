/******************************************************************************
 ** File Name:    usb_boot.c                                                  *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         4/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include <asm/arch/usb_boot.h>
#include <asm/arch/drv_usb.h>
#include <asm/arch/virtual_com.h>
#include <asm/arch/fdl_crc.h>
#include <asm/arch/fdl_channel.h>
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
//AHB Register
#define AHB_CTRL0                           (0x20900200)
#define USBD_EN                                     BIT_5
#define AHB_CONTROL_REG3                    (0x2090020c)
#define CLK_USB_REF_SEL                     BIT_1
#define USB_S_HBIGENDIAN                    BIT_2
#define CLK_USB_REF_EN                      BIT_6
//#define AHB_SOFT_RST                      (0x20900210)
//#define USBPHY_SOFT_RST                       BIT_7
//#define AHB_ARM_CLK                           (0x20900224)

#define USB_BUFF_SIZE  0x4000
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/

/*--------------------------- Local Data ------------------------------------*/
LOCAL __align(32) uint8 s_usb_snd_buff[USB_BUFF_SIZE];
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

void usb_write (unsigned char *write_buf,  unsigned int write_len);
void usb_init (unsigned long ext_clk26M);

static void SetPllClock (void)
{
    uint32 clk;
    clk = * (volatile uint32 *) (AHB_ARM_CLK);
    clk &= ~ ( (3 << 23) | (7 << 4) | (7));
    clk |= ( (1 << 23) | (1 << 4) | (0));

    * (volatile uint32 *) (AHB_ARM_CLK) = clk;
}

/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:         Daniel.Ding
//    Note:
/*****************************************************************************/
void usb_init (unsigned long ext_clk26M)
{
#if (!defined(_LITTLE_ENDIAN) && !defined(CHIP_ENDIAN_LITTLE))
    *(volatile uint32 *)AHB_CONTROL_REG3 |= USB_S_HBIGENDIAN;    //master AHB interface: Big Endian
#else
    *(volatile uint32 *)AHB_CONTROL_REG3 &= ~USB_S_HBIGENDIAN;    //master AHB interface: Little Endian
#endif
    
    *(volatile uint32 *)AHB_CTRL0 |= USBD_EN;  // usbd enable
	
    usb_core_init();
    
}
/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:         Daniel.Ding
//    Note:
/*****************************************************************************/
void usb_boot (uint32 ext_clk26M)
{
    usb_init (ext_clk26M);
}
void usb_varinit (void)
{
    usb_init (0);
}
/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void usb_ldo_enable (BOOLEAN is_usb_ldo_enabled)
{
    volatile unsigned long usb_ldo = 0;

    // usb_ldo  = *(volatile unsigned long *)(GLOBAL_CTL_LDO);

    if (is_usb_ldo_enabled)
    {
        usb_ldo &= ~ (BIT_10);
        usb_ldo |= (BIT_11);
    }
    else
    {
        usb_ldo |= (BIT_10);
        usb_ldo &= ~ (BIT_11);
    }

    //*(volatile unsigned long *)(GLOBAL_CTL_LDO) = usb_ldo;

    return;
}
/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:         Daniel.Ding
//    Note:
/*****************************************************************************/
void usb_write (unsigned char *write_buf,unsigned int write_len)
{
    memcpy (s_usb_snd_buff, write_buf, write_len);
    USB_EPxSendData (USB_EP5 , (unsigned int *) s_usb_snd_buff, write_len);
}
extern char VCOM_GetChar (void);
extern int VCOM_GetSingleChar (void);
static int FDL_UsbOpen (struct FDL_ChannelHandler  *channel, unsigned int baudrate)
{
    return 0;
}
static int FDL_UsbRead (struct FDL_ChannelHandler  *channel, const unsigned char *buf, unsigned int len)
{
    return -1;
}
static char FDL_UsbGetChar (struct FDL_ChannelHandler  *channel)
{
    return VCOM_GetChar();
}
static int FDL_UsbGetSingleChar (struct FDL_ChannelHandler  *channel)
{
    return VCOM_GetSingleChar();
}
static int FDL_UsbWrite (struct FDL_ChannelHandler  *channel, const unsigned char *buf, unsigned int len)
{
    usb_write ( (unsigned char *) buf, len);
    return 0;
}

static int FDL_UsbPutChar (struct FDL_ChannelHandler  *channel, const unsigned char ch)
{
    s_usb_snd_buff[0] = ch;
    //int data = ch;
    USB_EPxSendData (USB_EP3 , (unsigned int *) &s_usb_snd_buff, 1);
    return 0;
}

static int FDL_UsbSetBaudrate (struct FDL_ChannelHandler  *channel,  unsigned int baudrate)
{
    return 0;
}
static int FDL_UsbClose (struct FDL_ChannelHandler  *channel)
{
    return 0;
}
struct FDL_ChannelHandler gUSBChannel =
{
    FDL_UsbOpen,
    FDL_UsbRead,
    FDL_UsbGetChar,
    FDL_UsbGetSingleChar,
    FDL_UsbWrite,
    FDL_UsbPutChar,
    FDL_UsbSetBaudrate,
    FDL_UsbClose,
    0
};

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
