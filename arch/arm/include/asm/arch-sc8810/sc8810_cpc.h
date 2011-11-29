#ifndef _SC8810_CPC_H_
#define _SC8810_CPC_H_

#include <common.h>
#ifdef CONFIG_SC8810
#include <asm/arch/sc8810_reg_base.h>
#else
#error "no soc defined"
#endif

#define PIN_CTL_BASE CHIPPIN_CTL_BEGIN
#define PIN_CTL_REG CHIPPIN_CTL_BEGIN

#define ANA_CPC_BASE (ADI_CTL_BEGAIN + 0x180)
#define ANA_PIN_CTL_BASE (ANA_CPC_BASE + 0x8C)

#endif //_SC8810_CPC_H_