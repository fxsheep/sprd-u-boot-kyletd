#ifndef _SC8800H_REG_EMC_H_
    #define _SC8800H_REG_EMC_H_

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define EXT_MEM_CTL_BASE                0x20000000


#define EXT_MEM_CFG0                    (EXT_MEM_CTL_BASE + 0x0000)
#define EXT_MEM_CFG1                    (EXT_MEM_CTL_BASE + 0x0004)
#define EXT_MEM_CFG2                    (EXT_MEM_CTL_BASE + 0x0008)
#define EXT_MEM_CFG3                    (EXT_MEM_CTL_BASE + 0x000C)
#define EXT_MEM_CFG4                    (EXT_MEM_CTL_BASE + 0x0010)
#define EXT_MEM_STS0                    (EXT_MEM_CTL_BASE + 0x0014)
#define EXT_MEM_STS1                    (EXT_MEM_CTL_BASE + 0x0018)
#define EXT_MEM_STS2                    (EXT_MEM_CTL_BASE + 0x001C)

#define EXT_MEM_DCFG0                   (EXT_MEM_CTL_BASE + 0x0020)
#define EXT_MEM_DCFG1                   (EXT_MEM_CTL_BASE + 0x0024)
#define EXT_MEM_DCFG2                   (EXT_MEM_CTL_BASE + 0x0028)
#define EXT_MEM_DCFG3                   (EXT_MEM_CTL_BASE + 0x002c)
#define EXT_MEM_DCFG4                   (EXT_MEM_CTL_BASE + 0x0030)
#define EXT_MEM_DCFG5                   (EXT_MEM_CTL_BASE + 0x0034)
#define EXT_MEM_DCFG6                   (EXT_MEM_CTL_BASE + 0x0038)

#define EXT_MEM_SCFG0                   (EXT_MEM_CTL_BASE + 0x0040)
#define EXT_MEM_SCFG0_CS0               (EXT_MEM_CTL_BASE + 0x0060)
#define EXT_MEM_SCFG1_CS0               (EXT_MEM_CTL_BASE + 0x0064)
#define EXT_MEM_SCFG0_CS1               (EXT_MEM_CTL_BASE + 0x0068)
#define EXT_MEM_SCFG1_CS1               (EXT_MEM_CTL_BASE + 0x006C)
#define EXT_MEM_SCFG0_CS2               (EXT_MEM_CTL_BASE + 0x0070)
#define EXT_MEM_SCFG1_CS2               (EXT_MEM_CTL_BASE + 0x0074)
#define EXT_MEM_SCFG0_CS3               (EXT_MEM_CTL_BASE + 0x0078)
#define EXT_MEM_SCFG1_CS3               (EXT_MEM_CTL_BASE + 0x007C)


#define EXT_MEM_DL0                     (EXT_MEM_CTL_BASE + 0x0080)
#define EXT_MEM_DL1                     (EXT_MEM_CTL_BASE + 0x0084)
#define EXT_MEM_DL2                     (EXT_MEM_CTL_BASE + 0x0088)
#define EXT_MEM_DL3                     (EXT_MEM_CTL_BASE + 0x008C)
#define EXT_MEM_DL4                     (EXT_MEM_CTL_BASE + 0x0090)
#define EXT_MEM_DL5                     (EXT_MEM_CTL_BASE + 0x0094)
#define EXT_MEM_DL6                     (EXT_MEM_CTL_BASE + 0x0098)
#define EXT_MEM_DL7                     (EXT_MEM_CTL_BASE + 0x009C)
#define EXT_MEM_DL8                     (EXT_MEM_CTL_BASE + 0x00A0)
#define EXT_MEM_DL9                     (EXT_MEM_CTL_BASE + 0x00A4)
#define EXT_MEM_DL10                    (EXT_MEM_CTL_BASE + 0x00A8)
#define EXT_MEM_DL11                    (EXT_MEM_CTL_BASE + 0x00AC)
#define EXT_MEM_DL12                    (EXT_MEM_CTL_BASE + 0x00B0)
#define EXT_MEM_DL13                    (EXT_MEM_CTL_BASE + 0x00B4)
#define EXT_MEM_DL14                    (EXT_MEM_CTL_BASE + 0x00B8)
#define EXT_MEM_DL15                    (EXT_MEM_CTL_BASE + 0x00BC)
#define EXT_MEM_DL16                    (EXT_MEM_CTL_BASE + 0x00C0)
#define EXT_MEM_DL17                    (EXT_MEM_CTL_BASE + 0x00C4)
#define EXT_MEM_DL18                    (EXT_MEM_CTL_BASE + 0x00C8)
#define EXT_MEM_DL19                    (EXT_MEM_CTL_BASE + 0x00CC)
#define EXT_MEM_DL20                    (EXT_MEM_CTL_BASE + 0x00D0)
#define EXT_MEM_DL21                    (EXT_MEM_CTL_BASE + 0x00D4)
#define EXT_MEM_DL22                    (EXT_MEM_CTL_BASE + 0x00D8)
#define EXT_MEM_DL23                    (EXT_MEM_CTL_BASE + 0x00DC)
#define EXT_MEM_DL24                    (EXT_MEM_CTL_BASE + 0x00E0)
#define EXT_MEM_DL25                    (EXT_MEM_CTL_BASE + 0x00E4)
#define EXT_MEM_DL26                    (EXT_MEM_CTL_BASE + 0x00E8)
#define EXT_MEM_DL27                    (EXT_MEM_CTL_BASE + 0x00EC)
#define EXT_MEM_DL28                    (EXT_MEM_CTL_BASE + 0x00F0)
#define EXT_MEM_DL29                    (EXT_MEM_CTL_BASE + 0x00F4)
#define EXT_MEM_DL30                    (EXT_MEM_CTL_BASE + 0x00F8)
#define EXT_MEM_DL31                    (EXT_MEM_CTL_BASE + 0x00FC)


//the extenal memory control register setting under different frequency
#define MCU13M_ECS0_VALUE       		0x08020008
#define MCU13M_ECS1_VALUE       		0x08020008

#define MCU26M_ECS0_VALUE       		0x08021108
#define MCU26M_ECS1_VALUE       		0x08021108

#define MCU39M_ECS0_VALUE       		0x08021208
#define MCU39M_ECS1_VALUE       		0x08021208

#define MCU52M_ECS0_VALUE       		0x08022308
#define MCU52M_ECS1_VALUE       		0x08022308

#define MCU71M_ECS0_VALUE       		0x08024508
#define MCU71M_ECS1_VALUE       		0x08024508

#define MCU78M_ECS0_VALUE       		0x08024508
#define MCU78M_ECS1_VALUE       		0x08024508

#define MCU104M_ECS0_VALUE       		0x08026708
#define MCU104M_ECS1_VALUE       		0x08026708

#define MCU_MAX_ECS0_VALUE      		MCU104M_ECS0_VALUE
#define MCU_MAX_ECS1_VALUE      		MCU104M_ECS1_VALUE

#define DEFAULT_ECS_VALUE				0x0803eeef
#define	ECS_MAX_SUPPORT_NUMBER			8

#define DEFAULT_INT_MEM_CTL     		0xF             	//ARM only use 0x4000_8000 - 0x4000_FFFF


//External Memory (8 CHIP Select).
#define EXTM_XCSN0_START        		0x00000000
#define EXTM_XCSN0_END          		0x03FFFFFF
#define EXTM_XCSN1_START        		0x04000000
#define EXTM_XCSN1_END          		0x07FFFFFF

#define EXTM_XCSN2_START        		0x08000000
#define EXTM_XCSN2_END          		0x0BFFFFFF
#define EXTM_XCSN3_START        		0x0C000000
#define EXTM_XCSN3_END          		0x0FFFFFFF

#define EXTM_XCSN4_START        		0x50000000
#define EXTM_XCSN4_END          		0x53FFFFFF
#define EXTM_XCSN5_START        		0x54000000
#define EXTM_XCSN5_END          		0x57FFFFFF
#define EXTM_XCSN6_START        		0x58000000
#define EXTM_XCSN6_END          		0x5BFFFFFF
#define EXTM_XCSN7_START        		0x5C000000
#define EXTM_XCSN7_END          		0x5FFFFFFF

#ifdef   __cplusplus
    }
#endif
#endif
// End 
