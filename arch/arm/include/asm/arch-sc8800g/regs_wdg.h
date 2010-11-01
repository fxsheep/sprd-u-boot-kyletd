#ifndef _SC8800H_REG_WDG_H_
    #define _SC8800H_REG_WDG_H_

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define WDG_BASE                		0x89003000

#define	WDG_LOAD						(WDG_BASE + 0x0000)
#define	WDG_VALUE						(WDG_BASE + 0x0004)
#define	WDG_CTL							(WDG_BASE + 0x0008)
#define	WDG_INT_CLR						(WDG_BASE + 0x000C)
#define	WDG_INT_RAW_STS					(WDG_BASE + 0x0010)
#define	WDG_INT_MASKED_STS				(WDG_BASE + 0x0014)
#define	WDG_LOCK						(WDG_BASE + 0x0C00)

#ifdef   __cplusplus
    }
#endif
#endif
// End 
