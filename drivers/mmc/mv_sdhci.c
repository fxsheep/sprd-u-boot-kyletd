#include <common.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/io.h>
#include <asm/arch/regs_global.h>

#define MAX_DUMP_NUM 14
#define DRIVER_NAME "sdhci"
#define KERN_DEBUG " "

void sdhci_dumpregs(struct sdhci_host *host)
{
	int i = 0;
	printf(KERN_DEBUG DRIVER_NAME ": ============== REGISTER DUMP ==============\n");
	printf(KERN_DEBUG DRIVER_NAME ":AHB CTRL 0x%08x\n", *((unsigned int*)0x20900200));
#if 1
	for(i = 0; i < MAX_DUMP_NUM; i++){
		printf(KERN_DEBUG DRIVER_NAME ": address 0x%08x | value 0x%08x\n", host->ioaddr + i*4, sdhci_readl(host, i*4));
	}
#endif

	printf(KERN_DEBUG DRIVER_NAME ": Sys addr: 0x%08x | Version:  0x%08x\n",
			sdhci_readl(host, SDHCI_DMA_ADDRESS),
			sdhci_readw(host, SDHCI_HOST_VERSION));
	printf(KERN_DEBUG DRIVER_NAME ": Blk size: 0x%08x | Blk cnt:  0x%08x\n",
			sdhci_readw(host, SDHCI_BLOCK_SIZE),
			sdhci_readw(host, SDHCI_BLOCK_COUNT));
	printf(KERN_DEBUG DRIVER_NAME ": Argument: 0x%08x | Trn mode: 0x%08x\n",
			sdhci_readl(host, SDHCI_ARGUMENT),
			sdhci_readw(host, SDHCI_TRANSFER_MODE));
	printf(KERN_DEBUG DRIVER_NAME ": Present:  0x%08x | Host ctl: 0x%08x\n",
			sdhci_readl(host, SDHCI_PRESENT_STATE),
			sdhci_readb(host, SDHCI_HOST_CONTROL));
	printf(KERN_DEBUG DRIVER_NAME ": Power:    0x%08x | Blk gap:  0x%08x\n",
			sdhci_readb(host, SDHCI_POWER_CONTROL),
			sdhci_readb(host, SDHCI_BLOCK_GAP_CONTROL));
	printf(KERN_DEBUG DRIVER_NAME ": Wake-up:  0x%08x | Clock:    0x%08x\n",
			sdhci_readb(host, SDHCI_WAKE_UP_CONTROL),
			sdhci_readw(host, SDHCI_CLOCK_CONTROL));
	printf(KERN_DEBUG DRIVER_NAME ": Timeout:  0x%08x | Int stat: 0x%08x\n",
			sdhci_readb(host, SDHCI_TIMEOUT_CONTROL),
			sdhci_readl(host, SDHCI_INT_STATUS));
	printf(KERN_DEBUG DRIVER_NAME ": Int enab: 0x%08x | Sig enab: 0x%08x\n",
			sdhci_readl(host, SDHCI_INT_ENABLE),
			sdhci_readl(host, SDHCI_SIGNAL_ENABLE));
	printf(KERN_DEBUG DRIVER_NAME ": AC12 err: 0x%08x | Slot int: 0x%08x\n",
			sdhci_readw(host, SDHCI_ACMD12_ERR),
			sdhci_readw(host, SDHCI_SLOT_INT_STATUS));
	printf(KERN_DEBUG DRIVER_NAME ": Caps:     0x%08x | Max curr: 0x%08x\n",
			sdhci_readl(host, SDHCI_CAPABILITIES),
			sdhci_readl(host, SDHCI_MAX_CURRENT));

		printf(KERN_DEBUG DRIVER_NAME ": ADMA Err: 0x%08x | ADMA Ptr: 0x%08x\n",
				readl(host->ioaddr + SDHCI_ADMA_ERROR),
				readl(host->ioaddr + SDHCI_ADMA_ADDRESS));
	printf(KERN_DEBUG DRIVER_NAME ": ===========================================\n");
}

#define SDIO_MAX_CLK  SDIO_BASE_CLK_96M  //40Mhz

#define __raw_bits_and(v, a) writel((readl(a)&v), a)
#define __raw_bits_or(v, a) writel((readl(a)|v), a)
static void sdhci_sprd_set_base_clock(unsigned int clock)
{
	unsigned long flags;

	/* don't bother if the clock is going off. */
	if (clock == 0)
		return;

	if (clock > SDIO_MAX_CLK)
		clock = SDIO_MAX_CLK;

	//Select the clk source of SDIO
	__raw_bits_and(~(BIT_17|BIT_18), GR_CLK_GEN5);

	if (clock >= SDIO_BASE_CLK_96M) {
		//default is 96M
		;
	} else if (clock >= SDIO_BASE_CLK_64M) {
		__raw_bits_or((1<<17), GR_CLK_GEN5);
	} else if (clock >= SDIO_BASE_CLK_48M) {
		__raw_bits_or((2<<17), GR_CLK_GEN5);
	} else {
		__raw_bits_or((3<<17), GR_CLK_GEN5);
	}

	printf("after set sd clk, CLK_GEN5:%x\r\n",
			__raw_readl(GR_CLK_GEN5));
	return;
}
	                                                                                                                                                                                                                                                    

#ifdef CONFIG_MMC_SDHCI_IO_ACCESSORS
static struct sdhci_ops mv_ops;

#if defined(CONFIG_SHEEVA_88SV331xV5)
#define SD_CE_ATA_2	0xEA
#define  MMC_CARD	0x1000
#define  MMC_WIDTH	0x0100
static inline void mv_sdhci_writeb(struct sdhci_host *host, u8 val, int reg)
{
	struct mmc *mmc = host->mmc;
	u32 ata = (u32)host->ioaddr + SD_CE_ATA_2;

	if (!IS_SD(mmc) && reg == SDHCI_HOST_CONTROL) {
		if (mmc->bus_width == 8)
			writew(readw(ata) | (MMC_CARD | MMC_WIDTH), ata);
		else
			writew(readw(ata) & ~(MMC_CARD | MMC_WIDTH), ata);
	}

	writeb(val, host->ioaddr + reg);
}

#else
#define mv_sdhci_writeb	NULL
#endif /* CONFIG_SHEEVA_88SV331xV5 */
#endif /* CONFIG_MMC_SDHCI_IO_ACCESSORS */

static char *MVSDH_NAME = "mv_sdh";
int mv_sdh_init(u32 regbase, u32 max_clk, u32 min_clk, u32 quirks)
{
	struct sdhci_host *host = NULL;
	host = (struct sdhci_host *)malloc(sizeof(struct sdhci_host));
	if (!host) {
		printf("sdh_host malloc fail!\n");
		return 1;
	}

	host->name = MVSDH_NAME;
	host->ioaddr = (void *)regbase;
	host->quirks = quirks;
#ifdef CONFIG_MMC_SDHCI_IO_ACCESSORS
	memset(&mv_ops, 0, sizeof(struct sdhci_ops));
	mv_ops.write_b = mv_sdhci_writeb;
	host->ops = &mv_ops;
#endif
	if (quirks & SDHCI_QUIRK_REG32_RW)
		host->version = sdhci_readl(host, SDHCI_HOST_VERSION - 2) >> 16;
	else
		host->version = sdhci_readw(host, SDHCI_HOST_VERSION);
	sdhci_sprd_set_base_clock(375000);
	add_sdhci(host, max_clk, min_clk);
	return 0;
}
