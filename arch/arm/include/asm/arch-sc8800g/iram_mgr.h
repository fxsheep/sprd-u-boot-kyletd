#ifndef IRAM_MGR_H
#define IRAM_MGR_H

#include "sci_types.h"  

/*-----------------------------------------------------------------------------
 * IRAM_AREA
 */
typedef struct _IRAM_AREA {
    uint32  iram_addr;
    uint32  iram_length;
    void    *data;
    uint32  attri;
} IRAM_AREA;

/*-----------------------------------------------------------------------------
 * IRAM attribute
 */
#define IRAM_ATTRIBUTE_NORMAL           ((uint32)0x00000000)
#define IRAM_ATTRIBUTE_SAVE_CONTEXT     ((uint32)0x00000001) 
#define IRAM_ATTRIBUTE_ZI               ((uint32)0x00000002)

/*-----------------------------------------------------------------------------
 * IRAM client handle
 */
struct _IRAM_CLIENT;
typedef struct _IRAM_CLIENT *ICLIENT_HANDLE;

/*-----------------------------------------------------------------------------
 * IRAM lock handle
 */
struct _IRAM_LOCK;
typedef struct _IRAM_LOCK *ILOCK_HANDLE;

/*-----------------------------------------------------------------------------
 * IRAM invalid handle
 */
#define IRAM_INVALID_HANDLE             (NULL) 

/*-----------------------------------------------------------------------------
 * IRAM wait option
 */
#define IRAM_WAIT_FOR_EVER              (0xFFFFFFFF)
#define IRAM_NO_WAIT                    (0x00000000)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
 * IRAM_Init
 *      Initialize IRAM module.
 *
 * Parameters:
 *      area -
 *      count -
 *
 * Returns:
 *      TRUE -
 *      FLASE -
 */
BOOLEAN IRAM_Init(const IRAM_AREA *area, int32 count);

/*-----------------------------------------------------------------------------
 * IRAM_RegisterArea
 *      Regsiter area.
 *
 * Parameters:
 *      area - 
 *      count -
 *
 * Returns:
 *      NULL -
 *      !NULL -
 */
ICLIENT_HANDLE IRAM_RegisterArea(const IRAM_AREA *area, int32 count);

/*-----------------------------------------------------------------------------
 * IRAM_UnregisterArea
 *      Unregister area
 *
 * Parameters:
 *      client - Handle returned by IRAM_RegsiterArea
 *
 * Returns:
 *      TRUE -
 *      FALSE -
 */
BOOLEAN IRAM_UnregisterArea(ICLIENT_HANDLE client);

/*-----------------------------------------------------------------------------
 * IRAM_LockArea
 *      Lock area.
 *
 * Parameters:
 *      area - 
 *      count -
 *      wait -
 *
 * Returns:
 *      NULL -
 *      !NULL -
 */
ILOCK_HANDLE IRAM_LockArea(const IRAM_AREA *area, int32 count, uint32 wait);

/*-----------------------------------------------------------------------------
 * IRAM_LockRegisteredArea
 *      Lock registered area.
 *
 * Parameters:
 *      client  - Handle returned by IRAM_RegisterArea
 *      wait    -
 *      forceZI -
 *
 * Returns:
 *      NULL -
 *      !NULL -
 */
ILOCK_HANDLE IRAM_LockRegisteredArea(ICLIENT_HANDLE client, uint32 wait, BOOLEAN forceZI);

/*-----------------------------------------------------------------------------
 * IRAM_UnlockArea
 *      Unlock area.
 *
 * Parameters:
 *      lock - Handle returned by IRAM_Lock*
 *
 * Returns:
 *      TRUE -
 *      FALSE -
 */
BOOLEAN IRAM_UnlockArea(ILOCK_HANDLE lock);   

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IRAM_MGR_H */
