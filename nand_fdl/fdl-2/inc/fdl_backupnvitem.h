/* the following code are all copied from Dload, such as sc8810.xml */
#ifndef FDL_BACKUPNVITEM_H
#define FDL_BACKUPNVITEM_H

/*
				<NVItem name="Calibration" backup="1">
					<ID>0xFFFFFFFF</ID>
					<BackupFlag use="1">
						<NVFlag name ="Replace" check ="0"></NVFlag>
						<NVFlag name ="Continue" check ="0"></NVFlag>
					</BackupFlag>
				</NVItem>
				<NVItem name="IMEI" backup="1">
					<ID>0xFFFFFFFF</ID>
					<BackupFlag use="0"></BackupFlag>
				</NVItem>
				<NVItem name="TD_Calibration" backup="1">
					<ID>0xFFFFFFFF</ID>
					<BackupFlag use="1">
						<NVFlag name ="Replace" check ="0"></NVFlag>
						<NVFlag name ="Continue" check ="0"></NVFlag>
					</BackupFlag>
				</NVItem>
				<NVItem name="BlueTooth" backup="1">
					<ID>0x191</ID>
					<BackupFlag use="1">
						<NVFlag name ="Continue" check ="0"></NVFlag>
					</BackupFlag>
				</NVItem>
				<NVItem name="BandSelect" backup="1">
					<ID>0xD</ID>
					<BackupFlag use="1">
						<NVFlag name ="Continue" check ="0"></NVFlag>
					</BackupFlag>
				</NVItem>
				<NVItem name="WIFI" backup="1">
					<ID>0x199</ID>
					<BackupFlag use="1">
						<NVFlag name ="Continue" check ="1"></NVFlag>
					</BackupFlag>
				</NVItem>
				<NVItem name="MMITest" backup="1">
					<ID>0x19A</ID>
					<BackupFlag use="1">
						<NVFlag name ="Continue" check ="1"></NVFlag>
					</BackupFlag>
				</NVItem>
*/


/* define struction for above backup nv item */

#define MAX_PATH		(20)
#define MAX_NV_BACKUP_FALG_NUM	(5)

typedef struct _NV_BACKUP_FLAG_T
{
	unsigned char	szFlagName[MAX_PATH];	/* NVFlag name */
	unsigned long	dwCheck;		/* check ="0" */
}NV_BACKUP_FLAG_T,*PNV_BACKUP_FLAG_T;

typedef struct _NV_BACKUP_ITEM_T
{
	unsigned char 	szItemName[MAX_PATH];	/* NVItem name */
	unsigned long   wIsBackup;		/* backup="1" */
	unsigned long   wIsUseFlag;		/* BackupFlag use */	
	unsigned long  	dwID;
	unsigned long  	dwFlagCount;		/* NVFlag name count */
	NV_BACKUP_FLAG_T nbftArray[MAX_NV_BACKUP_FALG_NUM];
}NV_BACKUP_ITEM_T,*PNV_BACKUP_ITEM_T;

NV_BACKUP_ITEM_T backupnvitem[] = 
{
	{"Calibration", 1, 1, 0xFFFFFFFF, 2, {{"Replace", 0}, {"Continue", 0}}},
	{"IMEI", 1, 0, 0xFFFFFFFF, 0},
	{"IMEI2", 1, 0, 0x179, 0},
	{"IMEI3", 1, 0, 0x186, 0},
	{"IMEI4", 1, 0, 0x1E4, 0},
	{"TD_Calibration", 1, 1, 0xFFFFFFFF, 2, {{"Replace", 0}, {"Continue", 0}}},
	{"BlueTooth", 1, 1, 0x191, 1, {"Continue", 0}},
	{"BandSelect", 1, 1, 0xD, 1, {"Continue", 0}},
	{"WIFI", 1, 1, 0x199, 1, {"Continue", 1}},
	{"MMITest", 1, 1, 0x19A, 1, {"Continue", 1}}
};

/* GSMCaliVaPolicy is from [DownloadNV] in BMFileType.ini */
unsigned long GSMCaliVaPolicy = 0;

#endif /* FDL_BACKUPNVITEM_H */
