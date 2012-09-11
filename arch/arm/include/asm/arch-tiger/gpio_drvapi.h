/******************************************************************************
 ** File Name:      gpio_drvapi.h                                                *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           03/18/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 GPIO device.                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/18/2003     Xueliang.Wang     Create.                                  *
 ** 09/05/2003     Zhemin.Lin       Added functions for SM5100B EVB1.0        *
 ** 22/11/2004     Benjamin.Wang    Modified according CR16760              *
 ** 08/01/2008     Yiyue.He         modified the code for more generic adptation
 ******************************************************************************/

#ifndef _GPIO_DRVAPI_H_
#define _GPIO_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

typedef enum GPIO_INT_tag
{
    GPIO_INT_DISABLE = 0,   // Disable the relative gpio interrupt.
    GPIO_INT_LEVEL_HIGH,    // detect high level.
    GPIO_INT_LEVEL_LOW,     // detect low level.
    GPIO_INT_EDGES_BOTH,    // detect the rising edges and falling edges.
    GPIO_INT_EDGES_RISING,  // detect the rising edges.
    GPIO_INT_EDGES_FALLING  // detect the falling edges.
} GPIO_INT_TYPE;


#define GPIO_DEFAULT_SHAKING_TIME   50

// CallBack function pointer prototype.
typedef void (* GPIO_CALLBACK) (uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    This function initialize GPIO function.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_Init (void);

/*****************************************************************************/
//  Description:    This function used to get the direction of a gpio pin
//  Author:           Benjamin.Wang
// retread by:      Yiyue.He
//  Note:              0 - Input
//                        1 - Output
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetDirection (uint32  gpio_id);

/*****************************************************************************/
//  Description:    This function used to set the direction of a gpio pin
//  Author:           Benjamin.Wang
// retread by:      Yiyue.He
//  Note:              0 - Input
//                        1 - Output
/*****************************************************************************/
PUBLIC void GPIO_SetDirection (uint32  gpio_id, BOOLEAN directions);

/*****************************************************************************/
//  Description:    This function used to enable data w/r mask the given gpio port.
//  Author:           Benjamin.Wang
// retread by :     Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_Enable (uint32 gpio_id);

/*****************************************************************************/
//  Description:    This function used to disable the given gpio port.
//  Author:           Benjamin.Wang
// retread by :     Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_Disable (uint32 gpio_id);


/*****************************************************************************/
//  Description:    This function used to get the value of a gpio pin
//  Author:           Lin.Liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  GPIO_GetValue (uint32  gpio_id);

/*****************************************************************************/
//  Description:    This function used to set the value of a gpio pin
//  Author:           Lin.Liu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetValue (uint32 gpio_id, BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function used to Deshaking 0 interrupt
//  Author:         Younger
//  Note:
/*****************************************************************************/

void GPIO_ZeroIntDeShaking (uint32 state);

/*****************************************************************************/
//  Description:    This function use timer to fix 0 interrpt issue
//  Author:         Zhemin.Lin
//  Note:
/*****************************************************************************/

void GPIO_ZeroIntHandler (uint32 int_num);

/*****************************************************************************/
//  Description:    This function used to enable gpio IRQ Interrupt.
//  Author:           Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableInt (void);

/*****************************************************************************/
//  Description:    This function used to disable gpio IRQ Interrupt.
//  Author:           Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DisableInt (void);


/*****************************************************************************/
//  Description:    This function used to enable gpio IRQ Interrupt.
//  Author:           Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableInt (void);

/*****************************************************************************/
//  Description:    This function used to disable gpio IRQ Interrupt.
//  Author:           Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DisableInt (void);

/*****************************************************************************/
//  Description:    This function used to enable the given gpio port's interrupt.
//  Author:           Benjamin.Wang
// retread by:        Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableIntCtl (uint32 gpio_id);

/*****************************************************************************/
//  Description:    This function used to disable the given gpio port's interrupt.
//  Author:           Benjamin.Wang
// retread by:        Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DisableIntCtl (uint32 gpio_id);

/*****************************************************************************/
//  Description:    This function used to get the intr state of a gpio pin
//  Author:         Zhemin.Lin
//  retread by:    Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetIntState (uint32 gpio_id);

/*****************************************************************************/
//  Description:    This function used to clear the given interrupt status bit.
//  Author:           Benjamin.Wang
// retread by:        Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_ClearIntStatus (uint32 gpio_id);

/*****************************************************************************/
//  Description:    This function used to set gpio Interrupt sense type.
//  Author:           Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetInterruptSense (uint32 gpio_id, GPIO_INT_TYPE sensetype);

/*****************************************************************************/
//  Description:    This function GPIO input state
//  Author:         Zhemin.Lin
//  Retread by :  Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetGPIOState (uint32 gpio_id);

/*****************************************************************************/
//  Description:    This function add an callback function to gpio_int_table
//  Author:
//  Retread by: Yiyue.He
//  Note:            This function and GPIO_AddToIntTable() can't be called simultaneously.
/*****************************************************************************/
PUBLIC uint32 GPIO_AddCallbackToIntTable (
    uint32      gpio_id,            // GPIO ID
    BOOLEAN     de_shaking,         // if SCI_TRUE, need de shaking, else needn't
    uint32  shaking_interval,   // interval for de shaking
    GPIO_CALLBACK gpio_callback_fun         // Callback function
);

/*****************************************************************************/
//  Description:    This function deletes an int from gpio_int_table
//  Author:           Benjamin.Wang
//  Retread by: Yiyue.He
//  Note:           If the GPIO INT is triggered by LEVEL, you have to delete it from int table in
//                   interrupt callback functions. Otherwise, the interrupt will be triggered ceaselessly.
/*****************************************************************************/
PUBLIC uint32 GPIO_DeleteFromIntTable (uint32 gpio_id);

/*****************************************************************************/
//  Description:    This function used to know whether the gpio port is enabled.
//  Author:           Yiyue.He
//  notes:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetDataMask (uint32 gpio_id);

/*****************************************************************************/
//  Description:    This function used to set the gpio port to enabled or not
//  Author:           Yiyue.He
//  notes:
/*****************************************************************************/
PUBLIC void GPIO_SetDataMask (uint32 gpio_id, BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function retrun the max number of gpio
//  Author:         Zhemin.Lin
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_GetMaxNumber (void);

PUBLIC void GPIO_SetInitState (uint32  gpio_id, uint32 state);

PUBLIC uint32 GPIO_GetInitState (uint32  gpio_id);

PUBLIC uint32 GPIO_GetShakeInterval (uint32 gpio_id);

PUBLIC void GPIO_GetCallBack (uint32 gpio_id, uint32 gpio_state);


#ifdef __cplusplus
}
#endif

#endif  // _GPIO_DRVAPI_H_

// End of gpio_drvapi.h
