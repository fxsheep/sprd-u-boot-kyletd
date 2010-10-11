#include "fdl_stdio.h"

void *FDL_memcpy(void *dst, const void *src, unsigned int count)
{
	void *ret = dst;
    
    while (count-->0) {
    	*(char*)dst = *(char*)src;
    	dst = (char*)dst + 1;
    	src = (char*)src + 1;
    }
    
    return ret;
}

void *FDL_memset(void *dst, int c, unsigned int count)
{
	void *ret = dst;
	
	while (count--) {
		*(char*)dst = (char)c;
		dst = (char*)dst + 1;
	}
	return ret;
}

