#include <asm/arch/fdl_stdio.h>

void *FDL_memcpy (void *dst, const void *src, unsigned int count)
{
    void *ret = dst;
    unsigned int *tmp_dst = (unsigned int *) dst;
    unsigned int word_num, i;
    unsigned int value;

    
#if ((defined(SC8800S_LITTLE_ENDIAN) && defined(_BIG_ENDIAN))) || (defined(FDL_ENDIAN_SWITCH))//temp for data swap in fdl1
    count =  count / 4;

    while (count-->0)
    {
        value = * (unsigned int *) src;
        *tmp_dst = ( (value & 0x000000ff) <<24) | ( (value & 0xff000000) >>24) | ( (value & 0x0000ff00) <<8) | ( (value & 0x00ff0000) >>8);
        tmp_dst = (unsigned int *) tmp_dst + 1;
        src = (unsigned int *) src + 1;
    }

#else

    while (count-->0)
    {
        * (char *) dst = * (char *) src;
        dst = (char *) dst + 1;
        src = (char *) src + 1;
    }

#endif


    return ret;
}

void *FDL_memset (void *dst, int c, unsigned int count)
{
    void *ret = dst;

    while (count--)
    {
        * (char *) dst = (char) c;
        dst = (char *) dst + 1;
    }

    return ret;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
unsigned short EndianConv_16 (unsigned short value)
{
#ifdef _LITTLE_ENDIAN
    return (value >> 8 | value << 8);
#else
    return value;
#endif
}

unsigned int EndianConv_32 (unsigned int value)
{
#ifdef _LITTLE_ENDIAN
    unsigned int nTmp = 0;
	nTmp = (value >> 24 | value << 24);

    nTmp |= ( (value >> 8) & 0x0000FF00);
    nTmp |= ( (value << 8) & 0x00FF0000);
    return nTmp;
#else
    return value;
#endif
}

