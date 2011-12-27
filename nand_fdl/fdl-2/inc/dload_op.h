#ifndef DLOAD_OP_H
#define DLOAD_OP_H

#define DL_OP_MTD_COUNT		(30 * 5) /* mtd partition is not beyond 30, 5 types operation */
#define DL_OP_RECORD_LEN	(83)     /*   01 Base:0x???????? Size:0x???????? Op:StartData ...... */

const unsigned long Dl_Data_Address[] = {
0x90000001,
0x80000000,
0x80000001,
0x80000003,
0x80000004,
0x80000007,
0x80000009,
0x8000000a,
0x8000000b,
0x8000000c,
0x8000000f,
0x80000010,
0x90000002
};

const unsigned long Dl_Erase_Address[] = {
0x90000003,
0x8000000d,
0x8000000e
};

unsigned long FactoryDownloadTool_EndPartition_Address = 0x8000000e;
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

const char *Dl_Op_Type_Name[] = {
"StartData  ",
"Download   ",
"EndData    ",
"EraseFlash ",
"ReadFlash  "
};

const char *Dl_Op_Status_Name[] = {
"Success ",
"Fail    "
};

#endif /*  DLOAD_OP_H */
