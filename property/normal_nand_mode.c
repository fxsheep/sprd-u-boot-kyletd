#include "normal_mode.h"

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);
static int flash_page_size = 0;
int is_factorymode()
{
	int ret = 0;
	char *factorymodepoint = "/productinfo";
	char *factorymodefilename = "/productinfo/factorymode.file";
	cmd_yaffs_mount(factorymodepoint);
	ret = cmd_yaffs_ls_chk(factorymodefilename );
	cmd_yaffs_umount(factorymodepoint);
	return ret;
}
int read_logoimg(char *bmp_img,size_t size)
{
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	loff_t off = 0;

	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return -1;
	}
#define SPLASH_PART "fastboot_logo"

	ret = find_dev_and_part(SPLASH_PART, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", SPLASH_PART);
		return -1;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", SPLASH_PART);
		return -1;
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];

	ret = nand_read_offset_ret(nand, off, &size, (void *)bmp_img, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
		return -1;
	}
	return 0;
}
void addbuf(char *buf)
{
#if !(BOOT_NATIVE_LINUX)
	int str_len = strlen(buf);
	char * mtdpart_def = NULL;
	mtdpart_def = get_mtdparts();
	sprintf(&buf[str_len], " %s", mtdpart_def);
#endif
}
void addcmdline(char *buf)
{

}
int read_spldata()
{
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	loff_t off = 0;
	struct mtd_info *nand;
	int size = CONFIG_SPL_LOAD_LEN;

	int ret = find_dev_and_part(SPL_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", SPL_PART);
		return -1;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", SPL_PART);
		return -1;
	}
	off = part->offset;
	nand = &nand_info[dev->id->num];
	flash_page_size = nand->writesize;

	ret = nand_read_offset_ret(nand, off, &size, (void*)spl_data, &off);
	if(ret != 0) {
		printf("spl nand read error %d\n", ret);
		return -1;
	}
	return 0;
}
void vlx_nand_boot(char * kernel_pname, char * cmdline, int backlight_set)
{
    boot_img_hdr *hdr = (void *)raw_header;
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	size_t size;
	loff_t off = 0;
	char *fixnvpoint = "/fixnv";
	char *fixnvfilename = "/fixnv/fixnv.bin";
	char *backupfixnvpoint = "/backupfixnv";
	char *backupfixnvfilename = "/backupfixnv/fixnv.bin";
	char *runtimenvpoint = "/runtimenv";
	char *runtimenvfilename = "/runtimenv/runtimenv.bin";
	char *runtimenvfilename2 = "/runtimenv/runtimenvbkup.bin";
	char *productinfopoint = "/productinfo";
	char *productinfofilename = "/productinfo/productinfo.bin";
	char *productinfofilename2 = "/productinfo/productinfobkup.bin";
	int orginal_right, backupfile_right;
	unsigned long orginal_index, backupfile_index;
	nand_erase_options_t opts;
	char * mtdpart_def = NULL;
	#if (defined CONFIG_SC8810) || (defined CONFIG_SC8825)
	MMU_Init(CONFIG_MMU_TABLE_ADDR);
	#endif
	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return;
	}

#ifdef CONFIG_SPLASH_SCREEN
#define SPLASH_PART "boot_logo"
	ret = find_dev_and_part(SPLASH_PART, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", SPLASH_PART);
		return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", SPLASH_PART);
		return;
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];
	//read boot image header
	size = 1<<19;//where the size come from????
	char * bmp_img = malloc(size);
	if(!bmp_img){
	    printf("not enough memory for splash image\n");
	    return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void *)bmp_img, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
		return;
	}
   lcd_display_logo(backlight_set,(ulong)bmp_img,size);
#endif
    set_vibrator(0);

#if !(BOOT_NATIVE_LINUX)
	/*int good_blknum, bad_blknum;
	nand_block_info(nand, &good_blknum, &bad_blknum);
	printf("good is %d  bad is %d\n", good_blknum, bad_blknum);*/
	///////////////////////////////////////////////////////////////////////
	/* recovery damaged fixnv or backupfixnv */
	orginal_right = 0;
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
	cmd_yaffs_mount(fixnvpoint);
	ret = cmd_yaffs_ls_chk(fixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
		if (1 == fixnv_is_correct_endflag((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
			orginal_right = 1;//right
	}

	cmd_yaffs_umount(fixnvpoint);

	backupfile_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, FIXNV_SIZE + 4);
	cmd_yaffs_mount(backupfixnvpoint);
	ret = cmd_yaffs_ls_chk(backupfixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (1 == fixnv_is_correct_endflag((unsigned char *)RUNTIMENV_ADR, FIXNV_SIZE))
			backupfile_right = 1;//right
	}
	cmd_yaffs_umount(backupfixnvpoint);

	if ((orginal_right == 1) && (backupfile_right == 1)) {
		/* check index */
		orginal_index = get_nv_index((unsigned char *)FIXNV_ADR, FIXNV_SIZE);
		backupfile_index = get_nv_index((unsigned char *)RUNTIMENV_ADR, FIXNV_SIZE);
		if (orginal_index != backupfile_index) {
			orginal_right = 1;
			backupfile_right = 0;
		}
	}

	if ((orginal_right == 1) && (backupfile_right == 0)) {
		printf("fixnv is right, but backupfixnv is wrong, so erase and recovery backupfixnv\n");
		////////////////////////////////
		find_dev_and_part(BACKUPFIXNV_PART, &dev, &pnum, &part);
		//printf("offset = 0x%08x  size = 0x%08x\n", part->offset, part->size);
		nand = &nand_info[dev->id->num];
		memset(&opts, 0, sizeof(opts));
		opts.offset = part->offset;
		opts.length = part->size;
		opts.quiet = 1;
		nand_erase_opts(nand, &opts);
		////////////////////////////////
		cmd_yaffs_mount(backupfixnvpoint);
		cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)FIXNV_ADR, (FIXNV_SIZE + 4));
		cmd_yaffs_ls_chk(backupfixnvfilename);
		cmd_yaffs_umount(backupfixnvpoint);
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
		printf("backupfixnv is right, but fixnv is wrong, so erase and recovery fixnv\n");
		////////////////////////////////
		find_dev_and_part(FIXNV_PART, &dev, &pnum, &part);
		//printf("offset = 0x%08x  size = 0x%08x\n", part->offset, part->size);
		nand = &nand_info[dev->id->num];
		memset(&opts, 0, sizeof(opts));
		opts.offset = part->offset;
		opts.length = part->size;
		opts.quiet = 1;
		nand_erase_opts(nand, &opts);
		////////////////////////////////
		cmd_yaffs_mount(fixnvpoint);
		cmd_yaffs_mwrite_file(fixnvfilename, (char *)RUNTIMENV_ADR, (FIXNV_SIZE + 4));
		cmd_yaffs_ls_chk(fixnvfilename);
		cmd_yaffs_umount(fixnvpoint);
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nfixnv and backupfixnv are all wrong.\n\n");
	}
	///////////////////////////////////////////////////////////////////////
	/* FIXNV_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
	cmd_yaffs_mount(fixnvpoint);
	ret = cmd_yaffs_ls_chk(fixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
		cmd_yaffs_umount(fixnvpoint);
		if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
			memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
			/* backup nv start */
			cmd_yaffs_mount(backupfixnvpoint);
			ret = cmd_yaffs_ls_chk(backupfixnvfilename);
			if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)FIXNV_ADR);
				cmd_yaffs_umount(backupfixnvpoint);
				if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
					memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
				}
			} else
				cmd_yaffs_umount(backupfixnvpoint);
			/* backup nv end */
		}
	} else {
		cmd_yaffs_umount(fixnvpoint);
		/* backup nv start */
		cmd_yaffs_mount(backupfixnvpoint);
		ret = cmd_yaffs_ls_chk(backupfixnvfilename);
		if (ret == (FIXNV_SIZE + 4)) {
			cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)FIXNV_ADR);
			cmd_yaffs_umount(backupfixnvpoint);
			if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
				memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
			}
		} else
			cmd_yaffs_umount(backupfixnvpoint);
		/* backup nv end */
	}
	//array_value((unsigned char *)FIXNV_ADR, FIXNV_SIZE);


	/* recovery damaged productinfo or productinfobkup */
	orginal_right = 0;
	memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
	cmd_yaffs_mount(productinfopoint);
	ret = cmd_yaffs_ls_chk(productinfofilename);
	if (ret == (PRODUCTINFO_SIZE + 4)) {
		cmd_yaffs_mread_file(productinfofilename, (unsigned char *)PRODUCTINFO_ADR);
		if (1 == sn_is_correct_endflag((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE))
			orginal_right = 1;//right
	}
	cmd_yaffs_umount(productinfopoint);

	backupfile_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, PRODUCTINFO_SIZE + 4);
	cmd_yaffs_mount(productinfopoint);
	ret = cmd_yaffs_ls_chk(productinfofilename2);
	if (ret == (PRODUCTINFO_SIZE + 4)) {
		cmd_yaffs_mread_file(productinfofilename2, (unsigned char *)RUNTIMENV_ADR);
		if (1 == sn_is_correct_endflag((unsigned char *)RUNTIMENV_ADR, PRODUCTINFO_SIZE))
			backupfile_right = 1;//right
	}
	cmd_yaffs_umount(productinfopoint);
	if ((orginal_right == 1) && (backupfile_right == 1)) {
		/* check index */
		orginal_index = get_productinfo_index((unsigned char *)PRODUCTINFO_ADR);
		backupfile_index = get_productinfo_index((unsigned char *)RUNTIMENV_ADR);
		if (orginal_index != backupfile_index) {
			orginal_right = 1;
			backupfile_right = 0;
		}
	}
	if ((orginal_right == 1) && (backupfile_right == 0)) {
		printf("productinfo is right, but productinfobkup is wrong, so recovery productinfobkup\n");
		cmd_yaffs_mount(productinfopoint);
		cmd_yaffs_mwrite_file(productinfofilename2, (char *)PRODUCTINFO_ADR, (PRODUCTINFO_SIZE + 4));
		cmd_yaffs_ls_chk(productinfofilename2);
		cmd_yaffs_umount(productinfopoint);
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
		printf("productinfobkup is right, but productinfo is wrong, so recovery productinfo\n");
		cmd_yaffs_mount(productinfopoint);
		cmd_yaffs_mwrite_file(productinfofilename, (char *)RUNTIMENV_ADR, (PRODUCTINFO_SIZE + 4));
		cmd_yaffs_ls_chk(productinfofilename);
		cmd_yaffs_umount(productinfopoint);
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nproductinfo and productinfobkup are all wrong or no phasecheck.\n\n");
	}
	///////////////////////////////////////////////////////////////////////
	/* PRODUCTINFO_PART */
	printf("Reading productinfo to 0x%08x\n", PRODUCTINFO_ADR);
	cmd_yaffs_mount(productinfopoint);
	ret = cmd_yaffs_ls_chk(productinfofilename);
	if (ret == (PRODUCTINFO_SIZE + 4)) {
		cmd_yaffs_mread_file(productinfofilename, (unsigned char *)PRODUCTINFO_ADR);
		if (-1 == sn_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
			memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
			ret = cmd_yaffs_ls_chk(productinfofilename2);
			if (ret == (PRODUCTINFO_SIZE + 4)) {
				cmd_yaffs_mread_file(productinfofilename2, (unsigned char *)PRODUCTINFO_ADR);
				if (-1 == sn_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
					memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
				}
			}
		}
	} else {
		memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
		ret = cmd_yaffs_ls_chk(productinfofilename2);
		if (ret == (PRODUCTINFO_SIZE + 4)) {
			cmd_yaffs_mread_file(productinfofilename2, (unsigned char *)PRODUCTINFO_ADR);
			if (-1 == sn_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
				memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
			}
		}
	}
	cmd_yaffs_umount(productinfopoint);
	//array_value((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE);
	eng_phasechecktest((unsigned char *)PRODUCTINFO_ADR, SP09_MAX_PHASE_BUFF_SIZE);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	/* RUNTIMEVN_PART */
	/* recovery damaged runtimenv or runtimenvbkup */
	orginal_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
	cmd_yaffs_mount(runtimenvpoint);
	ret = cmd_yaffs_ls_chk(runtimenvfilename);
	if (ret == RUNTIMENV_SIZE) {
		cmd_yaffs_mread_file(runtimenvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE))
			orginal_right = 1;//right
	}
	cmd_yaffs_umount(runtimenvpoint);

	backupfile_right = 0;
	memset((unsigned char *)DSP_ADR, 0xff, RUNTIMENV_SIZE);
	cmd_yaffs_mount(runtimenvpoint);
	ret = cmd_yaffs_ls_chk(runtimenvfilename2);
	if (ret == RUNTIMENV_SIZE) {
		cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)DSP_ADR);
		if (1 == runtimenv_is_correct((unsigned char *)DSP_ADR, RUNTIMENV_SIZE))
			backupfile_right = 1;//right
	}
	cmd_yaffs_umount(runtimenvpoint);

	if ((orginal_right == 1) && (backupfile_right == 0)) {
		printf("runtimenv is right, but runtimenvbkup is wrong, so recovery runtimenvbkup\n");
		cmd_yaffs_mount(runtimenvpoint);
		cmd_yaffs_mwrite_file(runtimenvfilename2, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE);
		cmd_yaffs_ls_chk(runtimenvfilename2);
		cmd_yaffs_umount(runtimenvpoint);
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
		printf("productinfobkup is right, but productinfo is wrong, so recovery productinfo\n");
		cmd_yaffs_mount(runtimenvpoint);
		cmd_yaffs_mwrite_file(runtimenvfilename, (char *)DSP_ADR, RUNTIMENV_SIZE);
		cmd_yaffs_ls_chk(runtimenvfilename);
		cmd_yaffs_umount(runtimenvpoint);
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nruntimenv and runtimenvbkup are all wrong or no runtimenv.\n\n");
	}

	printf("Reading runtimenv to 0x%08x\n", RUNTIMENV_ADR);
	/* runtimenv */
    cmd_yaffs_mount(runtimenvpoint);
	ret = cmd_yaffs_ls_chk(runtimenvfilename);
	if (ret == RUNTIMENV_SIZE) {
		/* file exist */
		cmd_yaffs_mread_file(runtimenvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (-1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
			/* file isn't right and read backup file */
			memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
			ret = cmd_yaffs_ls_chk(runtimenvfilename2);
			if (ret == RUNTIMENV_SIZE) {
				cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)RUNTIMENV_ADR);
				if (-1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
					/* file isn't right */
					memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
				}
			}
		}
	} else {
		/* file don't exist and read backup file */
		memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
		ret = cmd_yaffs_ls_chk(runtimenvfilename2);
		if (ret == RUNTIMENV_SIZE) {
			cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)RUNTIMENV_ADR);
			if (-1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
				/* file isn't right */
				memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
			}
		}
	}
	cmd_yaffs_umount(runtimenvpoint);
	//array_value((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE);

	////////////////////////////////////////////////////////////////
	/* DSP_PART */
	printf("Reading dsp to 0x%08x\n", DSP_ADR);
	ret = find_dev_and_part(DSP_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", DSP_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", DSP_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	flash_page_size = nand->writesize;
	size = (DSP_SIZE + (flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("dsp image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)DSP_ADR, &off);
	if(ret != 0) {
		printf("dsp nand read error %d\n", ret);
		return;
	}
	secure_check(DSP_ADR, 0, DSP_ADR + DSP_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
#endif
	////////////////////////////////////////////////////////////////
	/* KERNEL_PART */
	printf("Reading kernel to 0x%08x\n", KERNEL_ADR);

	ret = find_dev_and_part(kernel_pname, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", kernel_pname);
        return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", kernel_pname);
        return;
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];
	//read boot image header
	size = nand->writesize;
	flash_page_size = nand->writesize;
	ret = nand_read_offset_ret(nand, off, &size, (void *)hdr, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
        return;
	}
	if(memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)){
		printf("bad boot image header, give up read!!!!\n");
        return;
	}
	else
	{
		//read kernel image
		size = (hdr->kernel_size+(flash_page_size - 1)) & (~(flash_page_size - 1));
		if(size <=0){
			printf("kernel image should not be zero\n");
			return;
		}
		ret = nand_read_offset_ret(nand, off, &size, (void *)KERNEL_ADR, &off);
		if(ret != 0){
			printf("kernel nand read error %d\n", ret);
			return;
		}
		//read ramdisk image
		size = (hdr->ramdisk_size+(flash_page_size - 1)) & (~(flash_page_size - 1));
		if(size<0){
			printf("ramdisk size error\n");
			return;
		}
		ret = nand_read_offset_ret(nand, off, &size, (void *)RAMDISK_ADR, &off);
		if(ret != 0){
			printf("ramdisk nand read error %d\n", ret);
			return;
		}
	}

#if !(BOOT_NATIVE_LINUX)
	////////////////////////////////////////////////////////////////
	/* MODEM_PART */
	printf("Reading modem to 0x%08x\n", MODEM_ADR);
	ret = find_dev_and_part(MODEM_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", MODEM_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", MODEM_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	flash_page_size = nand->writesize;
	size = (MODEM_SIZE +(flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("modem image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)MODEM_ADR, &off);
	if(ret != 0) {
		printf("modem nand read error %d\n", ret);
		return;
	}

	secure_check(MODEM_ADR, 0, MODEM_ADR + MODEM_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
	//array_value((unsigned char *)MODEM_ADR, MODEM_SIZE);

	////////////////////////////////////////////////////////////////
	/* VMJALUNA_PART */
	printf("Reading vmjaluna to 0x%08x\n", VMJALUNA_ADR);
	ret = find_dev_and_part(VMJALUNA_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", VMJALUNA_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", VMJALUNA_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	size = (VMJALUNA_SIZE +(flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("modem image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)VMJALUNA_ADR, &off);
	if(ret != 0) {
		printf("modem nand read error %d\n", ret);
		return;
	}
	secure_check(VMJALUNA_ADR, 0, VMJALUNA_ADR + VMJALUNA_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
#endif
	creat_cmdline(cmdline,hdr);
	vlx_entry();
}

