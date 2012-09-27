#include <linux/time.h>
#include <linux/ctype.h>



#define MAXU16	  0xffff	/* maximum unsigned 16-bit quantity */
#define BPN	  4		/* bits per nibble */
#define NPB	  2		/* nibbles per byte */

#define DOSMAGIC  0xaa55	/* DOS magic number */
#define MINBPS	  512		/* minimum bytes per sector */
#define MAXSPC	  128		/* maximum sectors per cluster */
#define MAXNFT	  16		/* maximum number of FATs */
#define DEFBLK	  4096		/* default block size */
#define DEFBLK16  2048		/* default block size FAT16 */
#define DEFRDE	  512		/* default root directory entries */
#define RESFTE	  2		/* reserved FAT entries */
#define MINCLS12  1		/* minimum FAT12 clusters */
#define MINCLS16  0x1000	/* minimum FAT16 clusters */
#define MINCLS32  2		/* minimum FAT32 clusters */
#define MAXCLS12  0xfed 	/* maximum FAT12 clusters */
#define MAXCLS16  0xfff5	/* maximum FAT16 clusters */
#define MAXCLS32  0xffffff5	/* maximum FAT32 clusters */
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define powerof2(x)     ((((x) - 1) & (x)) == 0)
#define howmany(x, y)   (((x) + ((y) - 1)) / (y))
#define MAX(x, y)  ((x) > (y) ? (x) : (y))
#define mincls(fat)  ((fat) == 12 ? MINCLS12 :	\
		      (fat) == 16 ? MINCLS16 :	\
				    MINCLS32)

#define maxcls(fat)  ((fat) == 12 ? MAXCLS12 :	\
		      (fat) == 16 ? MAXCLS16 :	\
				    MAXCLS32)

#define mk1(p, x)				\
    (p) = (unsigned char)(x)

#define mk2(p, x)				\
    (p)[0] = (unsigned char)(x),			\
    (p)[1] = (unsigned char)((x) >> 010)

#define mk4(p, x)				\
    (p)[0] = (unsigned char)(x),			\
    (p)[1] = (unsigned char)((x) >> 010),		\
    (p)[2] = (unsigned char)((x) >> 020),		\
    (p)[3] = (unsigned char)((x) >> 030)

struct timeval {
	long		tv_sec;		/* seconds */
	long		tv_usec;	/* microseconds */
};

struct bs {
    unsigned char jmp[3];		/* bootstrap entry point */
    unsigned char oem[8];		/* OEM name and version */
};

struct bsbpb {
    unsigned char bps[2];		/* bytes per sector */
    unsigned char spc;		/* sectors per cluster */
    unsigned char res[2];		/* reserved sectors */
    unsigned char nft;		/* number of FATs */
    unsigned char rde[2];		/* root directory entries */
    unsigned char sec[2];		/* total sectors */
    unsigned char mid;		/* media descriptor */
    unsigned char spf[2];		/* sectors per FAT */
    unsigned char spt[2];		/* sectors per track */
    unsigned char hds[2];		/* drive heads */
    unsigned char hid[4];		/* hidden sectors */
    unsigned char bsec[4];		/* big total sectors */
};

struct bsxbpb {
    unsigned char bspf[4];		/* big sectors per FAT */
    unsigned char xflg[2];		/* FAT control flags */
    unsigned char vers[2];		/* file system version */
    unsigned char rdcl[4];		/* root directory start cluster */
    unsigned char infs[2];		/* file system info sector */
    unsigned char bkbs[2];		/* backup boot sector */
    unsigned char rsvd[12];		/* reserved */
};

struct bsx {
    unsigned char drv;		/* drive number */
    unsigned char rsvd;		/* reserved */
    unsigned char sig;		/* extended boot signature */
    unsigned char volid[4];		/* volume ID number */
    unsigned char label[11]; 	/* volume label */
    unsigned char type[8];		/* file system type */
};

struct de {
    unsigned char namext[11];	/* name and extension */
    unsigned char attr;		/* attributes */
    unsigned char rsvd[10];		/* reserved */
    unsigned char time[2];		/* creation time */
    unsigned char date[2];		/* creation date */
    unsigned char clus[2];		/* starting cluster */
    unsigned char size[4];		/* size */
};

struct bpb {
    unsigned int bps;			/* bytes per sector */
    unsigned int spc;			/* sectors per cluster */
    unsigned int res;			/* reserved sectors */
    unsigned int nft;			/* number of FATs */
    unsigned int rde;			/* root directory entries */
    unsigned int sec;			/* total sectors */
    unsigned int mid;			/* media descriptor */
    unsigned int spf;			/* sectors per FAT */
    unsigned int spt;			/* sectors per track */
    unsigned int hds;			/* drive heads */
    unsigned int hid;			/* hidden sectors */
    unsigned int bsec; 		/* big total sectors */
    unsigned int bspf; 		/* big sectors per FAT */
    unsigned int rdcl; 		/* root directory start cluster */
    unsigned int infs; 		/* file system info sector */
    unsigned int bkbs; 		/* backup boot sector */
};

#define BPBGAP 0, 0, 0, 0, 0, 0

static struct {
    const char *name;
    struct bpb bpb;
} const stdfmt[] = {
    {"160",  {512, 1, 1, 2,  64,  320, 0xfe, 1,  8, 1, BPBGAP}},
    {"180",  {512, 1, 1, 2,  64,  360, 0xfc, 2,  9, 1, BPBGAP}},
    {"320",  {512, 2, 1, 2, 112,  640, 0xff, 1,  8, 2, BPBGAP}},
    {"360",  {512, 2, 1, 2, 112,  720, 0xfd, 2,  9, 2, BPBGAP}},
    {"640",  {512, 2, 1, 2, 112, 1280, 0xfb, 2,  8, 2, BPBGAP}},    
    {"720",  {512, 2, 1, 2, 112, 1440, 0xf9, 3,  9, 2, BPBGAP}},
    {"1200", {512, 1, 1, 2, 224, 2400, 0xf9, 7, 15, 2, BPBGAP}},
    {"1232", {1024,1, 1, 2, 192, 1232, 0xfe, 2,  8, 2, BPBGAP}},    
    {"1440", {512, 1, 1, 2, 224, 2880, 0xf0, 9, 18, 2, BPBGAP}},
    {"2880", {512, 2, 1, 2, 240, 5760, 0xf0, 9, 36, 2, BPBGAP}}
};

static const unsigned char bootcode[] = {
    0xfa,			/* cli		    */
    0x31, 0xc0, 		/* xor	   ax,ax    */
    0x8e, 0xd0, 		/* mov	   ss,ax    */
    0xbc, 0x00, 0x7c,		/* mov	   sp,7c00h */
    0xfb,			/* sti		    */
    0x8e, 0xd8, 		/* mov	   ds,ax    */
    0xe8, 0x00, 0x00,		/* call    $ + 3    */
    0x5e,			/* pop	   si	    */
    0x83, 0xc6, 0x19,		/* add	   si,+19h  */
    0xbb, 0x07, 0x00,		/* mov	   bx,0007h */
    0xfc,			/* cld		    */
    0xac,			/* lodsb	    */
    0x84, 0xc0, 		/* test    al,al    */
    0x74, 0x06, 		/* jz	   $ + 8    */
    0xb4, 0x0e, 		/* mov	   ah,0eh   */
    0xcd, 0x10, 		/* int	   10h	    */
    0xeb, 0xf5, 		/* jmp	   $ - 9    */
    0x30, 0xe4, 		/* xor	   ah,ah    */
    0xcd, 0x16, 		/* int	   16h	    */
    0xcd, 0x19, 		/* int	   19h	    */
    0x0d, 0x0a,
    'N', 'o', 'n', '-', 's', 'y', 's', 't',
    'e', 'm', ' ', 'd', 'i', 's', 'k',
    0x0d, 0x0a,
    'P', 'r', 'e', 's', 's', ' ', 'a', 'n',
    'y', ' ', 'k', 'e', 'y', ' ', 't', 'o',
    ' ', 'r', 'e', 'b', 'o', 'o', 't',
    0x0d, 0x0a,
    0
};


static void getdiskinfo(unsigned long count, struct bpb *);
static void print_bpb(struct bpb *);
static void setstr(unsigned char *dest, const char *src, size_t len);
unsigned int newfs_msdos_main(unsigned char *newfs_SDBUF, unsigned long part_size);


/*
 * Construct a FAT12, FAT16, or FAT32 file system.
 */
unsigned int newfs_msdos_main(unsigned char *newfs_SDBUF, unsigned long part_size)
{
    const char *opt_B =0, *opt_L =0, *opt_O=0, *opt_f =0;
    unsigned int opt_I = 0, opt_S = 0, opt_a = 0, opt_b = 0, opt_c = 0;
    unsigned int opt_e = 0, opt_h = 0, opt_i = 0, opt_k = 0, opt_m = 0, opt_n = 0;
    unsigned int opt_o = 0, opt_r = 0, opt_s = 0, opt_u = 0;
    int opt_N = 0;
    int Iflag = 0, mflag = 0, oflag = 0;
    int MAXPATHLEN = 4096;
    unsigned sectorCount = 0;
    char buf[MAXPATHLEN];
    unsigned int fname_off = 0;	
    struct bpb bpb;
    struct tm *tm;
    struct bs *bs;
    struct bsbpb *bsbpb;
    struct bsxbpb *bsxbpb;
    struct bsx *bsx;
    struct de *de;
    unsigned char *img;
    unsigned char *fname = newfs_SDBUF;
    unsigned int fat = 32, bss = 0, rds = 0, cls, dir = 0, lsn = 0, x = 0, x1 = 0, x2 = 0;
    int  fd = 0;
    long opt_ofs = 0;

    memset(&bpb, 0, sizeof(bpb));
    if (opt_h)
	bpb.hds = opt_h;
    if (opt_u)
	bpb.spt = opt_u;
    if (opt_S)
	bpb.bps = opt_S;
    if (opt_s)
	bpb.bsec = opt_s;
    if (oflag)
	bpb.hid = opt_o;
    if (!(opt_f || (opt_h && opt_u && opt_S && opt_s && oflag))) {
	//long delta;
	getdiskinfo(part_size, &bpb);
	bpb.spc = 8;
	#if 0
        if (opt_s) {
            bpb.bsec = opt_s;
        }
	bpb.bsec -= (opt_ofs / bpb.bps);
	delta = bpb.bsec % bpb.spt;
	if (delta != 0) {
	    bpb.bsec -= delta;
	}
	if (bpb.spc == 0) {	/* set defaults */
	    if (bpb.bsec <= 6000)	/* about 3MB -> 512 bytes */
		bpb.spc = 1;
	    else if (bpb.bsec <= (1<<17)) /* 64M -> 4k */
		bpb.spc = 8;
	    else if (bpb.bsec <= (1<<19)) /* 256M -> 8k */
		bpb.spc = 16;
	    else if (bpb.bsec <= (1<<21)) /* 1G -> 16k */
		bpb.spc = 32;
	    else
		bpb.spc = 64;		/* otherwise 32k */
	}
	#endif
    }

    if (opt_f && fat == 32)
	bpb.rde = 0;
    bss = 1;

    if (!bpb.nft)
	bpb.nft = 2;
    if (!fat) {
	if (bpb.bsec < (bpb.res ? bpb.res : bss) +
	    howmany((RESFTE + (bpb.spc ? MINCLS16 : MAXCLS12 + 1)) *
		    ((bpb.spc ? 16 : 12) / BPN), bpb.bps * NPB) *
	    bpb.nft +
	    howmany(bpb.rde ? bpb.rde : DEFRDE,
		    bpb.bps / sizeof(struct de)) +
	    (bpb.spc ? MINCLS16 : MAXCLS12 + 1) *
	    (bpb.spc ? bpb.spc : howmany(DEFBLK, bpb.bps)))
	    fat = 12;
	else if (bpb.rde || bpb.bsec <
		 (bpb.res ? bpb.res : bss) +
		 howmany((RESFTE + MAXCLS16) * 2, bpb.bps) * bpb.nft +
		 howmany(DEFRDE, bpb.bps / sizeof(struct de)) +
		 (MAXCLS16 + 1) *
		 (bpb.spc ? bpb.spc : howmany(8192, bpb.bps)))
	    fat = 16;
	else
	    fat = 32;
    }
    x = bss;
    if (fat == 32) {
	if (!bpb.infs) {
	    bpb.infs = x;
	}
	if (bpb.infs != MAXU16 && x <= bpb.infs)
	    x = bpb.infs + 1;
	if (!bpb.bkbs) {
	    bpb.bkbs = x;
	}
	if (bpb.bkbs != MAXU16 && x <= bpb.bkbs)
	    x = bpb.bkbs + 1;
    }
	
    if (!bpb.res)
	bpb.res = fat == 32 ? MAX(x, MAX(16384 / bpb.bps, 4)) : x;
    if (fat != 32 && !bpb.rde)
	bpb.rde = DEFRDE;
    rds = howmany(bpb.rde, bpb.bps / sizeof(struct de));
    if (!bpb.spc)
	for (bpb.spc = howmany(fat == 16 ? DEFBLK16 : DEFBLK, bpb.bps);
	     bpb.spc < MAXSPC &&
	     bpb.res +
	     howmany((RESFTE + maxcls(fat)) * (fat / BPN),
		     bpb.bps * NPB) * bpb.nft +
	     rds +
	     (unsigned long long)(maxcls(fat) + 1) * bpb.spc <= bpb.bsec;
	     bpb.spc <<= 1);
    x1 = bpb.res + rds;
    x = bpb.bspf ? bpb.bspf : 1;
    x1 += x * bpb.nft;
    x = (unsigned long long)(bpb.bsec - x1) * bpb.bps * NPB /
	(bpb.spc * bpb.bps * NPB + fat / BPN * bpb.nft);
    x2 = howmany((RESFTE + MIN(x, maxcls(fat))) * (fat / BPN),
		 bpb.bps * NPB);
    if (!bpb.bspf) {
	bpb.bspf = x2;
	x1 += (bpb.bspf - 1) * bpb.nft;
    }
    cls = (bpb.bsec - x1) / bpb.spc;
    x = (unsigned long long)bpb.bspf * bpb.bps * NPB / (fat / BPN) - RESFTE;
    if (cls > x)
	cls = x;
    if (cls > maxcls(fat)) {
	cls = maxcls(fat);
	bpb.bsec = x1 + (cls + 1) * bpb.spc - 1;
    }
    printf("%u sector%s in %u FAT%u cluster%s "
	   "(%u bytes/cluster)\n", cls * bpb.spc,
	   cls * bpb.spc == 1 ? "" : "s", cls, fat,
	   cls == 1 ? "" : "s", bpb.bps * bpb.spc);
    if (!bpb.mid)
	bpb.mid = !bpb.hid ? 0xf0 : 0xf8;
    if (fat == 32)
	bpb.rdcl = RESFTE;
    if (bpb.hid + bpb.bsec <= MAXU16) {
	bpb.sec = bpb.bsec;
	bpb.bsec = 0;
    }
    if (fat != 32) {
	bpb.spf = bpb.bspf;
	bpb.bspf = 0;
    }
    print_bpb(&bpb);
    if (!opt_N) {	
	img = malloc(bpb.bps);
	dir = bpb.res + (bpb.spf ? bpb.spf : bpb.bspf) * bpb.nft;
	for (lsn = 0; lsn < dir + (fat == 32 ? bpb.spc : rds); lsn++) {
	    x = lsn;
	    if (opt_B &&
		fat == 32 && bpb.bkbs != MAXU16 &&
		bss <= bpb.bkbs && x >= bpb.bkbs) {
		x -= bpb.bkbs;
	    }
	    memset(img, 0, bpb.bps);
	    if (!lsn ||
	      (fat == 32 && bpb.bkbs != MAXU16 && lsn == bpb.bkbs)) {
		x1 = sizeof(struct bs);
		bsbpb = (struct bsbpb *)(img + x1);
		mk2(bsbpb->bps, bpb.bps);
		mk1(bsbpb->spc, bpb.spc);
		mk2(bsbpb->res, bpb.res);
		mk1(bsbpb->nft, bpb.nft);
		mk2(bsbpb->rde, bpb.rde);
		mk2(bsbpb->sec, bpb.sec);
		mk1(bsbpb->mid, bpb.mid);
		mk2(bsbpb->spf, bpb.spf);
		mk2(bsbpb->spt, bpb.spt);
		mk2(bsbpb->hds, bpb.hds);
		mk4(bsbpb->hid, bpb.hid);
		mk4(bsbpb->bsec, bpb.bsec);
		x1 += sizeof(struct bsbpb);
		if (fat == 32) {
		    bsxbpb = (struct bsxbpb *)(img + x1);
		    mk4(bsxbpb->bspf, bpb.bspf);
		    mk2(bsxbpb->xflg, 0);
		    mk2(bsxbpb->vers, 0);
		    mk4(bsxbpb->rdcl, bpb.rdcl);
		    mk2(bsxbpb->infs, bpb.infs);
		    mk2(bsxbpb->bkbs, bpb.bkbs);
		    x1 += sizeof(struct bsxbpb);
		}
		bsx = (struct bsx *)(img + x1);
		mk1(bsx->sig, 0x29);
		if (Iflag)
		    x = opt_I;
		mk4(bsx->volid, 0);
		sprintf(buf, "FAT%u", fat);
		setstr(bsx->type, buf, sizeof(bsx->type));
		if (!opt_B) {
		    x1 += sizeof(struct bsx);
		    bs = (struct bs *)img;
		    mk1(bs->jmp[0], 0xeb);
		    mk1(bs->jmp[1], x1 - 2);
		    mk1(bs->jmp[2], 0x90);
		    setstr(bs->oem, opt_O ? opt_O : "BSD  4.4",
			   sizeof(bs->oem));
		    memcpy(img + x1, bootcode, sizeof(bootcode));
		    mk2(img + MINBPS - 2, DOSMAGIC);
		}
	    } else if (fat == 32 && bpb.infs != MAXU16 &&
		       (lsn == bpb.infs ||
			(bpb.bkbs != MAXU16 &&
			 lsn == bpb.bkbs + bpb.infs))) {
		mk4(img, 0x41615252);
		mk4(img + MINBPS - 28, 0x61417272);
		mk4(img + MINBPS - 24, 0xffffffff);
		mk4(img + MINBPS - 20, bpb.rdcl);
		mk2(img + MINBPS - 2, DOSMAGIC);
	    } else if (lsn >= bpb.res && lsn < dir &&
		       !((lsn - bpb.res) %
			 (bpb.spf ? bpb.spf : bpb.bspf))) {
		mk1(img[0], bpb.mid);
		for (x = 1; x < fat * (fat == 32 ? 3 : 2) / 8; x++)
		    mk1(img[x], fat == 32 && x % 4 == 3 ? 0x0f : 0xff);
	    } else if (lsn == dir && opt_L) {
		de = (struct de *)img;
		mk1(de->attr, 050);
		mk2(de->time, 0);
		mk2(de->date, 0);
	    }
 	    memcpy(fname + fname_off, img, bpb.bps);
	    fname_off = fname_off + bpb.bps;
	    sectorCount = sectorCount + bpb.bps;
        }
	free(img);
	}

        return sectorCount;
}



/*
 * Get disk slice, partition, and geometry information.
 */

static void getdiskinfo(unsigned long count, struct bpb *bpb)
{
    bpb->bps = 512;
    bpb->spt = 16;
    bpb->hds = 4;
    bpb->bsec = count / 512 ;
}


/*
 * Print out BPB values.
 */
static void print_bpb(struct bpb *bpb)
{
    printf("bps = %u spc = %u res = %u nft = %u", bpb->bps, bpb->spc, bpb->res,
	   bpb->nft);
    if (bpb->rde)
	printf(" rde = %u", bpb->rde);
    if (bpb->sec)
	printf(" sec=%u", bpb->sec);
	printf(" mid=%#x", bpb->mid);
    if (bpb->spf)
	printf(" spf=%u", bpb->spf);
    printf(" spt = %u hds=%u hid=%u", bpb->spt, bpb->hds, bpb->hid);
    if (bpb->bsec)
	printf(" bsec=%u", bpb->bsec);
    if (!bpb->spf) {
	printf(" bspf=%u rdcl=%u", bpb->bspf, bpb->rdcl);
	printf(" infs=");
	printf(bpb->infs == MAXU16 ? "%#x" : "%u", bpb->infs);
	printf(" bkbs=");
	printf(bpb->bkbs == MAXU16 ? "%#x" : "%u", bpb->bkbs);
    }
    printf("\n");
}

/*
 * Copy string, padding with spaces.
 */
static void setstr(unsigned char *dest, const char *src, size_t len)
{
    while (len--)
	*dest++ = *src ? *src++ : ' ';
}

