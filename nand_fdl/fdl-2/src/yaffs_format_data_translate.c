//#include "yaffs_packedtags2.h"
#include "fdl_yaffs2.h"
#include "yaffs_format_data_translate.h"

#define YAFFS_LOWEST_SEQUENCE_NUMBER	0x00001000
#define YAFFS_HIGHEST_SEQUENCE_NUMBER	0xEFFFFF00
#define NULL ((void*)0)
//global variables
static unsigned long totalchunk = 0;
static unsigned long chunknumber = 0;
/*******************************************************************************
* this function used to translate yaffs format binary data stream.
* p_dst_page->p_pagedata	hold the actually one dst 'page' data that needed.
* the return values:
* -1	there is an error occured
* >0    how many src 'pages' has been translated in the src binary data stream.
********************************************************************************/
int yaffs_page_translate(yaffs_page *p_src_page,yaffs_page *p_dst_page){

	int ret = -1;
	yaffs_ObjectHeader * pheader = NULL;
	//src chunk
	char *p_src_data = p_src_page->p_pagedata;
	yaffs_PackedTags2 *p_src_spare = (yaffs_PackedTags2*)&p_src_page->p_pagedata[p_src_page->pagesize];
	//dst chunk
	char *p_dst_data = p_dst_page->p_pagedata;
	yaffs_PackedTags2 *p_dst_spare = (yaffs_PackedTags2*)&p_dst_page->p_pagedata[p_dst_page->pagesize];
	//how many src_page become one dst page
	int  count = p_dst_page->pagesize/p_src_page->pagesize;
	//object id,this used to identify which fileobject this chunk belongs to
	unsigned int  object_id = p_src_spare->t.objectId;

	if(p_dst_page->pagesize%p_src_page->pagesize != 0){
		//the translation may become too complex,here we do not cover it.
		return ret;
	}

	//first of all,reset the buffer.
	memset(p_dst_page->p_pagedata,0xff,p_dst_page->pagesize+p_dst_page->oobsize);
	if(p_src_spare->t.chunkId == 0){
		//header chunk
		//data
		memcpy(p_dst_data,p_src_data,p_src_page->pagesize);
		//spare
		p_dst_spare->t.chunkId = 0;//p_src_spare->t.chunkId;
		p_dst_spare->t.sequenceNumber = YAFFS_LOWEST_SEQUENCE_NUMBER;
		p_dst_spare->t.byteCount = p_src_spare->t.byteCount;
		p_dst_spare->t.objectId = object_id;
		yaffs_ECCCalculateOther((unsigned char *)&p_dst_spare->t, sizeof(yaffs_PackedTags2TagsPart), &p_dst_spare->ecc);
		//store the number of trunks hold by this file
		pheader = (yaffs_ObjectHeader *)p_src_data;
		if (pheader->type == YAFFS_OBJECT_TYPE_FILE) {
			totalchunk = (pheader->fileSize + p_src_page->pagesize - 1) / p_src_page->pagesize;
			chunknumber = 0;
		}
		ret = 1;
	}else if(p_src_spare->t.chunkId > 0){
		//common data chunk
		unsigned int offset = 0;
		unsigned int i = count;
		//data
		ret = 0;
		while(i){
			//every time canculate one 'src' page
			chunknumber++;
			if(chunknumber > totalchunk){
				//now we run out of the end of the file.
				i--;
				continue;
			}
			if(object_id != p_src_spare->t.objectId){
				//the two trunk do not belongs to the same fileobject.error!!
				return -1;
			}

			memcpy(&p_dst_data[offset],p_src_data,p_src_spare->t.byteCount);
			offset += p_src_spare->t.byteCount;

			//update iterators
			ret++;
			i--;
			p_src_data += p_src_page->pagesize + p_src_page->oobsize;
			p_src_spare = (yaffs_PackedTags2*)&p_src_data[p_src_page->pagesize];
		}
		//spare
		p_dst_spare->t.chunkId = chunknumber/count;
		p_dst_spare->t.sequenceNumber = YAFFS_LOWEST_SEQUENCE_NUMBER;
		p_dst_spare->t.byteCount = offset;
		p_dst_spare->t.objectId = object_id;
		yaffs_ECCCalculateOther((unsigned char *)&p_dst_spare->t, sizeof(yaffs_PackedTags2TagsPart), &p_dst_spare->ecc);
	}else{
		//unused chunk,current it means encounter an fatal error.
		return ret;
	}

	//reset global contex
	if(chunknumber >= totalchunk){
		chunknumber = 0;
		totalchunk = 0;
	}

	return ret;
}
