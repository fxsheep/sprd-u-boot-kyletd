/******************************************************************************
 ** File Name:      com_drvapi.h                                                 *
 ** Author:         weihua.wang                                               *
 ** DATE:           2006-4-24                                                 *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the common macro definiton and structure*
 **                      for uart drv and usb virtual com                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 25/04/2006     weihua.wang      creat                                     *
 ** 02/09/2008     Tao.Zhou         driver arrangement                        *
 ******************************************************************************/
#ifndef _COM_DRVAPI_H_
#define _COM_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro definition                                  *
 **---------------------------------------------------------------------------*/
enum{
    UART_COM0 = 0x0,
    UART_COM1 = 0x1,
    UART_COM2 = 0x2,
    UART_COM3 = 0x3
};

#define UART_COM0               (0)
#define UART_COM1               (1)
#define UART_COM2               (2)
#define UART_COM3               (3)


#define USB_COM0                (0)
#define USB_COM1                (1)
#define USB_MAX_NUM             (2)

#define CARD_COM0               (0)
#define CARD_COM1               (1)
#define CARDCOM_MAX_NUM         (2)

/* add for DUN COM @eddie.li */
#define DUN_COM                 (0)
#define DUN_MAX_NUM             (1)
/* add for DUN COM @eddie.li end */

//return sts
#define UART_SUCCESS            (0)
#define UART_FAILURE            (1)
#define UART_PORT_NUM_ERR       (2)
#define UART_PORT_REOPEN_ERR    (3)
#define UART_PORT_RECLOSE_ERR   (4)
#define UART_CALLBACK_ERR       (5)

//event type
#define EVENT_DATA_TO_READ      (0)
#define EVENT_WRITE_COMPLETE    (1)
#define EVENT_INIT_COMPLETE     (2)
#define EVENT_SHUTDOWN_COMPLETE (3)
#define COM_MAX_EVENT           (4)

//Baud Rate. (CLK = 12.998 MHz)
#define BAUD_1200               0x2A50
#define BAUD_2400               0x1528
#define BAUD_4800               0x0A94
#define BAUD_9600               0x054A
#define BAUD_19200              0x02A5
#define BAUD_38400              0x0152
#define BAUD_57600              0x00E2
#define BAUD_115200             0x0071
#define BAUD_230400             0x0038
#define BAUD_460800             0x001C
#define BAUD_921600             0x000E

//#define UART_GET_MUTEX_WAIT_TIME  500  //wait for 500ms at most

/*
    Uart software settings
*/
#define TX_WATER_MARK      8

// Set uart register directly, for debug purpose, not commend
#define UART0_QUCIKCFG(ien, ctl0, ctl1, ctl2, ckd0, ckd1)\
    {\
        *(volatile uint32 *)(ARM_UART0_BASE + ARM_UART_IEN)   = (ien);\
        *(volatile uint32 *)(ARM_UART0_BASE + ARM_UART_CTL0)  = (ctl0);\
        *(volatile uint32 *)(ARM_UART0_BASE + ARM_UART_CTL1)  = (ctl1);\
        *(volatile uint32 *)(ARM_UART0_BASE + ARM_UART_CTL2)  = (ctl2);\
        *(volatile uint32 *)(ARM_UART0_BASE + ARM_UART_CLKD0) = (ckd0);\
        *(volatile uint32 *)(ARM_UART0_BASE + ARM_UART_CLKD1) = (ckd1);\
    }

#define UART1_QUCIKCFG(ien, ctl0, ctl1, ctl2, ckd0, ckd1)\
    {\
        *(volatile uint32 *)(ARM_UART1_BASE + ARM_UART_IEN)   = (ien);\
        *(volatile uint32 *)(ARM_UART1_BASE + ARM_UART_CTL0)  = (ctl0);\
        *(volatile uint32 *)(ARM_UART1_BASE + ARM_UART_CTL1)  = (ctl1);\
        *(volatile uint32 *)(ARM_UART1_BASE + ARM_UART_CTL2)  = (ctl2);\
        *(volatile uint32 *)(ARM_UART1_BASE + ARM_UART_CLKD0) = (ckd0);\
        *(volatile uint32 *)(ARM_UART1_BASE + ARM_UART_CLKD1) = (ckd1);\
    }

#define UART2_QUCIKCFG(ien, ctl0, ctl1, ctl2, ckd0, ckd1)\
    {\
        *(volatile uint32 *)(ARM_UART2_BASE + ARM_UART_IEN)   = (ien);\
        *(volatile uint32 *)(ARM_UART2_BASE + ARM_UART_CTL0)  = (ctl0);\
        *(volatile uint32 *)(ARM_UART2_BASE + ARM_UART_CTL1)  = (ctl1);\
        *(volatile uint32 *)(ARM_UART2_BASE + ARM_UART_CTL2)  = (ctl2);\
        *(volatile uint32 *)(ARM_UART2_BASE + ARM_UART_CLKD0) = (ckd0);\
        *(volatile uint32 *)(ARM_UART2_BASE + ARM_UART_CLKD1) = (ckd1);\
    }

#define UART3_QUCIKCFG(ien, ctl0, ctl1, ctl2, ckd0, ckd1)\
    {\
        *(volatile uint32 *)(ARM_UART3_BASE + ARM_UART_IEN)   = (ien);\
        *(volatile uint32 *)(ARM_UART3_BASE + ARM_UART_CTL0)  = (ctl0);\
        *(volatile uint32 *)(ARM_UART3_BASE + ARM_UART_CTL1)  = (ctl1);\
        *(volatile uint32 *)(ARM_UART3_BASE + ARM_UART_CTL2)  = (ctl2);\
        *(volatile uint32 *)(ARM_UART3_BASE + ARM_UART_CLKD0) = (ckd0);\
        *(volatile uint32 *)(ARM_UART3_BASE + ARM_UART_CLKD1) = (ckd1);\
    }
#if 0
#define UART_INIT_UART_LOWLEVEL(reg_base, baudrate) {\
        *(volatile uint32 *)((reg_base) + ARM_UART_IEN)   = 0x00;\
        *(volatile uint32 *)((reg_base)+ ARM_UART_CTL0)   = 0x1C;\
        *(volatile uint32 *)((reg_base) + ARM_UART_CTL1)  = 0x00;\
        *(volatile uint32 *)((reg_base) + ARM_UART_CLKD0) = \
                (CHIP_GetAPBClk() + (baudrate / 2)) / (baudrate);\
        *(volatile uint32 *)((reg_base) + ARM_UART_CLKD1) = 0x00;\
    }
#endif
typedef enum
{
    FIVE_BITS = 0,
    SIX_BITS,
    SEVEN_BITS,
    EIGHT_BITS
} UART_BYTE_SIZE_E;

typedef enum
{
    EVEN_PARITY = 0,
    ODD_PARITY
} UART_PARITY_E;

typedef enum
{
    PARITY_DISABLE = 0,
    PARITY_ENABLE
} UART_PARITY_SET_E;

typedef enum
{
    UNUSED = 0,
    ONE_STOP_BIT,
    ONE_AND_HALF_BIT,
    TWO_STOP_BIT
} UART_STOP_BIT_E;

typedef enum
{
    NO_FLOW_CONTROL = 0,
    HW_FLOW_CONTROL
} UART_FLOW_CTR_E;

/* uart deepsleep wakeup control */
typedef enum
{
    DS_WAKEUP_DISABLE = 0,
    DS_WAKEUP_ENABLE
} UART_DS_CTRL_E;

/* uart deepsleep wakeup conditon */
typedef enum
{
    DS_WAKEUP_TYPE_START = 0,
    DS_WAKEUP_BY_RXD     ,
    DS_WAKEUP_BY_DSR     ,
    DS_WAKEUP_BY_RXD_DSR ,
    DS_WAKEUP_BY_CTS     ,
    DS_WAKEUP_BY_CTS_RXD ,
    DS_WAKEUP_BY_CTS_DSR ,
    DS_WAKEUP_BY_CTS_DSR_RXD,
    DS_WAKEUP_TYPE_MAX
} UART_DS_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                                                                   *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                                                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Struct definition                                                                                    *
**---------------------------------------------------------------------------*/
typedef void (*UartCallback) (uint32 event);

typedef struct
{
    uint16 tx_watermark; // 0~127B
    uint16 rx_watermark; // 0~127B
    uint32 baud_rate;
    UART_PARITY_E     parity;
    UART_PARITY_SET_E parity_enable;
    UART_BYTE_SIZE_E  byte_size;
    UART_STOP_BIT_E   stop_bits;
    UART_FLOW_CTR_E   flow_control;
    UART_DS_CTRL_E    ds_wakeup_en;
    UART_DS_TYPE_E    ds_wakeup_type;
} UART_INIT_PARA_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:   This function initilizes uart port, configure uart
//                   registers, interrupt service, and register callback
//                   function for notifying the status of uart read and write.
//  Param:         uint32 port_num: usb virtual com num USB_COM0/USB_COM1
//                 UART_INIT_PARA_T* :include param for uart, only useful for uart
//                 UartCallback func :uplayer callback
//  Author:        xingyun.he
//  Note:          NONE
/*****************************************************************************/
PUBLIC uint32 UART_Initilize (uint32 port_num,
                              UART_INIT_PARA_T *para,
                              UartCallback func);

/*****************************************************************************/
//  Description:  This function close uart port
//  Author:       weihua.wang
//  Param :       uint32 port_num: usb virtual com number
//  Note:         NONE
/*****************************************************************************/
PUBLIC uint32 UART_Close (uint32 port_num);

/*****************************************************************************/
// Description :   Use the given baudrate to init the uart register.
//                 and can receivce/send data via it.
// Global resource dependence:  NONE
// Author :        Lin.liu
// Input:
//        uart port num: UART_COM0, UART_COM1
//        baudrate : the baudrate need to be set( 115200, 9600 etc ).
// Return:
//        None
// Note:           NONE
/*****************************************************************************/
PUBLIC void UART_InitUartLowLevel (uint32 port, uint32 baudrate);

/*****************************************************************************/
//  Description:  This function is used to get uart port deepsleep-awaken
//                function status: enable or disable
//  Author:       zhengjiang.lei
//  Param :
//  Note:         NONE
/*****************************************************************************/
PUBLIC BOOLEAN UARTCOM_DslpFuncSts_Get (uint32 port_num);

/*****************************************************************************/
// Description :   check the DSR and set the DTR to enable/disable deep sleep
// Global resource dependence:  NONE
// Author :        Jim.zhang
// Note :          If DSR = 1: Disable deep sleep, DTR = 1
//                 If DSR = 0, Enable deep sleep,  DTR = 0
/*****************************************************************************/
PUBLIC void UART_CheckDeepSleepFlag (void);

/*****************************************************************************/
//  Description:   This function read data from receiving buffer.It is called*
//                 when up layfer receives the event of "EVENT_DATA_TO_READ".*
//  Author:        weihua.wang                                               *
//  Param :        uint32 port_num : uart com number index                   *
//                 uint8* buffer   : des buffer header address               *
//                 uint32 length   : size to be read                         *
//  Return:        if success,return size read in effect                     *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint32 UART_ReadData (uint32 port_num,
                             uint8 *buffer,
                             uint32 length);

/*****************************************************************************/
//  Description:   This function record data which will be written to        *
//                  tx fifo and enable tx fifo empty interrupt. The length   *
//                  of data should be not larger than FIFO_SIZE(127B).       *
//  Author:        weihua.wang                                               *
//  Param :        uint32 port_num : uart com number index                   *
//                 uint8* buffer   : src buffer header address               *
//                 uint32 length   : size to be writed                       *
//  Return:        if success,return size writed in effect                   *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint32 UART_WriteData (uint32 port_num,
                              const uint8 *buffer,
                              uint32 length);

/*****************************************************************************/
//  Description:   This function record data which will be written to        *
//                  tx fifo and enable tx fifo empty interrupt. The length   *
//                  of data should be not larger than FIFO_SIZE     .       *
//  Author:        weihua.wang                                               *
//  Param :        uint32 port_num : uart com number index                   *
//                 uint8* buffer   : src buffer header address               *
//                 uint32 length   : size to be writed                       *
//  Return:        if success,return size writed in effect                   *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint32 UART_WriteDataEx (uint32 port_num,
                                const uint8 *buffer,
                                uint32 length,
                                BOOLEAN irq_en);
/*****************************************************************************/
//  Description:   This function get char from uart receive register.        *
//  Author:        xingyun.he                                                *
//  Return value:  Returns a char from uart receive register.                *
//  Note:          return -1 indicates that no char received from fifo       *
/*****************************************************************************/
PUBLIC int32 UART_DumpGetChar (uint32 phy_port);

/*****************************************************************************/
//  Description:   This function put a char to the uart send register.       *
//  Author:        xingyun.he                                                *
//  Return value:  put a char from uart receive register.                    *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_DumpPutChar (uint32 phy_port, uint8 nchar);

/*****************************************************************************/
//  Description:   This function read data in sync mode.                     *
//  Author:        Tao.Zhou                                                  *
//  Return value:  Returns a char from uart receive register.                *
//  Note:          return -1 indicates that no char received from fifo       *
/*****************************************************************************/
PUBLIC uint32 UART_ReadDataSync (uint32 port_num, uint8 *buffer, uint32 length);

/*****************************************************************************/
//  Description:   This function write data in sync mode.                    *
//  Author:        Tao.Zhou                                                  *
//  Return value:  put a char from uart receive register.                    *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_WriteDataSync (uint32 port_num, const uint8 *buffer, uint32 length);


/*****************************************************************************/
//  Description:   This function set uart speed                              *
//  Author:        xingyun.he                                                *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_SetBaudSpeed (uint32 port_num, uint32 speed);


/*****************************************************************************/
//  Description:   This function set uart parity parameter                   *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_SetParity (uint32 port_num, UART_PARITY_E parity);


/*****************************************************************************/
//  Description:   uart parity enable/disable                                *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_ParityEnable (uint32 port_num, UART_PARITY_SET_E is_enable);


/*****************************************************************************/
//  Description:   This function set uart byte size                          *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_SetByteSize (uint32 port_num, UART_BYTE_SIZE_E byte_size);

/*****************************************************************************/
//  Description:   This function set uart stop bits                          *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_SetStopBits (uint32 port_num, UART_STOP_BIT_E stop_bits);


/*****************************************************************************/
//  Description:   This function set uart tx water mark                      *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_SetTxWaterMark (uint32 port_num, uint16 tx_watermark);

/*****************************************************************************/
//  Description:   This function set uart rx water mark                      *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_SetRxWaterMark (uint32 port_num, uint16 rx_watermark);

/*****************************************************************************/
//  Description:    return the available size in uart fifo
//  Global resource dependence:  NONE
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//  Note:           NONE
/*****************************************************************************/
PUBLIC uint32 UART_GetTxFifoCnt (uint32 phy_port);

/*****************************************************************************/
//  Description:    return the Tx fifo is busy or idle
//  Global resource dependence:  NONE
//  Author  :       Jeff.Li
//  Param   :       uint32 port_num: usb virtual com number
//  Return  :       1 - busy, 0 - idle
//  Note:           This interface is just for 8801H platform currently
/*****************************************************************************/
PUBLIC BOOLEAN UART_IsTxFifoBusy (uint32 phy_port);

//@David.Jia 2007.7.4  cr82866 begin
/*****************************************************************************/
//  Description:    return the received bytes in uart fifo
//  Global resource dependence:  NONE
//  Author:         David.Jia
//  Param   :       uint32 port_num: com number
//  Note:           NONE
/*****************************************************************************/
PUBLIC uint32 UART_GetRxFifoCnt (uint32 phy_port);
//@David.Jia 2007.7.4  cr82866 end

/*****************************************************************************/
//  Description:   uart hardware flow control enable/disable                 *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_FlowCtlEnable (uint32 port_num, UART_FLOW_CTR_E is_enable);

/*****************************************************************************/
//  Description:   uart fifo control by DAM enable/disable                   *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_DmaModeEnable (uint32 port_num, BOOLEAN is_enable);

/*****************************************************************************/
//  Description:    uart tx empty enable/disable
//  Global resource dependence:  NONE
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//                  BOOLEAN is_enable: TRUE/FALSE
//  Note:           NONE
/*****************************************************************************/
PUBLIC void UART_Tx_Int_Enable (uint32 phy_port, BOOLEAN is_enable);

/*****************************************************************************/
//  Description:    uart rx full enable/disable
//  Global resource dependence:  NONE
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//                  BOOLEAN is_enable: TRUE/FALSE
//  Note:           NONE
/*****************************************************************************/
PUBLIC void UART_Rx_Int_Enable (uint32 phy_port, BOOLEAN is_enable);

/*****************************************************************************/
//  Description:   enable/disable self loop mode                             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_SelfLoopEnable (uint32 phy_port, BOOLEAN is_enable);

/*****************************************************************************/
//  Description:   uart callback function register                           *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_RegCallBack (uint32 port_num, UartCallback func);

/*****************************************************************************/
//  Description:   uart callback function unregister                         *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_UnRegCallBack (uint32 port_num);

/*****************************************************************************/
//  Description:   uart dma mode writing                                     *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_DmaWriteData (uint32 port_num, const uint8 *buffer, uint32 length);
//PUBLIC uint32 UART_WriteData(uint32 port_num, const uint8* buffer, uint32 length);

/*****************************************************************************/
//  Description:   get the max number of UART
//  Global resource dependence : NONE
//  Author:        hanjun.liu
//  Note:          NONE
/*****************************************************************************/
PUBLIC uint32 UART_MaxPortNum (void);

/*****************************************************************************/
//  Description:   get the  of UART
//  Global resource dependence : NONE
//  Author:        hanjun.liu
//  Note:          NONE
/*****************************************************************************/
PUBLIC uint32 UART_RxWaterMark (void);

PUBLIC void DSPLogUARTSwitchOn (void);
PUBLIC void DSPLogUARTSwitchOff (void);

uint32 GetUART0BaseAddr (void);

uint32 getRMUartBaseAddr (void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //¢_COM_DRVAPI_H_