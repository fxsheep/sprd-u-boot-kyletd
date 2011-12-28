#include <common.h>
char * get_mtdparts(void)
{
        return MTDPARTS_DEFAULT;
}

int get_cal_enum_ms(void)
{
    return CALIBRATE_ENUM_MS;
}
int get_cal_io_ms(void)
{
    return CALIBRATE_IO_MS;
}
unsigned int get_bat_low_level(void)
{
    return LOW_BAT_ADC_LEVEL;
}

unsigned int get_pwr_key_cnt(void)
{
    return PWR_KEY_DETECT_CNT;
}

unsigned int get_alarm_lead_set(void)
{
    return ALARM_LEAD_SET_MS;
}
