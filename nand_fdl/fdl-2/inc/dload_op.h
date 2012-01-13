#ifndef DLOAD_OP_H
#define DLOAD_OP_H

#define DL_OP_MTD_COUNT		(30 * 5) /* mtd partition is not beyond 30, 5 types operation */
#define DL_OP_RECORD_LEN	(83)     /*   01 Base:0x???????? Size:0x???????? Op:StartData ...... */

/* FactoryDownload Tool operation type */
typedef enum DL_OP_TYPE_DEF
{
	STARTDATA = 0,
	MIDSTDATA,
	ENDDATA,
	ERASEFLASH,
	READFLASH
} DL_OP_TYPE_E;

typedef enum DL_OP_STATUS_DEF
{
	SUCCESS = 0,
	FAIL
} DL_OP_STATUS_E;

typedef struct DL_OP_RECORD_DEF
{
    unsigned long   base;
    unsigned long   size;
    DL_OP_TYPE_E    type;
    DL_OP_STATUS_E  status;
    unsigned long   status_cnt;
} DL_OP_RECORD_S;

typedef struct DL_Address_CNT
{
    unsigned long   *base;
    unsigned long   cnt;
} DL_Address_CNT_S;

void get_Dl_Erase_Address_Table(DL_Address_CNT_S *dls);
void get_Dl_Data_Address_Table(DL_Address_CNT_S *dls);

char *Dl_Op_Type_Name[] = {
"StartData  ",
"Download   ",
"EndData    ",
"EraseFlash ",
"ReadFlash  "
};

char *Dl_Op_Status_Name[] = {
"Success ",
"Fail    "
};

#endif /*  DLOAD_OP_H */
