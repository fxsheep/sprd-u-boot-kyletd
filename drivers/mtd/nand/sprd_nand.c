#include <config.h>
#include <asm/arch/bits.h>
#include <asm/arch/regs_global.h>
#include <asm/arch/regs_ahb.h>
#include <asm/arch/regs_nfc.h>
#include <asm/arch/regs_cpc.h>
#include <nand.h>
#include <asm/io.h>
#include <linux/mtd/nand.h>

#define  CONFIG_MTD_NAND_SC8800S 1
#ifdef CONFIG_NAND_SPL
#define printf(arg...) do{}while(0)
#endif

#define NAND_DEBUG 1

#define NF_PARA_20M        	0x7ac05      //trwl = 0  trwh = 0
#define NF_PARA_40M        	0x7ac15      //trwl = 1  trwh = 0
#define NF_PARA_53M        	0x7ad26      //trwl = 2  trwh = 1
#define NF_PARA_80M        	0x7ad37      //trwl = 3  trwh = 1
#define NF_PARA_DEFAULT    	0x7ad77      //trwl = 7  trwh = 1
#define NF_TIMEOUT_VAL 		0x1000000

#define PAGE_SIZE_S         512
#define SPARE_SIZE_S        16
#define PAGE_SIZE_L         2048
#define SPARE_SIZE_L        64

#define BLOCK_TOTAL         1024
#define PAGEPERBLOCK	    64

#define REG_CPC_NFWPN				(*((volatile unsigned int *)(CPC_NFWPN_REG)))
#define REG_CPC_NFRB				(*((volatile unsigned int *)(CPC_NFRB_REG)))
#define REG_CPC_NFCLE                           (*((volatile unsigned int *)(CPC_NFCLE_REG)))
#define REG_CPC_NFALE				(*((volatile unsigned int *)(CPC_NFALE_REG)))
#define REG_CPC_NFCEN                           (*((volatile unsigned int *)(CPC_NFCEN_REG)))
#define REG_CPC_NFWEN                           (*((volatile unsigned int *)(CPC_NFWEN_REG)))
#define REG_CPC_NFREN                           (*((volatile unsigned int *)(CPC_NFREN_REG)))
#define REG_CPC_NFD0                            (*((volatile unsigned int *)(CPC_NFD0_REG)))
#define REG_CPC_NFD1                            (*((volatile unsigned int *)(CPC_NFD1_REG)))
#define REG_CPC_NFD2                            (*((volatile unsigned int *)(CPC_NFD2_REG)))
#define REG_CPC_NFD3                            (*((volatile unsigned int *)(CPC_NFD3_REG)))
#define REG_CPC_NFD4                            (*((volatile unsigned int *)(CPC_NFD4_REG)))
#define REG_CPC_NFD5                            (*((volatile unsigned int *)(CPC_NFD5_REG)))
#define REG_CPC_NFD6                            (*((volatile unsigned int *)(CPC_NFD6_REG)))
#define REG_CPC_NFD7                            (*((volatile unsigned int *)(CPC_NFD7_REG)))
#define REG_CPC_NFD8                            (*((volatile unsigned int *)(CPC_NFD8_REG)))

#define REG_CPC_NFD9                            (*((volatile unsigned int *)(CPC_NFD9_REG)))
#define REG_CPC_NFD10                           (*((volatile unsigned int *)(CPC_NFD10_REG)))
#define REG_CPC_NFD11                           (*((volatile unsigned int *)(CPC_NFD11_REG)))
#define REG_CPC_NFD12                           (*((volatile unsigned int *)(CPC_NFD12_REG)))
#define REG_CPC_NFD13                           (*((volatile unsigned int *)(CPC_NFD13_REG)))
#define REG_CPC_NFD14                           (*((volatile unsigned int *)(CPC_NFD14_REG)))
#define REG_CPC_NFD15                           (*((volatile unsigned int *)(CPC_NFD15_REG)))


#define REG_AHB_CTL0		       		(*((volatile unsigned int *)(AHB_CTL0)))

#define REG_GR_NFC_MEM_DLY                      (*((volatile unsigned int *)(GR_NFC_MEM_DLY)))

#define set_gpio_as_nand()                              \
do {                                                    \
        REG_CPC_NFWPN = BIT_0 | BIT_4 | BIT_5;          \
        REG_CPC_NFWPN &= ~(BIT_6 | BIT_7);              \
        REG_CPC_NFRB = BIT_0 | BIT_3 | BIT_4 | BIT_5;   \
        REG_CPC_NFRB &= ~(BIT_6 | BIT_7);               \
	REG_CPC_NFCLE |= BIT_4 | BIT_5;			\
	REG_CPC_NFCLE &= ~(BIT_6 | BIT_7);		\
	REG_CPC_NFALE |= BIT_4 | BIT_5;                 \
        REG_CPC_NFALE &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFCEN |= BIT_4 | BIT_5;                 \
        REG_CPC_NFCEN &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFWEN |= BIT_4 | BIT_5;                 \
        REG_CPC_NFWEN &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFREN |= BIT_4 | BIT_5;                 \
        REG_CPC_NFREN &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFD0 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD0 &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFD1 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD1 &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFD2 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD2 &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFD3 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD3 &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFD4 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD4 &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFD5 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD5 &= ~(BIT_6 | BIT_7);              \
	REG_CPC_NFD6 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD6 &= ~(BIT_6 | BIT_7);              \
        REG_CPC_NFD7 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD7 &= ~(BIT_6 | BIT_7);              \
       	REG_CPC_NFD8 |= BIT_4 | BIT_5;                 \
        REG_CPC_NFD8 &= ~(BIT_6 | BIT_7);              \
       	REG_CPC_NFD9 |= BIT_4 | BIT_5 | BIT_6;         \
        REG_CPC_NFD9 &= ~(BIT_7);                      \
       	REG_CPC_NFD10 |= BIT_4 | BIT_5 | BIT_6;        \
        REG_CPC_NFD10 &= ~(BIT_7);                     \
       	REG_CPC_NFD11 |= BIT_4 | BIT_5 | BIT_6;        \
        REG_CPC_NFD11 &= ~(BIT_7);                     \
       	REG_CPC_NFD12 |= BIT_4 | BIT_5 | BIT_6;        \
        REG_CPC_NFD12 &= ~(BIT_7);                     \
       	REG_CPC_NFD13 |= BIT_4 | BIT_5 | BIT_6;        \
        REG_CPC_NFD13 &= ~(BIT_7);                     \
       	REG_CPC_NFD14 |= BIT_4 | BIT_5 | BIT_6;        \
        REG_CPC_NFD14 &= ~(BIT_7);                     \
       	REG_CPC_NFD15 |= BIT_4 | BIT_5 | BIT_6;        \
        REG_CPC_NFD15 &= ~(BIT_7);                     \
} while (0)

struct sprd_platform_nand {
	/* timing information for nand flash controller */
	int	acs;
	int 	ach;
	int	rwl;
	int	rwh;
	int	rr;
	int	acr;
	int	ceh;
};

struct sprd_nand_address {
	int column;
	int row;
	int colflag;
	int rowflag;
};

struct sprd_nand_info {
	struct sprd_platform_nand	*platform;
	struct clk	*clk;
};

typedef enum {
	NO_OP,
	WRITE_OP,
	READ_OP,
} sprd_nand_wr_mode_t;

typedef enum {
	NO_AREA,
	DATA_AREA,
	OOB_AREA,
	DATA_OOB_AREA,
} sprd_nand_area_mode_t;

static struct mtd_info *sprd_mtd = NULL;
static unsigned long g_cmdsetting = 0;
static sprd_nand_wr_mode_t sprd_wr_mode = NO_OP;
static sprd_nand_area_mode_t sprd_area_mode = NO_AREA;
static unsigned long nand_flash_id = 0;
static struct sprd_nand_address sprd_colrow_addr = {0, 0, 0, 0};
static __attribute__((aligned(4))) unsigned char io_wr_port[NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE];
static nand_ecc_modes_t sprd_ecc_mode = NAND_ECC_NONE;

static void nand_copy(unsigned char *src, unsigned char *dst, unsigned long len)
{
	unsigned long i;
	unsigned long *pDst_32, *pSrc_32;
	unsigned short *pDst_16, *pSrc_16;
	unsigned long flag = 0;
	
	flag = (unsigned long *)dst;
	flag = flag & 0x3;

	switch (flag) {
		case 0://word alignment
#ifdef NAND_DEBUG
			printf("%s  %d\n", __FUNCTION__, __LINE__);
#endif
        		pDst_32 = (unsigned long *)dst;
                	pSrc_32 = (unsigned long *)src;
                	for (i = 0; i < (len / 4); i++) {
				*pDst_32 = *pSrc_32;
                    		pDst_32++;
                    		pSrc_32++;
			}
        	break;
        	case 2://half word alignment
#ifdef NAND_DEBUG
			printf("%s  %d\n", __FUNCTION__, __LINE__);
#endif
                	pDst_16 = (unsigned short *)dst;
                	pSrc_16 = (unsigned short *)src;
                	for (i = 0; i < (len / 2); i++) {
                    		*pDst_16 = *pSrc_16;
                    		pDst_16++;
                    		pSrc_16++;
                	}
            	break;
        	default://byte alignment
#ifdef NAND_DEBUG
			printf("%s  %d\n", __FUNCTION__, __LINE__);
#endif
                	for (i = 0; i < len; i++) {
                    		*dst = *src;
                    		dst++;
                    		src++;
                	}
            	break;
    	}//switch	
}

#ifdef CONFIG_NAND_SPL
static u_char nand_read_byte16(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
#ifdef CONFIG_MTD_NAND_SC8800S
	return (uint8_t)readl(this->IO_ADDR_R);
#else
	return readb(this->IO_ADDR_R);
#endif
}

static void nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;

#ifdef CONFIG_MTD_NAND_SC8800S
	nand_copy(buf,this->IO_ADDR_W, len);
#else
	for (i = 0; i < len; i++)
		writeb(buf[i], this->IO_ADDR_W);
#endif
}

static void nand_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;
#ifdef CONFIG_MTD_NAND_SC8800S
	nand_copy(this->IO_ADDR_R, buf, len);
#else
	for (i = 0; i < len; i++)
		buf[i] = readb(this->IO_ADDR_R);
#endif
}
#endif
//static unsigned long g_CmdSetting;
static int nfc_wait_command_finish(void)
{
	unsigned long nfc_cmd = REG_NFC_CMD;
	unsigned long counter = 0;
	
	while ((nfc_cmd & (0x1 << 31)) && (counter < NF_TIMEOUT_VAL)) {
		nfc_cmd = REG_NFC_CMD;
		counter++;
	}
	
	if (NF_TIMEOUT_VAL == counter) {
		return 2;
	}
	
	return 0;
}

static void set_nfc_param(unsigned long ahb_clk)
{
	nfc_wait_command_finish();
	
	switch (ahb_clk) {
	case 20:
        	REG_NFC_PARA = NF_PARA_20M;
        break;

        case 40:
        	REG_NFC_PARA = NF_PARA_40M;
        break;

        case 53:
        	REG_NFC_PARA = NF_PARA_53M;
        break;

        case 80:
        	REG_NFC_PARA = NF_PARA_80M;
        break;

        default:
             	REG_NFC_PARA = NF_PARA_DEFAULT;    
    	}	
}


static void memset(unsigned char * s, unsigned char c, unsigned long len)
{
	unsigned long i = 0;
	if(len < 1)
		return;
	for(i = 0; i<len; i++)
	{
		*(s+i) = c;
	}
}

static void sprd_nand_hwcontrol(struct mtd_info *mtd, int cmd,
				   unsigned int ctrl)
{
	unsigned long phyblk, pageinblk, pageperblk;
	unsigned long i;
	unsigned long addr_cycle = 5; /* advance 0 : can be set 3 or 4; advance 1: can be set 4 or 5 */
	unsigned long advance = 1; /* can be set 0 or 1 */
	unsigned long pagetype; /* 0: small page; 1: large page*/
	unsigned long buswidth = 1; /* 0: X8 bus width 1: X16 bus width */
	unsigned long chipsel = 0;
	//static unsigned long readaaa = 0;
	struct nand_chip *this = (struct nand_chip *)(mtd->priv);
	if (cmd == NAND_CMD_NONE)
		return;

	if (512 == mtd->writesize)
		pagetype = 0;
   	else
   	    	pagetype = 1;
	if(addr_cycle == 3)
   		addr_cycle = 0;
   	else if((addr_cycle == 4) &&(advance == 1))
   	    	addr_cycle = 0;
   	else
   	    	addr_cycle = 3;

	if (ctrl & NAND_CLE) {
		switch (cmd) {
		case NAND_CMD_RESET:
			REG_NFC_CMD = cmd | (0x1 << 31);
			nfc_wait_command_finish();
		break;
		case NAND_CMD_STATUS:
			REG_NFC_CMD = cmd | (0x1 << 31);
			nfc_wait_command_finish();
			
			memset((unsigned char *)(this->IO_ADDR_R), 0xff, NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE);
			nand_copy((unsigned char *)NFC_IDSTATUS, this->IO_ADDR_R, 4);
#if 0 
			/* transfer to big endian */
			i = io_wr_port[3]; io_wr_port[3] = io_wr_port[0]; io_wr_port[0] = i;
			i = io_wr_port[2]; io_wr_port[2] = io_wr_port[1]; io_wr_port[1] = i;
#endif
#ifdef NAND_DEBUG
			for (i = 0; i < 4; i++)
                 			printf("io_wr_port[%d] = 0x%02x\n", i, io_wr_port[i]);
#endif
		break;
		case NAND_CMD_READID:
			REG_NFC_CMD = cmd | (0x1 << 31);
			nfc_wait_command_finish();
			nand_flash_id = REG_NFC_IDSTATUS;
#ifdef NAND_DEBUG
			printf("nand id: %x\n", nand_flash_id);
#endif
		break;
		case NAND_CMD_ERASE1:
			sprd_colrow_addr.column = 0;
			sprd_colrow_addr.row = 0;
			sprd_colrow_addr.colflag = 0;
			sprd_colrow_addr.rowflag = 0;
		break;
		case NAND_CMD_ERASE2:
			if ((0 == sprd_colrow_addr.colflag) && (0 == sprd_colrow_addr.rowflag)) {
				printf("erase address error!\n");
				return;
			} else {
				if (1 == sprd_colrow_addr.colflag) {
					sprd_colrow_addr.row = sprd_colrow_addr.column;
					sprd_colrow_addr.column = 0;
					sprd_colrow_addr.rowflag = 1;
					sprd_colrow_addr.colflag = 0;	
				}	
			}
			
			if ((0 == sprd_colrow_addr.colflag) && (1 == sprd_colrow_addr.rowflag)) {
				g_cmdsetting = (chipsel << 26) | (addr_cycle << 24) | (advance << 23) | (buswidth << 19) | (pagetype << 18) | (0 << 16) | (0x1 << 31);

				REG_NFC_STR0 = sprd_colrow_addr.row * mtd->writesize;
REG_NFC_CMD = g_cmdsetting | NAND_CMD_ERASE1;
				nfc_wait_command_finish();
			}
		break;
		case NAND_CMD_READ0:
			sprd_colrow_addr.column = 0;
			sprd_colrow_addr.row = 0;
			sprd_colrow_addr.colflag = 0;
			sprd_colrow_addr.rowflag = 0;
			sprd_wr_mode = READ_OP;
			sprd_area_mode = NO_AREA;
memset(io_wr_port, 0xff, NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE);
		break;
		case NAND_CMD_READSTART:
			if (sprd_colrow_addr.column == (mtd->writesize >> 1)) {
				g_cmdsetting = (chipsel << 26) | (addr_cycle << 24) | (advance << 23) | (buswidth << 19) | (pagetype << 18) | (0 << 16) | (0x1 << 31);
				REG_NFC_CMD = g_cmdsetting | NAND_CMD_READ0;
				nfc_wait_command_finish();
				nand_copy((unsigned long *)NFC_SBUF, (unsigned long *)io_wr_port, mtd->oobsize);

			} else if (sprd_colrow_addr.column == 0) {
if (sprd_area_mode == DATA_AREA)
						sprd_area_mode = DATA_OOB_AREA;

					if (sprd_area_mode == DATA_OOB_AREA) {
						REG_NFC_END0 = 0xffffffff;
						g_cmdsetting = (chipsel << 26) | (addr_cycle << 24) | (advance << 23) | (1 << 21) | (buswidth << 19) | (pagetype << 18) | (0 << 16) | (0x1 << 31);
						REG_NFC_CMD = g_cmdsetting | NAND_CMD_READ0;
						nfc_wait_command_finish();
						nand_copy((unsigned char *)NFC_MBUF, io_wr_port, mtd->writesize);
					} else if (sprd_colrow_addr.column == DATA_AREA) {
						g_cmdsetting = (addr_cycle << 24) | (advance << 23) | (buswidth << 19) | (pagetype << 18) | (0 << 16) | (0x1 << 31);
						REG_NFC_CMD = g_cmdsetting | NAND_CMD_READ0;
						nfc_wait_command_finish();
			
						nand_copy((unsigned char *)NFC_MBUF, io_wr_port, mtd->writesize);
				}
			} else
				printf("Operation !!! area.  %s  %s  %d\n", __FILE__, __FUNCTION__, __LINE__);
			sprd_wr_mode = NO_OP;
			sprd_area_mode = NO_AREA;
		break;
		case NAND_CMD_SEQIN:
			sprd_colrow_addr.column = 0;
			sprd_colrow_addr.row = 0;
			sprd_colrow_addr.colflag = 0;
			sprd_colrow_addr.rowflag = 0;
			sprd_wr_mode = WRITE_OP;
			sprd_area_mode = NO_AREA;
			memset(io_wr_port, 0xff, NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE);
		break;
		case NAND_CMD_PAGEPROG:
			if (sprd_colrow_addr.column == (mtd->writesize >> 1)) {
				g_cmdsetting = (chipsel << 26) | (addr_cycle << 24) | (advance << 23) |	(buswidth << 19) | (pagetype << 18) | (0 << 16) | (0x1 << 31);
		nand_copy((unsigned long *)io_wr_port, (unsigned long *)NFC_SBUF, mtd->oobsize);
				REG_NFC_CMD = g_cmdsetting | NAND_CMD_SEQIN;
				nfc_wait_command_finish();
			} else if (sprd_colrow_addr.column == 0) {

				if (sprd_area_mode == DATA_OOB_AREA) {
					REG_NFC_END0 = 0xffffffff;
					g_cmdsetting = (chipsel << 26) | (addr_cycle << 24) | (advance << 23) | (buswidth << 19) | (pagetype << 18) | (0 << 16) | (0x1 << 31);

			
					REG_NFC_CMD = g_cmdsetting | NAND_CMD_SEQIN;
					nfc_wait_command_finish();
				
				} else if (sprd_colrow_addr.column == DATA_AREA) {
					g_cmdsetting = (chipsel << 26) | (addr_cycle << 24) | (advance << 23) | (buswidth << 19) | (pagetype << 18) | (0 << 16) | (0x1 << 31);
					nand_copy(io_wr_port, (unsigned char *)NFC_MBUF, mtd->writesize);
					REG_NFC_CMD = g_cmdsetting | NAND_CMD_SEQIN;
					nfc_wait_command_finish();
				}
			} else
				printf("Operation !!! area.  %s  %s  %d\n", __FILE__, __FUNCTION__, __LINE__);
			sprd_wr_mode = NO_OP;
			sprd_area_mode = NO_AREA;
		break;
		default:
		break;	
		}
	} else {
		if (0 == sprd_colrow_addr.colflag) {
			sprd_colrow_addr.colflag = 1;
			sprd_colrow_addr.column = cmd;
			return;
		}
		
		if (0 == sprd_colrow_addr.rowflag) {
			sprd_colrow_addr.rowflag = 1;
			sprd_colrow_addr.row = cmd;
		}
		
		if ((1 == sprd_colrow_addr.colflag) && (1 == sprd_colrow_addr.rowflag)) {
			if (sprd_colrow_addr.column == (mtd->writesize >> 1)) {
				pageperblk = mtd->erasesize / mtd->writesize;

				phyblk = sprd_colrow_addr.row / pageperblk;
        			pageinblk = sprd_colrow_addr.row % pageperblk;
        			REG_NFC_STR0 = phyblk * pageperblk * mtd->writesize + 
							pageinblk * mtd->writesize + 
							sprd_colrow_addr.column;
        			REG_NFC_END0 = phyblk * pageperblk * mtd->writesize + 
							pageinblk * mtd->writesize + 
							sprd_colrow_addr.column + (mtd->oobsize >> 1) -1;
				sprd_area_mode = OOB_AREA;	

			} else if (sprd_colrow_addr.column == 0) {
				pageperblk = mtd->erasesize / mtd->writesize;
				phyblk = sprd_colrow_addr.row / pageperblk;
        			pageinblk = sprd_colrow_addr.row % pageperblk;
        			REG_NFC_STR0 = phyblk * pageperblk * mtd->writesize + 
							pageinblk * mtd->writesize + 
							sprd_colrow_addr.column;
        			REG_NFC_END0 = phyblk * pageperblk * mtd->writesize  + 
							pageinblk * mtd->writesize + 
							sprd_colrow_addr.column + (mtd->writesize >> 1) - 1;
				sprd_area_mode = DATA_AREA;	
			} else
				printf("Operation ??? area.  %s  %s  %d\n", __FILE__, __FUNCTION__, __LINE__);
		}
	}		
}

static unsigned long sprd_nand_readid(struct mtd_info *mtd)
{
	return(nand_flash_id);
}

static int sprd_nand_devready(struct mtd_info *mtd)
{
	unsigned long status = 0;
        unsigned long cmd = NAND_CMD_STATUS | (0x1 << 31);

        REG_NFC_CMD = cmd;
        nfc_wait_command_finish();

        status = REG_NFC_IDSTATUS;
   	if ((status & 0x1) != 0) 	
     		return -1; /* fail */
   	else if ((status & 0x20) == 0)
     		return 0; /* busy */
   	else 							
     		return 1; /* ready */
}
static void sprd_nand_select_chip(struct mtd_info *mtd, int chip)
{
	struct nand_chip *this = mtd->priv;
	struct sprd_nand_info *info = this->priv;

#if 0	
	if (chip != -1)
		clk_enable(info->clk);
	else
		clk_disable(info->clk);
#endif
}

static void sprd_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
	sprd_ecc_mode = mode;
}

static unsigned long sprd_nand_wr_oob(struct mtd_info *mtd)
{
	unsigned int i;
        nand_copy(io_wr_port, (unsigned char *)NFC_SBUF, mtd->oobsize);
	printf("%s\n", __FUNCTION__);
	for(i=0;i<64;i++) {
		printf("%x ", io_wr_port[i]);
		if(i!=0 && i%8==7)
		  printf("\n");
	}
	/* write oob area */
	if (sprd_area_mode == NO_AREA)
		sprd_area_mode = OOB_AREA;
	else if (sprd_area_mode == DATA_AREA)
		sprd_area_mode = DATA_OOB_AREA;

	return 0;
}

void nand_ecc_trans(unsigned char *pEccIn, unsigned char *pEccOut, unsigned char nSct)
{
#if 0
	/* little endian */
        switch(nSct)
        {
           case 1:
                 pEccOut[0] = pEccIn[0];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[2];
                 break;
           case 2:
                 pEccOut[0] = pEccIn[0];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[2];
                 pEccOut[4] = pEccIn[4];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[6];
                 break;
           case 3:
                 pEccOut[0] = pEccIn[0];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[2];
                 pEccOut[4] = pEccIn[4];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[6];
                 pEccOut[8] = pEccIn[8];
                 pEccOut[9] = pEccIn[9];
                 pEccOut[10] = pEccIn[10];
                 break;
           case 4:
                 pEccOut[0] = pEccIn[0];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[2];
                 pEccOut[4] = pEccIn[4];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[6];
                 pEccOut[8] = pEccIn[8];
                 pEccOut[9] = pEccIn[9];
                 pEccOut[10] = pEccIn[10];
                 pEccOut[12] = pEccIn[12];
                 pEccOut[13] = pEccIn[13];
                 pEccOut[14] = pEccIn[14];
                 break;
           default:
                 break;     
        }
#else
	/* big endian */
        switch(nSct)
        {
           case 1:
                 pEccOut[0] = pEccIn[2];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[3];
                 break;
           case 2:
                 pEccOut[0] = pEccIn[2];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[3];
                 pEccOut[4] = pEccIn[6];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[7];
                 break;
           case 3:
                 pEccOut[0] = pEccIn[2];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[3];
                 pEccOut[4] = pEccIn[6];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[7];
                 pEccOut[8] = pEccIn[10];
                 pEccOut[9] = pEccIn[9];
                 pEccOut[10] = pEccIn[11];
                 break;
           case 4:
                 pEccOut[0] = pEccIn[2];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[3];
                 pEccOut[4] = pEccIn[6];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[7];
                 pEccOut[8] = pEccIn[10];
                 pEccOut[9] = pEccIn[9];
                 pEccOut[10] = pEccIn[11];
                 pEccOut[12] = pEccIn[14];
                 pEccOut[13] = pEccIn[13];
                 pEccOut[14] = pEccIn[15];
                 break;
           default:
                 break;     
        }
#endif
}

static int sprd_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	unsigned char ecc_val_in[16];
	unsigned char ecc_val_out[16];
        unsigned long *pecc_val;
	unsigned int i, j;

	if (sprd_ecc_mode == NAND_ECC_WRITE) {
		pecc_val = (unsigned long *)ecc_val_in;

		REG_NFC_ECCEN = 0x1;
		nand_copy(io_wr_port, (unsigned char *)NFC_MBUF, mtd->writesize);
		/* large page */
		pecc_val[0] = REG_NFC_PAGEECC0;
		pecc_val[1] = REG_NFC_PAGEECC1;
		pecc_val[2] = REG_NFC_PAGEECC2;
		pecc_val[3] = REG_NFC_PAGEECC3;
#ifdef NAND_DEBUG
		for(i = 0; i< 4; i++)
			printf("write ecc %d is %x\n", i, pecc_val[i]);
#endif
		nand_ecc_trans(ecc_val_in, ecc_val_out, 4);

		ecc_code[0] = ecc_val_out[0];
		ecc_code[1] = ecc_val_out[1];
		ecc_code[2] = ecc_val_out[2];

		ecc_code[3] = ecc_val_out[4];
		ecc_code[4] = ecc_val_out[5];
		ecc_code[5] = ecc_val_out[6];

		ecc_code[6] = ecc_val_out[8];
		ecc_code[7] = ecc_val_out[9];
		ecc_code[8] = ecc_val_out[10];

		ecc_code[9] = ecc_val_out[12];
		ecc_code[10] = ecc_val_out[13];
		ecc_code[11] = ecc_val_out[14];

		REG_NFC_ECCEN = 0;
		memset(io_wr_port, 0xff, NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE);	
	} else if (sprd_ecc_mode == NAND_ECC_READ) {
 		pecc_val = (unsigned long *)ecc_val_in;
                /* large page */
                pecc_val[0] = REG_NFC_PAGEECC0;
                pecc_val[1] = REG_NFC_PAGEECC1;
                pecc_val[2] = REG_NFC_PAGEECC2;
                pecc_val[3] = REG_NFC_PAGEECC3;
#ifdef NAND_DEBUG
		for(i = 0; i< 4; i++)
			printf("read ecc %d is %x\n", i, pecc_val[i]);
#endif
		nand_ecc_trans(ecc_val_in, ecc_val_out, 4);

		ecc_code[0] = ecc_val_out[0];
		ecc_code[1] = ecc_val_out[1];
		ecc_code[2] = ecc_val_out[2];

		ecc_code[3] = ecc_val_out[4];
		ecc_code[4] = ecc_val_out[5];
		ecc_code[5] = ecc_val_out[6];

		ecc_code[6] = ecc_val_out[8];
		ecc_code[7] = ecc_val_out[9];
		ecc_code[8] = ecc_val_out[10];

		ecc_code[9] = ecc_val_out[12];
		ecc_code[10] = ecc_val_out[13];
		ecc_code[11] = ecc_val_out[14];

                memset(io_wr_port, 0xff, NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE);
		nand_copy((unsigned char *)NFC_SBUF, io_wr_port, mtd->oobsize);
#ifdef NAND_DEBUG
		printf("read oob \n");
		for(i = 0; i<64; i++){
			printf("%x ", io_wr_port[i]);
			if(i!=0 && i%8 == 7)
			  printf("\n");
		}
#endif
	}	
	sprd_ecc_mode = NAND_ECC_NONE;

	return 0;
}

static int countbits(unsigned long byte)
{
        int res = 0;

        for (;byte; byte >>= 1)
                res += byte & 0x01;
        return res;
}

static int ECC_CompM(unsigned char *pEcc1, unsigned char *pEcc2, unsigned char *pBuf, unsigned char nBW)
{
	unsigned long  nEccComp = 0, nEccSum = 0;
	unsigned long  nEBit    = 0;
	unsigned long  nEByte   = 0;
	unsigned long  nXorT1   = 0, nXorT2 = 0;
	unsigned long  nCnt;

	for (nCnt = 0; nCnt < 2; nCnt++) {
        	nXorT1 ^= (((*pEcc1) >> nCnt) & 0x01);
        	nXorT2 ^= (((*pEcc2) >> nCnt) & 0x01);
    	}

    	for (nCnt = 0; nCnt < 3; nCnt++) {
        	nEccComp |= ((~pEcc1[nCnt] ^ ~pEcc2[nCnt]) << (nCnt * 8));
    	}
    	for(nCnt = 0; nCnt < 24; nCnt++) {
        	nEccSum += ((nEccComp >> nCnt) & 0x01);
    	}
    	switch (nEccSum) {
	case 0 :
            	return 0;
        case 1 :
             	return 1;
        case 12 :
            	if (nXorT1 != nXorT2) {
                	if (nBW == 0) {
                    		nEByte  = ((nEccComp >>  9) & 0x100) +
                              		((nEccComp >>  8) & 0x80) + ((nEccComp >>  7) & 0x40) +
                              		((nEccComp >>  6) & 0x20) + ((nEccComp >>  5) & 0x10) +
                              		((nEccComp >>  4) & 0x08) + ((nEccComp >>  3) & 0x04) +
                              		((nEccComp >>  2) & 0x02) + ((nEccComp >>  1) & 0x01);
                    		nEBit   = ((nEccComp >> 21) & 0x04) +
                              		((nEccComp >> 20) & 0x02) + ((nEccComp >> 19) & 0x01);
                	} else {   /* (nBW == BW_X16) */
                    		nEByte  = ((nEccComp >>  7) & 0x100) +
                              		((nEccComp >>  6) & 0x80) + ((nEccComp >>  5) & 0x40) +
                              		((nEccComp >>  4) & 0x20) + ((nEccComp >>  3) & 0x10) +
                              		((nEccComp >>  2) & 0x08) + ((nEccComp >>  1) & 0x04) +
                              		(nEccComp & 0x02)         + ((nEccComp >> 23) & 0x01);
                    		nEBit   = (unsigned char)(((nEccComp >> 19) & 0x04) +
                              		((nEccComp >> 18) & 0x02) + ((nEccComp >> 17) & 0x01));
                	}
                	if (pBuf != NULL) {
                        		pBuf[nEByte] = (unsigned char)(pBuf[nEByte] ^ (1 << nEBit));
                	}
                	return 1;
            	}
        default :
 
           break;
    	}
	return -1;
}

static int correct(u_char *dat, u_char *read_ecc, u_char *calc_ecc)
{
	return ECC_CompM(read_ecc, calc_ecc, dat, 1);
}

static int sprd_nand_correct_data(struct mtd_info *mtd, uint8_t *dat,
				     uint8_t *read_ecc, uint8_t *calc_ecc)
{
	int i, retval = 0;

	if (mtd->writesize > 512) {
		for (i = 0; i < 4; i++) {
			if (correct(dat + 512 * i, read_ecc + 3 * i, calc_ecc + 3 * i) == -1) {				
				retval = -1;
			}
		}
	} else
		retval = correct(dat, read_ecc, calc_ecc);
	
	return retval;
}

static struct sprd_nand_info g_info = {0,0};
static struct sprd_platform_nand g_plat = {0};

int board_nand_init(struct nand_chip *this)
{
	struct mtd_info *mtd;
	
	/*structs must be linked */
	//mtd = &host->mtd;
	mtd->priv = this;
	//host->nand = this;
	
	g_info.platform = &g_plat;
		/* set sprd_colrow_addr */
	sprd_colrow_addr.column = 0;
	sprd_colrow_addr.row = 0;
	sprd_colrow_addr.colflag = 0;
	sprd_colrow_addr.rowflag = 0;
	sprd_wr_mode = NO_OP;
	sprd_area_mode = NO_AREA;
	sprd_ecc_mode = NAND_ECC_NONE;

	REG_AHB_CTL0 |= BIT_8 | BIT_9;//no BIT_9
	REG_NFC_INTSRC |= BIT_0 | BIT_4 | BIT_5;
	/* 0x1 : WPN disable, and micron nand flash status is 0xeo 
 *  	   0x0 : WPN enable, and micron nand flash status is 0x60 */
	REG_NFC_WPN = 0x1;
	
	set_gpio_as_nand();
	REG_GR_NFC_MEM_DLY = 0x0;
	set_nfc_param(0);//53MHz
	memset(io_wr_port, 0xff, NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE);

/* set the timing for nand controller */
	/* 16-bit bus width */
	this->IO_ADDR_R = this->IO_ADDR_W = io_wr_port;
	this->options |= NAND_BUSWIDTH_16;
	this->cmd_ctrl = sprd_nand_hwcontrol;
	this->dev_ready = sprd_nand_devready;
	this->select_chip = sprd_nand_select_chip;

#ifdef CONFIG_NAND_SPL
	this->read_byte	= nand_read_byte16;
	this->write_buf = nand_write_buf;
	this->read_buf  = nand_read_buf;
#endif
	this->nfc_readid = sprd_nand_readid;
	this->nfc_wr_oob = sprd_nand_wr_oob;

#ifdef CONFIG_SPRD_NAND_HWECC
	this->ecc.calculate = sprd_nand_calculate_ecc;
	this->ecc.correct = sprd_nand_correct_data;
	this->ecc.hwctl = sprd_nand_enable_hwecc;
	this->ecc.mode = NAND_ECC_HW;
	this->ecc.size = CONFIG_SYS_NAND_ECCSIZE;//512;
	this->ecc.bytes = CONFIG_SYS_NAND_ECCBYTES;//3
#else
	this->ecc.mode = NAND_ECC_SOFT;
#endif

	this->chip_delay = 20;
	this->priv = &g_info;
	return 0;
}
