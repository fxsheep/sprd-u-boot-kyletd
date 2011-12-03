#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

#include <linux/key_code.h>

static unsigned char board_key_map[]={
    0x27, KEY_HOME,
    0x42, KEY_BACK,
    0x41, KEY_VOLUMEUP,
    0x30, KEY_MENU,
};

#define CONFIG_KEYPAD_ROW_CNT 8
#define CONFIG_KEYPAD_COL_CNT 8
#define CONFIG_KEYPAD_LONG_CNT 0xc
#define CONFIG_KEYPAD_DEBOUNCE_CNT 0x5
#endif //_KEY_MAP_H_
