#include <common.h>
char * get_mtdparts(void)
{
        return MTDPARTS_DEFAULT;
}

int get_cal_det_ms(void)
{
    return CALIBERATE_DETECT_MS;
}
