/******************************************************************************
 ** File Name:      sio.h                                                     *
 ** Author:         Richard Yang                                              *
 ** DATE:           04/02/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This driver is for development board, Integrator/AP.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Richard.Yang     Create.                                   *
 ** 05/13/2002     Richard.Yang     Add sio_send_packet, sio_send_log_packet  *
 ** 05/30/2003     Eric.zhou        Add AT through channel server associated  *
 ** functions.                                                                    *
 ** 08/25/2003     Zhemin.Lin     Add MIX_MODE for COM_USER port, according MS00004213 *
 ******************************************************************************/

#ifndef _SIO_H_
#define _SIO_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "com_drvapi.h"
#include "chip.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#define SIO_PPP_SUCCESS 0
#define SIO_PPP_FAIL    -1

#define SIO_RX_FLOW_CONTROL 0x1
#define SIO_TX_FLOW_CONTROL 0x2


/*
    Debug port communications protocol flag.
*/
#define ESCAPE_BYTE       0x7D
#define FLAG_BYTE         0x7E
#define COMPLEMENT_BYTE   0x20

/*
    Uart/USB port definitions
*/

#define COM0             (0)
#define COM1             (1)
#define COM2             (2)
#define COM3             (3)
#define VIR_COM0         (4)
#define VIR_COM1         (5)
#define DSK_COM0         (6)
#define DSK_COM1         (7)
/* added @eddie.li */
#define VIR_COM2         (8)
#define MAX_SIO_PORT_NUM (9)
/* added @eddie.li end */

#define MAX_LOGICAL_SIO_PORT_NUM  2             //'COM_DATA', 'COM_DEBUG'
#define SIO_ALREADY_OPEN  0x55

#define LOG_FRM_MED       0
#define LOG_FRM_NONE      1
#define LOG_FRM_END       2

#define ATC_MODE          0x01
#define DATA_MODE         0x02
#define MIX_MODE    0x03

#define COM_USER          0
#define COM_DATA          0
#define COM_DEBUG         1


// @Tao.Zhou, CR:MS00057325, 09/29/2006
/* We must send at least(FIFO_SIZE - FIFO_EMPTY - fifo_cnt) bytes during one interrupt */
#define FIFO_EMPTY        6

#define GET_BUF_FREE_SIZE(free_size, _buf)  \
    if (_buf->start_point >= _buf->end_point) \
    {\
        free_size = (int) ((int)_buf->size - _buf->start_point + _buf->end_point - 1);\
    }\
    else\
    {\
        free_size = (int) (_buf->end_point - _buf->start_point - 1);\
    }

#define GET_BUF_DATA_SIZE(data_size, _buf)  \
    if (_buf->start_point >= _buf->end_point) \
    {\
        data_size = (int) (_buf->start_point - _buf->end_point);\
    }\
    else\
    {\
        data_size = (int) ((int)(_buf->size) + _buf->start_point - _buf->end_point);\
    }

#if defined(PLATFORM_SC8800H) || defined(PLATFORM_SC8800G)  || defined(CONFIG_SC8810)//modified by tyler.song for compiling error.
#define ATC_REC_BUF_SIZE           2600
#else
#define ATC_REC_BUF_SIZE           1600
#endif


// SIO definition

typedef void *HSIO;
typedef struct
{
    uint32  baud_rate;
    uint8   parity;
    uint8   stop_bits;
    uint8   byte_size;
    uint8   flow_control;
} SIO_CONTROL_S;

typedef struct
{
    uint32  cbInQue;
    uint32  cbOutQue;
} UARTSTAT_S, *UARTSTAT_PTR_T;

typedef void (* ATC_CALLBACK) (uint32);



/*
    Software structures
*/
typedef struct
{
    uint32  size;              // Total Size of the buffer.
    volatile int  start_point;       // First data in the buffer.
    volatile int  end_point;         // Last data in the buffer.
    volatile uint32  status;            // Indicate the buffer status.
    uint32  lost_num;          //
    uint8   *sio_buf_ptr;      // Sio buffer address.
} SIO_BUF_S;

typedef struct 
{
    uint32 (*init) (uint32 port_num, UART_INIT_PARA_T * ,
                    UartCallback callback);                /* drv com creat   */

    uint32 (*close) (uint32 port_num);                           /* drv com close   */

    uint32 (*get_fifocnt) (uint32 phy_port);                     /* get tx fifo cnt */

    uint32 (*read) (uint32 port_num,uint8 *buffer,uint32 length); /* read data from drv */

    uint32 (*write) (uint32 port_num,const uint8 *buffer,uint32 length); /*send data to drv*/

    int32 (*get_char) (uint32 phy_port);                         /* read single char*/

    void (*put_char) (uint32 phy_port, uint8 nchar);             /* send signle char*/

    void (*tx_enable) (uint32 phy_port,BOOLEAN is_enable);       /* drv send enable */

    void (*rx_enable) (uint32 phy_port,BOOLEAN is_enable);       /* drv receive enable*/

    void (*flush) (uint32 phy_port);                             /* only useful for usb virtual com     */
    /* flush data from local buffer to fifo*/
} sio_operation_st;

typedef struct
{
    uint32 sio_port_index;
    uint32 phy_port_index;
} SIO_PORT_CVT_T;

typedef struct
{
    uint32         open_flag;
    uint32         phy_port;
    SIO_CONTROL_S  dcb;
    SIO_BUF_S      tx_buf;
    SIO_BUF_S      rx_buf;
    SIO_BUF_S      atc_buf;
    volatile unsigned       mdm_stat;   /* modem status at last change */
    volatile uint32         last_tx;    // Last time for tx
    struct   com_line      *p_line;     /* ptr to comline driver structure
                                           no use for debug port, just for user port */
	sio_operation_st sio_op;  /* sio operation for COM_DEBUG/COM_DATA */


} SIO_INFO_S;

/*
    Functions definitions
*/
/* Create a device control block.*/
PUBLIC HSIO SIO_Create (uint32        port,
                        uint32        phy_port,
                        SIO_CONTROL_S *dcb
                       );
PUBLIC uint32 SIO_SetBaudRate (uint32 port, uint32 baud_rate);
PUBLIC uint32 SIO_GetBaudRate (uint32 port);
PUBLIC void SIO_SetATCBuffer (uint8 *src,uint16 length);
PUBLIC uint32 SIO_ChangeMode (uint32 mode);
PUBLIC uint32 SIO_GetMode (void);
PUBLIC  void  SIO_Close (uint32  port);
PUBLIC  void  SIO_BuildDCB (
    uint32         port,
    SIO_CONTROL_S  *dcb
);

PUBLIC SIO_INFO_S *SIO_GetPPPPort (void);
PUBLIC  int   SIO_ReadPPPFrame (
    uint32 port,  // Port number COM_USER, COM_DEBUG
    uint8  *src,            // Data to receive
    int    size,            // MAX size to receive
    uint32 *dest_len        // Actual size received
);

PUBLIC  int   SIO_WritePPPFrame (
    uint32  port,
    uint8   *src,
    int     size
);

PUBLIC  int   SIO_FastWritePPPFrame (
    uint32  port,
    uint8   *src,
    int     size
);

PUBLIC  int   SIO_GetChar (
    uint32 port
);

PUBLIC  int   SIO_PutChar (
    uint32  port,
    uint8   nchar
);

/*****************************************************************************/
// Description :    this function changes userport from one mode to another,
//                       following mode transition can be done by this funcition:
//                       MIX_MODE-->ATC_MODE
//                       MIX_MODE-->DATA_MODE
//                       ATC_MODE-->MIX_MODE
//                       ATC_MODE-->DATA_MODE
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC  uint32 SIO_SetUserPortMode (
    uint32 mode
);
PUBLIC  uint32 SIO_ClearError (
    uint32          port,
    uint32          *error_ptr,
    UARTSTAT_PTR_T  *uart_stat_ptr
);

PUBLIC  int    SIO_SendLogPacket (
    uint32  *head_ptr,
    int     head_len,
    uint32  *body_ptr,
    int     body_len
);

PUBLIC  int    SIO_SendPacket (
    void  *src,
    int   size
);

PUBLIC void SIO_ATC_SetMixMode (
    BOOLEAN mix_enable
);

PUBLIC int SIO_SendFrame (
    void *src,
    int size
);

PUBLIC uint16 SIO_GetFreePPPFrameCount (void);

PUBLIC int     SIO_DumpGetChar (
    uint32 port
);
PUBLIC void    SIO_DumpPutChar (
    uint32 port,
    char   nchar
);

PUBLIC void SIO_DumpPutCharEnd (
    uint32 port
);

PUBLIC uint32  SIO_WaitEvent (
    uint32  port,
    uint32  *event_mask
);

PUBLIC void SIO_ResetBuf (
    uint32 port
);

PUBLIC void SIO_ATC_ReadCmdLine (
    uint8  *data_ptr,       // Output argument, point to the buf which is
    // alloced by caller
    uint   length,          // The length of cmd line which should be read from
    // sio
    uint   *read_length_ptr // The actual read length
);

PUBLIC void SIO_ATC_WriteCmdRes (
    uint8  *res_data_ptr,  // Point to the response infomation buffer
    uint   length          // The length of response infomation buffer
);

 PUBLIC void SIO_ATC_ClrCmdLine (
    void
);
 
void SIO_ATC_SetCmdLineTerminateChar (
    uint8 terminate_char1,   // Default terminate char is '\n'
    uint8 terminate_char2    // Default terminate char is '\0'
);
PUBLIC void SIO_ATC_SetCmdLineBackSpaceChar (
    uint8  back_space_char  // Default backspace char value is 8
);

PUBLIC void SIO_ATC_SetDataMode (BOOLEAN mode);
PUBLIC void SIO_ATC_SetEcho (BOOLEAN IsEcho);

PUBLIC void SIO_ATC_SetCallback (
    ATC_CALLBACK callback
);

PUBLIC void SIO_BuildDCB (
    uint32         port,
    SIO_CONTROL_S  *dcb
);
/*sio variable extern*/
#define            MAX_PLUS_NUMBER   3
#define            MODE_CHANGE_DELAY 1000 // 1000 milliseconds
#define            BACKSPACE_CHAR    0x08
#define            END_CHAR          0x0D
#define            PLUS_CHAR         '+'


#ifdef _U0_FLOW_CONTROL
#define     SPACE_CHECK_DELAY       300     // time delay of space check of Rx buffer for COM_DATA
#define     EMPTY_SPACE_WATER_MARK  258     // empty sapce water mark of rx buffer 
#endif

#define     XOFF    0x13    // Xoff of Software flow control.
#define     XON     0x11    // Xon of Software flow control.
#if 0
#define DO_ATC_CALLBACK(len) \
    if ( NULL == AT_callback_ind) \
    {\
        DefaultAtHandle(len);\
    }\
    else \
    {\
        (*AT_callback_ind)(len);\
    }
#endif
//@Zhemin.Lin, 08/15/2003
/*****************************************************************************/
// Description :    Update clock divider value due to the change of system frequency
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC void SIO_UpdateDividerValue (void);

//@Zhemin.Lin, CR:MS00004213, 08/22/2003, begin

//SIO communication error type
typedef enum
{
    SIO_PARITY_ERROR,
    SIO_CHECKSUM_ERROR,
    SIO_TIMEOUT_ERROR
} SIO_LINK_ERROR_E;
typedef void (*SIO_ERROR_HANDLE) (uint32 portid, SIO_LINK_ERROR_E linkerror);

typedef enum
{
    SIO_NOTIFY_READY,
    SIO_NOTIFY_BUSY,
    SIO_NOTIFY_STATUS
} SIO_NOTIFY_ID_E;
typedef void (*SIO_NOTIFY_HANDLE) (uint32 notify_id, uint32 param);

/*****************************************************************************/
// Description :    Register error handle, when sio link error,  driver will call this handle to deal
//                       the errors
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_RegisterErrorHandle (SIO_ERROR_HANDLE errorhandle);

/*****************************************************************************/
// Description :    Register notify handle, under the mix mode, when sio drv receive a ready
//                       package or busy package or status package, it will call the notify handle
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_RegisterNotifyHandle (SIO_NOTIFY_HANDLE notifyhandle);

/*****************************************************************************/
// Description :    Register data receive function
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_RegisterDataRecvHandle (ATC_CALLBACK recvhandle);

/*****************************************************************************/
// Description :    under the MIX_MODE,  use this function to send data package
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SendDataPacket (uint8 *src, uint32 length);

/*****************************************************************************/
// Description :    under the MIX_MODE,  use this function to send status package
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SendStatusPacket (uint8 status);

/*****************************************************************************/
// Description :    under the MIX_MODE,  use this function to send ready package
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SendReadyPacket (void);

/*****************************************************************************/
// Description :    under the MIX_MODE,  use this function to send busy package
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SendBusyPacket (void);

/*****************************************************************************/
// Description :    this function changes userport from one mode to another,
//                       following mode transition can be done by this funcition:
//                       MIX_MODE-->ATC_MODE
//                       MIX_MODE-->DATA_MODE
//                       ATC_MODE-->MIX_MODE
//                       ATC_MODE-->DATA_MODE
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SetUserPortMode (uint32 mode);

/*****************************************************************************/
// Description :    get the mode of COM_USER port
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_GetUserPortMode (void);

/*****************************************************************************/
// Description :    get phy port no from logical port
// Global resource dependence :
// Author :        weihua.wang
// Note :          uint32 port: COM_DEUBG/COM_DATA
/*****************************************************************************/
PUBLIC uint32 SIO_GetPhyPortNo (uint32 port);

/*****************************************************************************/
// Description :    get the mode of COM_USER port
// Global resource dependence :
// Author :         hanjun.liu
// Note :
/*****************************************************************************/

PUBLIC void SIO_SendRemainLogMessage (void);
//@Zhemin.Lin, CR:MS00004213, 08/22/2003, end

/*@jim.zhang CR:MS00008486 04/06/2004 */
/*****************************************************************************/
// Description :   get the mix used state for GPRS.
// Global resource dependence :
// Author :        Jim.zhang
// Note : return 1 means PPP and Log are mixed, 0 means not mixed.
/*****************************************************************************/
PUBLIC BOOLEAN SIO_IsPPPMixedLogel (void);
/* end CR:MS00008486 */

/*
** Send the char of PPP out.
** It's the function called by PPP.
*/
PUBLIC int uart_ppp_putc (int port, uint8 ch);

/*****************************************************************************/
// Description :   Use the given baudrate to init the uart register.
//                 and can receivce/send data via it.
// Global resource dependence :
// Author :        Lin.liu
// Input:
//        port        : uart port number, UART_COM0/1
//        baudrate : the baudrate need to be set( 115200, 9600 etc ).
// Return:
//        None
// Note :
/*****************************************************************************/
PUBLIC void SIO_InitUartLowLevel (uint32 port,  uint32 baudrate);

/*****************************************************************************/
// Description :   get com_debug/com_data rx buffer max size;
// Global resource dependence :
// Author :
// Input:     COM_Port No.
// Return:
//        COM_DEBUG/Data rx buffer max size
// Note :
/*****************************************************************************/
PUBLIC int32 SIO_GetRxBufMaxSize (uint32 port);

/*****************************************************************************/
// Description :  Register COM operations to the table.
// Global resource dependence :
// Author :        eddie.li
// Input:
//
// Return:
//        None
// Note :
/*****************************************************************************/
PUBLIC void SIO_RegisterOpsToTab (uint32                  port,
                                  uint32                  phy_port,
                                  sio_operation_st ops);


PUBLIC uint32 SIO_PhyPortfromIndex (uint32 index);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /*End sio.h*/

