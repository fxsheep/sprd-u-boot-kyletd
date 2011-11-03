#include <common.h>
char * get_mtdparts(void)
{
        return MTDPARTS_DEFAULT;
}

int get_cal_det_ms(void)
{
    return CALIBERATE_DETECT_MS;
}
unsigned int get_bat_low_level(void)
{
    return LOW_BAT_ADC_LEVEL;
}

unsigned int get_pwr_key_cnt(void)
{
    return PWR_KEY_DETECT_CNT;
}
