#include <common.h>
#include <asm/arch/adc_reg_v3.h>
#include <asm/arch/regs_adi.h>
#include <asm/arch/regs_ana.h>
#include <asm/arch/adc_drvapi.h>
#include <asm/arch/adi_hal_internal.h>

#define pr_err(fmt...) printf(fmt)
#define pr_warning(fmt...) printf(fmt)

void ADC_Init(void)
{
	ANA_REG_OR(ANA_APB_CLK_EN, ADC_EB | CLK_AUXAD_EN | CLK_AUXADC_EN);
	ANA_REG_OR(ADC_CTRL, ADC_EN_BIT);
}

void ADC_SetCs(adc_channel id)
{
    if(id >= ADC_MAX){
        pr_err("adc limits to 0~%d\n", ADC_MAX);
        return;
    }

    ANA_REG_MSK_OR(ADC_CS, id, ADC_CS_BIT_MSK);
}

void ADC_SetScale(bool scale)
{
    if(ADC_SCALE_1V2 == scale){
        ANA_REG_AND(ADC_CS, ~ADC_SCALE_BIT);
    }else if(ADC_SCALE_3V == scale){
        ANA_REG_OR(ADC_CS, ADC_SCALE_BIT);
    }else
      pr_err("adc scale %d not support\n", scale);
}

void ADC_ConfigTPC(uint8_t x, uint8_t y)
{
    if(x > ADC_MAX || y > ADC_MAX){
        pr_err("tpc x and y channel should be in 0~%d\n", ADC_MAX);
        return;
    }

    ANA_REG_MSK_OR(ADC_TPC_CH_CTRL, x|y<<ADC_TPC_Y_CH_OFFSET, ADC_TPC_X_CH_MSK|ADC_TPC_Y_CH_MSK);
}

int32_t ADC_GetValue(adc_channel id, bool scale)
{
    uint32_t result;
    unsigned long irq_flag;
    uint32_t count;

    // clear int 
    ANA_REG_OR(ADC_INT_CLR, ADC_IRQ_CLR_BIT);

    //choose channel
    ADC_SetCs(id);

    //set ADC scale
    ADC_SetScale(scale);

    //run ADC soft channel
    ANA_REG_OR(ADC_CTRL, SW_CH_ON_BIT);

    count = 12;

    //wait adc complete
    while(!(ANA_REG_GET(ADC_INT_SRC)&ADC_IRQ_RAW_BIT) && count){
        udelay(50);
        count--;
    }
    if (count == 0) {
        pr_warning("WARNING: ADC_GetValue timeout....\n");
        return -1;
    }

    result = ANA_REG_GET(ADC_DAT) & ADC_DATA_MSK; // get adc value
    ANA_REG_AND(ADC_CTRL, ~SW_CH_ON_BIT); // turn off adc soft channel
    ADC_SetCs(TPC_CHANNEL_X);             // set tpc channel x back
    ANA_REG_OR(ADC_INT_CLR, ADC_IRQ_CLR_BIT); // clear irq of this time

    return result;
}
