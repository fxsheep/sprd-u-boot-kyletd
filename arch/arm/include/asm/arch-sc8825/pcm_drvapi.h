/******************************************************************************
 ** File Name:      pcm_drvapi.h                                             *
 ** Author:         Thomas.Chai                                                 *
 ** DATE:           4/26/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contains driver for LCM controller.
 **
 ******************************************************************************/

#ifndef _PCM_DRVAPI_H_
#define _PCM_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//pcm operation mode
typedef enum
{
    LONG_FRAME_SYNC_MODE  = 0, //long frame sync mode.
    SHORT_FRAME_SYNC_MODE = 1, //short frame sync mode
    MULTI_SLOT_OPERATOR_MODE = 2
                           }
                           PCM_SYNC_MODE_E;

typedef enum
{
    MASTER_MODE = 0,  //PCM working at master mode
    SLAVE_MODE = 1    //PCM working at slave mode
} PCM_OPERATE_MODE_E;


typedef enum
{
    PCM_SLOT0 = 0x01,
    PCM_SLOT1 = 0x02,
    PCM_SLOT2 = 0x04
} PCM_SLOT_E;

typedef enum
{
    TX_8BITS_PER_FRAME = 0,   // the data bit number of user transmitting/receiving is 8 bits
    TX_16BITS_PER_FRAME = 1,  // the data bit number of user transmitting/receiving is 16 bits
    TX_32BITS_PER_FRAME   = 2 // the data bit number of user transmitting/receiving is 32 bits
} DATA_BIT_NUM_E;


typedef enum
{
    CLOSE_STATUS = 0,
    OPEN_STATUS = 1
} PCM_STATUS_E;

typedef enum
{
    ERR_PCM_SUCCESS = 0,
    ERR_PCM_NO_TX_DATA,        // No vaild data to transmit
    ERR_PCM_TIMEOUT ,               // receive data is timeout
    ERR_PCM_CLOSE ,                 // PCM had been closed
    ERR_PCM_PARAM_SCALE_WRONG ,     // PCM_PARA_T Parameter scale wrong
    ERR_PCM_PARAM_DISMATCH_WRONG,   // INPUT Parameters dismatch
    ERR_PCM_POINTER_NULL,           // Input pointer is PNULL
    ERR_PCM_OPERATE_FAIL,           // Operate fail
    ERR_PCM_FUNC_NOT_SUPPORT        // PCM not support this function
    // Success, no error
} ERR_PCM_E;



/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct
{
    uint32 value;     //the data number had been transmitted/received
    ERR_PCM_E  error;    //the status of current transmitting/receiving
} CALL_RET_T;

//Call back define
typedef ERR_PCM_E (*PCMCallback) (CALL_RET_T ret);


typedef struct
{
    PCM_SYNC_MODE_E    sync_mode;         //pcm operate mode.(sc6600m only support short frame sync mode)
    PCM_OPERATE_MODE_E     operate_mode;      //Master/Slave mode.0:master 1:slave
    DATA_BIT_NUM_E     data_bit_num;    //the data bit number want to transmit
    PCM_SLOT_E        pcm_slot;
    uint8     tx_watermark;            // the watermark value when the Tx interrupt will happen  1 ~~~~~ 32
    uint8     rx_watermark;           // the watermark value when the Rx interrupt will happen
    PCMCallback tx_CallBack;          // Tx call back function
    PCMCallback rx_CallBack;          // Rx call back function
} PCM_PARA_T;

typedef struct
{
    uint32 size;      // Total Size of the buffer.
    uint32 start_point;   // First data in the buffer.
    uint8 *sio_buf_ptr;       // Sio buffer address.
} PCM_BUF_T;

typedef struct
{
    PCM_PARA_T  dev_para;
    PCM_BUF_T   tx_buf;
    PCM_BUF_T   rx_buf;
    PCM_STATUS_E pcm_status;
    uint8  byte_per_frame;
} PCM_HAL_T;



/**---------------------------------------------------------------------------*
 **                      Function  Prototype
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function initalize PCM Paramer
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
PUBLIC void PCM_Init (void);


/*****************************************************************************/
//  Description:    Open a PCM interface, and start to transmit data. you can set
//                  a new paramer for PCM, and you also set the paramer to zero,
//                  then it will use the default setting to PCM. If the new paramer
//                  is unvaild, it also will use the default setting
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
PUBLIC ERR_PCM_E  PCM_Open (PCM_PARA_T *paramer);


/*****************************************************************************/
//  Description:    Give the outside buffer pointer to PCM data sending buffer
//                  pointer. and start the transmit interrupt to transmit data
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC int32  PCM_SendData (uint8 *data_ptr,  //data pointer of user want to send
                            uint32 data_len);  //data length


/*****************************************************************************/
//  Description:    Move received data from fifo to outside buffer
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC int32 PCM_ReceiveData (uint8 *buff_ptr,   //receive data buffer pointer
                              uint32 data_len);   //receive data buffer length

/*****************************************************************************/
//  Description:    stop PCM transmit, and close pcm interface.
//  Global resource dependence:
//  Author:         Livy.Liu&Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC ERR_PCM_E  PCM_Close (void);



/*****************************************************************************/
//  Description:    Set a new PCM Paramer to system, if the new paramer is not right,
//                  the  setting is unuseful
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC ERR_PCM_E PCM_SetParamer (PCM_PARA_T *paramer);

/*****************************************************************************/
//  Description:    Get Current PCM Paramer in the PCM driver
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC ERR_PCM_E  PCM_GetCurrentParamer (PCM_PARA_T   *paramer);


/*****************************************************************************/
//  Description:    PCM ISR Entry
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC void PCM_ISRHandle (uint32 int_num);



#ifdef   __cplusplus
}
#endif

#endif /*_PCM_DRVAPI_H_*/
