#ifndef _BOOT_MODE_H_
#define _BOOT_MODE_H_

void normal_mode(void);
void recovery_mode(void);
void charge_mode(void);
void dloader_mode(void);
void fastboot_mode(void);
void vlx_mode(void);
void alarm_mode(void);

#define RECOVERY_MODE   0x77665502
#define FASTBOOT_MODE   0x77665500

#endif
