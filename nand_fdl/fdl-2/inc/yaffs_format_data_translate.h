#ifndef __YAFFS_FORMAT_DATA_TRANSLATE_h__
#define __YAFFS_FORMAT_DATA_TRANSLATE_h__
#include "fdl_yaffs2.h"

typedef struct{
	int pagesize;
	int oobsize;
	char *p_pagedata;
}yaffs_page;

int yaffs_page_translate(yaffs_page *p_src_page,yaffs_page *p_dst_page);
#endif
