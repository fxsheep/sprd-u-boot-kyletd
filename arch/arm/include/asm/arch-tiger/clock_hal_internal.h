/******************************************************************************
 ** File Name:      clock_hal_internal.h                                           *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                            *
 ******************************************************************************/
#ifndef _CLOCK_HAL_INTERNAL_H_
#define _CLOCK_HAL_INTERNAL_H_

#ifdef __cplusplus
extern   "C"
{
#endif

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
#define CLOCK_SUCCESS     0
#define CLOCK_FAIL        1

struct _CLOCK_OBJECT;
typedef struct _CLOCK_OBJECT CLOCK_OBJECT;
struct _DEVICE_OBJECT;
typedef struct _DEVICE_OBJECT DEVICE_OBJECT;
typedef uint32 CLOCK_STATUS;

/*****************************************************************************/
//  Description:    Config all system clocks and devices
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS CLOCK_Config (void);

/*****************************************************************************/
//  Description:    Open device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_OpenClock (DEVICE_OBJECT *thiz);

/*****************************************************************************/
//  Description:    Reset device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Reset (DEVICE_OBJECT *thiz);

/*****************************************************************************/
//  Description:    Close device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_CloseClock (DEVICE_OBJECT *thiz);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetClockAndDiv (DEVICE_OBJECT *thiz, int ClockValue, int Div);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetClock (DEVICE_OBJECT *thiz, int ClockValue);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_EnableClock (DEVICE_OBJECT *thiz);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_DisableClock (DEVICE_OBJECT *thiz);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetDiv (DEVICE_OBJECT *thiz, int Div);

/*****************************************************************************/
//  Description:    Get device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC uint32 DEVICE_GetClock (DEVICE_OBJECT *thiz);

#ifdef __cplusplus
}
#endif

#endif //_CLOCK_HAL_INTERNAL_H_

