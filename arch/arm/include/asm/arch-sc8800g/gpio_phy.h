/******************************************************************************
 ** File Name:      gpio_phy.h                                                *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           03/14/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of   GPIO device. *
 **                                                                                                                                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME                DESCRIPTION                           *
 ******************************************************************************/
#ifndef _GPIO_PHY_H_
#define _GPIO_PHY_H_


typedef struct
{

    GPIO_SECTION_E gpio_type;
    uint32 baseAddr;
    uint16 bit_num;

} GPIO_INFO_T;


/*****************************************************************************/
//  Description:    This function get gpio module base info.
//  Dependency:     Gpio_GetCfgSectionTable(uint32 *table)
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_GetBaseInfo (uint32 gpio_id, GPIO_INFO_T *gpio_info);

/*****************************************************************************/
//  Description:    This function used to get the state of a gpio gpi gpo pin
//  Author:         Zhemin.Lin
//  retread by:     Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetPinData (GPIO_INFO_T *pGpio_info);
/*****************************************************************************/
//  Description:    This function used to get the state of a gpio pin
//  Author:         Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetPinData (GPIO_INFO_T *pGpio_info ,BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function used to know whether the gpio port is enabled.
//  notes:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetDataMask (GPIO_INFO_T *pGpio_info);

/*****************************************************************************/
//  Description:    This function used to know whether the gpio port is enabled.
//  Note:              0 - disable
//                     1 - enable
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetDataMask (GPIO_INFO_T *pGpio_info, BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function used to know whether the gpio int mask is enabled.
//                  Interrupt mask register, "1" corresponding pin is not masked.
//                  "0" corresponding pin interrupt is masked
//  Author:         Benjamin.Wang
//  Retreat by:     Steve.Zhan
//  Note:           SCI_FALSE - disable
//                  SCI_TRUE - enable
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetIntIsMask (GPIO_INFO_T *pGpio_info);

/*****************************************************************************/
//  Description:    This function used to Set GPIO IE.
//  Note:              0 - disable
//                     1 - enable
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetIntMask (GPIO_INFO_T *pGpio_info);

/*****************************************************************************/
//  Description:    This function used to Set GPIO IE.
//  Note:              0 - disable
//                     1 - enable
/*****************************************************************************/
PUBLIC void GPIO_PHY_CleanIntMask (GPIO_INFO_T *pGpio_info);
/*****************************************************************************/
//  Description:    This function used to get the direction of a gpio pin
//  Note:              0 - Input
//                     1 - Output
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetDirection (GPIO_INFO_T *pGpio_info);


/*****************************************************************************/
//  Description:    This function used to set the direction of a gpio pin
//  Note:              0 - Input
//                        1 - Output
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetDirection (GPIO_INFO_T *pGpio_info, BOOLEAN directions);

/*****************************************************************************/
//  Description:    This function used to get the intr state of a gpio pin
//  Author:         Zhemin.Lin
//  retread by:    Yiyue.He
//  Retreat by:     Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetIntState (GPIO_INFO_T *pGpio_info);

/*****************************************************************************/
//  Description:    This function used to clear the given interrupt status bit.
//  Author:           Benjamin.Wang
// retread by:        Yiyue.He
//  Retreat by:     Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_ClearIntStatus (GPIO_INFO_T *pGpio_info);

/*****************************************************************************/
//  Description:    This function used to set gpio Interrupt sense type.
//  Author:         Benjamin.Wang
//  Retreat by:     Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetInterruptSense (GPIO_INFO_T *pGpio_info, GPIO_INT_TYPE sensetype);

/*****************************************************************************/
//  Description:    This function used to set gpin Debounce time.
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetGPIDebounce (GPIO_INFO_T *pGpio_info, uint8 debounce_period);

/*****************************************************************************/
//  Description:    This function used to set  gpi  Interrupt Trigering
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_TrigGPIDetect (GPIO_INFO_T *pGpio_info);

/*****************************************************************************/
//  Description:    This function used to Enable gpi Detect function
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_EnableGPIDetect (GPIO_INFO_T *pGpio_info);

#ifdef GPIO_DRV_DEBUG
#define GPIO_PRINT(_x_)       SCI_TRACE_LOW _x_
#define GPIO_ASSERT(_x_)     SCI_ASSERT(_x_)
#else
#define GPIO_PRINT(_x_)
#define GPIO_ASSERT(_x_)
#endif  // GPIO_DRV_DEBUG

#ifdef GPIO_ASSERT_IF_WRONG_SECTION
#define GPIO_SECTION_ASSERT SCI_ASSERT
#else
#define GPIO_SECTION_ASSERT
#endif

#endif

