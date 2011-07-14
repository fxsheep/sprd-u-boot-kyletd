/******************************************************************************
 ** File Name:      DRV_usb.c                                                 *
 ** Author:         JiaYong.Yang                                              *
 ** DATE:           09/01/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "common.h"
#include "usb200_fdl.h"
#include "drv_usb20.h"
#include "virtual_com.h"

#define FDL2_MODULE	1

PUBLIC void Dcache_InvalRegion(void *addr, unsigned int length);
static __inline void usb_handler (void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#define MAX_RECV_LENGTH     (64*64)//640*64 0xa000
#define USB_TIMEOUT             (1000)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
LOCAL uint32    s_setup_packet[8] = {0};
LOCAL uint32    enum_speed = 0;
LOCAL uint32    recv_length = 0;
LOCAL uint32 nIndex = 0;
LOCAL uint32 readIndex = 0;

PUBLIC __align (32)  unsigned char usb_out_endpoint_buf[2] [MAX_RECV_LENGTH];
 
/*****************************************************************************/
//  Description:   configure out endpoint0 to receive setup message.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL int EPO0_config (BOOLEAN is_dma, uint32 *buffer)
{
    // Programs DOEPTSIZ0 register with Packet Count and Transfer Size
    * (volatile uint32 *) USB_DOEP0TSIZ |= (unsigned int) (BIT_29|BIT_30); //setup packet count , 3 packet
    * (volatile uint32 *) USB_DOEP0TSIZ |= (unsigned int) (BIT_3|BIT_4); //set Transfer Size ,24 bytes

    if (is_dma)
    {
        * (volatile uint32 *) USB_DOEPDMA (0) = (uint32) buffer;//lint !e718
    }

    * (volatile uint32 *) USB_DOEP0CTL |= (unsigned int) BIT_26;    // set clear NAK
    * (volatile uint32 *) USB_DOEP0CTL |= (unsigned int) BIT_31;    // set endpoint enable
    return 0;
}
/*****************************************************************************/
//  Description:   configure in endpoint0 to send message.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
void EPI0_config (uint32 transfer_size, uint32 packet_count, BOOLEAN is_dma, uint32 *buffer)
{
    volatile USB_DIEP0TSIZ_U *diep0tsiz_ptr = (USB_DIEP0TSIZ_U *) USB_DIEP0TSIZ;

    diep0tsiz_ptr->mBits.transfer_size = transfer_size;
    diep0tsiz_ptr->mBits.packet_count = packet_count;

    if (is_dma)
    {
        * (volatile uint32 *) USB_DIEPDMA (0) = (uint32) buffer;//lint !e718
    }

    * (volatile uint32 *) USB_DIEP0CTL &= (unsigned int) (~ (BIT_22|BIT_23|BIT_24|BIT_25)); // set EP0 in tx fifo nummber

    * (volatile uint32 *) USB_DIEP0CTL |= (unsigned int) BIT_26;                    // clear NAK

    * (volatile uint32 *) USB_DIEP0CTL |= (unsigned int) BIT_31;                    // set endpoint enable
}
/*****************************************************************************/
//  Description:   usb reset interrupt handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
void usb_EPActive (USB_EP_NUM_E ep_num, BOOLEAN dir)
{
    if (dir) // out endpoint
    {
        * (volatile uint32 *) USB_DOEPCTL (ep_num)  |= (unsigned int) BIT_15; //lint !e718// endpoint active
        * (volatile uint32 *) USB_DOEPMSK = (unsigned int) (BIT_13|BIT_12|BIT_3);
    }
    else
    {
        * (volatile uint32 *) USB_DIEPCTL (ep_num)  |= (unsigned int) BIT_15; //lint !e718// endpoint active
        * (volatile uint32 *) USB_DIEPMSK = 0xffffffff;
    }
}
/*****************************************************************************/
//  Description:   configure specified endpoint to send/receive message.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void EPn_config (USB_EP_NUM_E ep_num, USB_EP_TYPE_E ep_type, BOOLEAN dir, uint32 mps)
{
    // out endpoint
    if (dir)
    {
        volatile USB_DOEPCTL_U *doepctl_ptr = (USB_DOEPCTL_U *) USB_DOEPCTL (ep_num);

        doepctl_ptr->mBits.ep_type = ep_type;
        doepctl_ptr->mBits.mps =mps;
        doepctl_ptr->mBits.set_nak = 0x1;
    }
    else
    {
        volatile USB_DIEPCTL_U *diepctl_ptr = (USB_DIEPCTL_U *) USB_DIEPCTL (ep_num);

        diepctl_ptr->mBits.ep_type = ep_type;
        diepctl_ptr->mBits.mps = mps;
        diepctl_ptr->mBits.set_nak = 0x1;
    }
}

/*****************************************************************************/
//  Description:   start endpoint transfer.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void usb_start_transfer (USB_EP_NUM_E ep_num, BOOLEAN dir, uint32 transfer_size, BOOLEAN is_dma, uint32 *buffer)
{
    uint16 packet_count = 0;

    if (dir)
    {
        volatile USB_DOEPTSIZ_U *doeptsiz_ptr = (USB_DOEPTSIZ_U *) USB_DOEPTSIZ (ep_num);

        if (is_dma)
        {
            * (volatile uint32 *) USB_DOEPDMA (ep_num) = (uint32) buffer;
        }

        doeptsiz_ptr->mBits.transfer_size = MAX_RECV_LENGTH;    // transfer size
        doeptsiz_ptr->mBits.packet_count = 64;
        * (volatile uint32 *) USB_DOEPCTL (ep_num) |= (unsigned int) BIT_26; // clear nak
        * (volatile uint32 *) USB_DOEPCTL (ep_num) |= (unsigned int) BIT_31; // endpoint enable
    }
    else
    {
        volatile USB_DIEPTSIZ_U *dieptsiz_ptr = (USB_DIEPTSIZ_U *) USB_DIEPTSIZ (ep_num);
        volatile USB_DIEPCTL_U   *diepctl_ptr = (USB_DIEPCTL_U *) USB_DIEPCTL (ep_num);

        if (is_dma)
        {
            * (volatile uint32 *) USB_DIEPDMA (ep_num) = (uint32) buffer;
        } 

        dieptsiz_ptr->mBits.transfer_size = transfer_size;                  // transfer size
        packet_count = (transfer_size+diepctl_ptr->mBits.mps-1) /diepctl_ptr->mBits.mps;
        dieptsiz_ptr->mBits.packet_count = packet_count;                    // packet count
        diepctl_ptr->mBits.tx_fifo_number = ep_num;                         // tx fifo number

        * (volatile uint32 *) USB_DIEPCTL (ep_num) |= (unsigned int) BIT_26;            // clear nak
        * (volatile uint32 *) USB_DIEPCTL (ep_num) |= (unsigned int) BIT_31;            // endpoint enable
    }

}

/*****************************************************************************/
//  Description:   process desecriptor request.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void usb_get_descriptor (USB_REQUEST_1_U *request1, USB_REQUEST_2_U *request2)
{
    uint32 length = 0;
    uint8   *send_data=NULL;


    length = (uint32) request2->mBits.length_l;

    switch (request1->mBits.value_m)
    {

        case USB_DEVICE_DESCRIPTOR_TYPE:

            send_data = (uint8 *) UCOM_Get_DevDesc();//lint !e718
            EPI0_config (0x12, 0x1, TRUE, (uint32 *) send_data);

            break;

        case USB_CONFIGURATION_DESCRIPTOR_TYPE:

            send_data = (uint8 *) UCOM_Get_CfgDesc();//lint !e718

            if (0xff == length)
            {
                EPI0_config (0x20, 0x1, TRUE, (uint32 *) send_data);
            }
            else
            {
                EPI0_config (length, 0x1, TRUE, (uint32 *) send_data);
            }

            break;

        default:
            break;
    }
}
/*****************************************************************************/
//  Description:   process setup transaction.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/

LOCAL void usb_setup_handle (void)
{
    uint32  vendor_ack = 0;
    USB_REQUEST_1_U   *request1;
    USB_REQUEST_2_U   *request2;

    request1 = (USB_REQUEST_1_U *) (unsigned int) (&s_setup_packet[0]);
    request2 = (USB_REQUEST_2_U *) (unsigned int) (&s_setup_packet[1]);

    switch (request1->mBits.type)
    {

        case USB_REQ_STANDARD://standard

            switch (request1->mBits.brequest)
            {
                case USB_REQUEST_SET_ADDRESS:
                    {
                        volatile USB_DCFG_U *dcfg_ptr = (USB_DCFG_U *) USB_DCFG;

                        dcfg_ptr->mBits.devaddr = request1->mBits.value_l;
                        EPI0_config (0, 1, FALSE, NULL);
                    }
                    break;
                case USB_REQUEST_GET_DESCRIPTOR:

                    usb_get_descriptor (request1, request2);

                    break;
                case USB_REQUEST_SET_CONFIGURATION:
                    EPI0_config (0, 1, FALSE, NULL);
                    break;
                default:
                    EPI0_config (0, 1, TRUE, &vendor_ack);
                    break;
            }

            break;

        case USB_REQ_CLASS://class

            switch (request1->mBits.recipient)
            {
                case USB_REC_INTERFACE:

                    if ( (0x01 == request1->mBits.value_l)
                            && (0x06 == request1->mBits.value_m))
                    {

                        usb_EPActive (USB_EP5, USB_EP_DIR_IN);
                        usb_EPActive (USB_EP6, USB_EP_DIR_OUT);
                    }

                    EPI0_config (0, 1, FALSE, NULL);
                    break;
                default:
                    EPI0_config (0, 1, TRUE, NULL);
                    break;
            }

            break;
        case USB_REQ_VENDOR:
            EPI0_config (4, 1, TRUE, &vendor_ack);
            break;
        default:
            EPI0_config (0, 1, TRUE, NULL);
            break;
    }

}
/*****************************************************************************/
//  Description:   usb reset interrupt handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/

LOCAL void usb_reset_handler (void)
{

    uint32  timeout = 0;

    //SCI_TraceLow("enter usb reset.\r\n");

    * (volatile uint32 *) USB_GINTMSK &= (unsigned int) (~BIT_12);                          // disable reset interrupt

    * (volatile uint32 *) USB_DOEP0CTL |= (unsigned int) BIT_27;                            // set NAK for all OUT endpoint

    * (volatile uint32 *) USB_DOEPCTL (6) |= (unsigned int) BIT_27;

    * (volatile uint32 *) USB_DAINTMSK |= (unsigned int) (BIT_0|BIT_16);
    * (volatile uint32 *) USB_DOEPMSK |= (unsigned int) (BIT_0|BIT_3|BIT_2|BIT_1);
    * (volatile uint32 *) USB_DIEPMSK |= (unsigned int) (BIT_0|BIT_3|BIT_1|BIT_2|BIT_5);//lint !e718

    * (volatile uint32 *) USB_GRXFSIZ = (unsigned int) (BIT_2|BIT_4|BIT_9);                 //RX fifo ,276 Dword,start address is 0
    * (volatile uint32 *) USB_GNPTXFSIZ = (unsigned int) ( (BIT_2|BIT_4|BIT_9) |BIT_21);    //EP0 TX fifo, 32 Dword, start address is 0+276=276
    * (volatile uint32 *) USB_DIEPTXF (5) = (unsigned int) ( (BIT_2|BIT_4|BIT_5|BIT_9) |BIT_23); //lint !e718//EP5 TX fifo, 64 Dword, start address is 276+32 = 308

    * (volatile uint32 *) USB_GRSTCTL |= (unsigned int) BIT_5;                          //reflush tx fifo

    while ( (* (volatile uint32 *) USB_GRSTCTL) & ( (unsigned int) BIT_5))
    {
        timeout++;

        if (timeout >= USB_TIMEOUT)
        {
            break;
        }
    }

    timeout = 0;
    * (volatile uint32 *) USB_GRSTCTL |= (unsigned int) BIT_4;                          //reflush rx fifo

    while ( (* (volatile uint32 *) USB_GRSTCTL) & ( (unsigned int) BIT_4))
    {
        timeout++;

        if (timeout >= USB_TIMEOUT)
        {
            break;
        }
    }

    EPO0_config (TRUE, s_setup_packet);

    * (volatile uint32 *) USB_GINTMSK |= (unsigned int) BIT_12;                             // enable reset interrupt

    * (volatile uint32 *) USB_GINTSTS |= (unsigned int) BIT_12;                             //clear reset interrupt

}

/*****************************************************************************/
//  Description:   usb enumeration done handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/

void usb_enumeration_done (void)
{
    volatile USB_DSTS_U *dsts_ptr = (USB_DSTS_U *) USB_DSTS;

    enum_speed = dsts_ptr->mBits.enumspd;                                   //read enumration speed
    //SCI_TraceLow("usb_enumeration_done: device_speed = %d\r\n",enum_speed);

    * (volatile uint32 *) USB_DIEP0CTL &= (unsigned int) (~ (BIT_0|BIT_1));

    EPn_config (USB_EP5, USB_EP_TYPE_BULK, USB_EP_DIR_IN, USB_PACKET_64);
    EPn_config (USB_EP6, USB_EP_TYPE_BULK, USB_EP_DIR_OUT, USB_PACKET_64);

    EPO0_config (TRUE, s_setup_packet);

    * (volatile uint32 *) USB_DCTL |= (unsigned int) BIT_8;

    * (volatile uint32 *) USB_GINTSTS |= (unsigned int) BIT_13;

}
/*****************************************************************************/
//  Description:    out endpoint6 handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
static int currentDmaBufferIndex = 0;
LOCAL void usb_EP6_handle (void)
{
    volatile USB_DOEPINT_U *doepint_ptr = (USB_DOEPINT_U *) USB_DOEPINT (USB_EP6);
    volatile USB_DOEPINT_U  doepint;
    volatile USB_DOEPTSIZ_U *doeptsiz_ptr = (USB_DOEPTSIZ_U *) USB_DOEPTSIZ (USB_EP6);
    volatile uint32  mask;

    mask = * (volatile uint32 *) USB_DOEPMSK;
    doepint.dwValue = doepint_ptr->dwValue;

    doepint.dwValue &= (unsigned int) mask;

    if (doepint.mBits.transfer_com)
    {
        doepint_ptr->mBits.transfer_com = 1;
        recv_length = MAX_RECV_LENGTH - doeptsiz_ptr->mBits.transfer_size;

        * (volatile uint32 *) USB_DOEPMSK |= (unsigned int) BIT_13;
        * (volatile uint32 *) USB_DOEPMSK |= (unsigned int) BIT_4;
        * (volatile uint32 *) USB_DOEPMSK &= (unsigned int) (~BIT_0);
#ifdef FDL2_MODULE
        Dcache_InvalRegion(usb_out_endpoint_buf[currentDmaBufferIndex],  MAX_RECV_LENGTH);
#endif
		
    }
    else if (doepint.mBits.nak)
    {
        usb_start_transfer (USB_EP6, USB_EP_DIR_OUT, 1, TRUE, (uint32 *) usb_out_endpoint_buf[currentDmaBufferIndex]);
        * (volatile uint32 *) USB_DOEPMSK &= (unsigned int) (~BIT_13);
        * (volatile uint32 *) USB_DOEPMSK |= (unsigned int) BIT_0;
        doepint_ptr->mBits.nak = 0x1;

    }
    else if (doepint.mBits.bbleerr)
    {
        doepint_ptr->mBits.bbleerr = 0x01;
    }
}
/*****************************************************************************/
//  Description:   out endpoint0 handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/

LOCAL void usb_EP0_out_handle (void)
{
    volatile USB_DOEPINT_U *doepint_ptr = (USB_DOEPINT_U *) USB_DOEPINT (0);

    if (doepint_ptr->mBits.timeout_condi)
    {
        usb_setup_handle();
    }

    if (doepint_ptr->mBits.transfer_com)
    {
        * (volatile uint32 *) USB_DOEP0CTL |= (unsigned int) BIT_27;
        doepint_ptr->mBits.transfer_com = 0x1;
    }

    if (doepint_ptr->mBits.outtokenfifoemp)
    {
        doepint_ptr->mBits.outtokenfifoemp = 0x1;
    }

    doepint_ptr->dwValue = 0xffffffff;// clear all interrupt

    EPO0_config (TRUE, s_setup_packet); //renable ep0 nd set packet count
}

/*****************************************************************************/
//  Description:   out endpoint handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void usb_EP_out_handle (void)
{
    volatile USB_DAINT_U *daint_ptr = (USB_DAINT_U *) USB_DAINT;
    USB_DAINT_U daint;

    daint.dwValue = daint_ptr->dwValue;         // disable EP out interrupt
    * (volatile uint32 *) USB_GINTMSK &= (unsigned int) (~BIT_19);

    if (daint.mBits.outepint_0)
    {
        usb_EP0_out_handle();
    }

    if (daint.mBits.outepint_6)
    {
        usb_EP6_handle();
    }

    * (volatile uint32 *) USB_GINTMSK |= (unsigned int) BIT_19; // enable reset interrupt
}
/*****************************************************************************/
//  Description:   in endpoint handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void usb_EP_in_handle (void)
{
    volatile USB_DAINT_U *daint_ptr = (USB_DAINT_U *) USB_DAINT;
    USB_DAINT_U daint;


    * (volatile uint32 *) USB_GINTMSK &= (unsigned int) (~BIT_18); // disable EP in interrupt
    daint.dwValue = daint_ptr->dwValue;

    if (daint.mBits.inepint_0)
    {
        volatile USB_DIEPINT_U *diepint_ptr = (USB_DIEPINT_U *) USB_DIEPINT (0);

        diepint_ptr->dwValue = 0xFFFFFFFF;
    }

    if (daint.mBits.inepint_5)
    {
        volatile USB_DIEPINT_U *diepint_ptr = (USB_DIEPINT_U *) USB_DIEPINT (5);

        diepint_ptr->dwValue = 0xFFFFFFFF;
    }

    * (volatile uint32 *) USB_GINTMSK |= (unsigned int) BIT_18; // enable EP in interrupt
}

/*****************************************************************************/
//  Description:   usb interrupt handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
static __inline void usb_handler (void)
{
    volatile USB_INTSTS_U *usb_int_ptr = (USB_INTSTS_U *) USB_GINTSTS;
    volatile USB_INTSTS_U  usb_int;

    usb_int.dwValue = usb_int_ptr->dwValue;

    // in endpoint interrupt
    if (usb_int.mBits.iepint)
    {
        usb_EP_in_handle();
    }

    // out endpoint interrupt
    if (usb_int.mBits.oepint)
    {
        usb_EP_out_handle();
    }

    // enumeration done interrupt
    if (usb_int.mBits.enumdone)
    {
        usb_enumeration_done();
    }

    // reset interrupt
    if (usb_int.mBits.usbrst)
    {
        usb_reset_handler();
    }
}
/*****************************************************************************/
//  Description:   configure in endpoint ep_id to send message.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/

PUBLIC int USB_EPxSendData (char ep_id ,unsigned int *pBuf,int len)
{
    usb_start_transfer ( (USB_EP_NUM_E) ep_id, USB_EP_DIR_IN, len, TRUE, (uint32 *) pBuf);
    return 0;
}

/*****************************************************************************/
//  Description:   initialize the usb core.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
PUBLIC void usb_core_init (void)
{
    readIndex = 0;
    recv_length = 0;
    return;
}


char VCOM_GetChar (void)
{

    if (readIndex == recv_length)
    {	  
        readIndex = 0;
        recv_length = 0;
REGET:
        usb_handler();
		
        if (recv_length > 0)
        {
            nIndex = currentDmaBufferIndex;
            currentDmaBufferIndex ^= 0x1;
        }
        else
        {
            goto REGET;
        }
        
    }
     return usb_out_endpoint_buf[nIndex][readIndex++];
}

int VCOM_GetSingleChar (void)
{ 
    if (readIndex == recv_length)
    {
        readIndex = 0;
        recv_length = 0;
	
        usb_handler();

        if (recv_length > 0)
        {
            nIndex = currentDmaBufferIndex;
	     currentDmaBufferIndex ^= 0x1;
        }
        else
        {
            return -1;
        }
    
    }
        
    return (int)usb_out_endpoint_buf[nIndex][readIndex++];
}



