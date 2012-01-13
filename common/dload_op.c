
typedef struct DL_Address_CNT
{
    unsigned long   *base;
    unsigned long   cnt;
} DL_Address_CNT_S;

unsigned long Dl_Data_Address_Table[] = {
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

unsigned long Dl_Erase_Address_Table[] = {
0x90000003,
0x8000000d,
0x8000000e	/* the last ddress must be the last erase partition logical address in factorydownload tool */
};

void get_Dl_Data_Address_Table(DL_Address_CNT_S *dls)
{
	dls->base = Dl_Data_Address_Table;
	dls->cnt = sizeof(Dl_Data_Address_Table) / sizeof(unsigned long);
}

void get_Dl_Erase_Address_Table(DL_Address_CNT_S *dls)
{
	dls->base = Dl_Erase_Address_Table;
	dls->cnt = sizeof(Dl_Erase_Address_Table) / sizeof(unsigned long);
}

int get_DL_Status()
{
	return 0; /* factorydownload don't check dl status */
	//return 1; /* factorydownload check dl status */
}


