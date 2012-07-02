#include <common.h>
#include <asm/io.h>

#include <asm/arch/regs_adi.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/arch/analog_reg_v3.h>

uint16_t adc_voltage_table[2][2] =
{
	{928,4200},
	{796,3600},
};
uint16_t CHGMNG_AdcvalueToVoltage (uint16_t adcvalue)
{
	int32_t temp;
	temp = adc_voltage_table[0][1] - adc_voltage_table[1][1];
	temp = temp * (adcvalue - adc_voltage_table[0][0]);
	temp = temp / (adc_voltage_table[0][0] - adc_voltage_table[1][0]);

	printf("mingwei uboot vol:%d,adc4200:%d,adc3600:%d\n",temp + adc_voltage_table[0][1],
		adc_voltage_table[0][0],adc_voltage_table[1][0]);

	return temp + adc_voltage_table[0][1];
}
void CHG_TurnOn (void)
{
	ANA_REG_AND (ANA_CHGR_CTL0,~CHGR_PD_BIT);
}

void CHG_ShutDown (void)
{
	ANA_REG_OR (ANA_CHGR_CTL0,CHGR_PD_BIT);
}

void CHG_SetRecharge (void)
{
	ANA_REG_OR (ANA_CHGR_CTL0,CHGR_RECHG_BIT);
}

void CHG_Init (void)
{
	unsigned int efuse_cal_data[2] = {0};
	if(sci_efuse_calibration_get(efuse_cal_data))
	{
		printf("sci_efuse_calibration_get OK");
		adc_voltage_table[0][1]=efuse_cal_data[0]&0xffff;
		adc_voltage_table[0][0]=(efuse_cal_data[0]>>16)&0xffff;
		adc_voltage_table[1][1]=efuse_cal_data[1]&0xffff;
		adc_voltage_table[1][0]=(efuse_cal_data[1]>>16)&0xffff;
	}
	CHG_SetRecharge();
}

