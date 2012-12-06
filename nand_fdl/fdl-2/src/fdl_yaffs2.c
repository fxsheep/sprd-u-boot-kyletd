#include "fdl_yaffs2.h"
//#include <malloc.h>

/* Extra flags applied to chunkId */

#define EXTRA_HEADER_INFO_FLAG	0x80000000
#define EXTRA_SHRINK_FLAG	0x40000000
#define EXTRA_SHADOWS_FLAG	0x20000000
#define EXTRA_SPARE_FLAGS	0x10000000

#define ALL_EXTRA_FLAGS		0xF0000000

//#define YAFFS_IGNORE_TAGS_ECC	1

/* Also, the top 4 bits of the object Id are set to the object type. */
#define EXTRA_OBJECT_TYPE_SHIFT (28)
#define EXTRA_OBJECT_TYPE_MASK  ((0x0F) << EXTRA_OBJECT_TYPE_SHIFT)

void yaffs2_InitialiseTags(yaffs_ExtendedTags * tags)
{
	memset(tags, 0, sizeof(yaffs_ExtendedTags));
	tags->validMarker0 = 0xAAAAAAAA;
	tags->validMarker1 = 0x55555555;
}

void yaffs2_PackTags2(yaffs_PackedTags2 * pt, const yaffs_ExtendedTags * t)
{
	pt->t.chunkId = t->chunkId;
	pt->t.sequenceNumber = t->sequenceNumber;
	pt->t.byteCount = t->byteCount;
	pt->t.objectId = t->objectId;

	if (t->chunkId == 0 && t->extraHeaderInfoAvailable) {
		/* Store the extra header info instead */
		/* We save the parent object in the chunkId */
		pt->t.chunkId = EXTRA_HEADER_INFO_FLAG
			| t->extraParentObjectId;
		if (t->extraIsShrinkHeader) {
			pt->t.chunkId |= EXTRA_SHRINK_FLAG;
		}
		if (t->extraShadows) {
			pt->t.chunkId |= EXTRA_SHADOWS_FLAG;
		}

		pt->t.objectId &= ~EXTRA_OBJECT_TYPE_MASK;
		pt->t.objectId |=
		    (t->extraObjectType << EXTRA_OBJECT_TYPE_SHIFT);

		if (t->extraObjectType == YAFFS_OBJECT_TYPE_HARDLINK) {
			pt->t.byteCount = t->extraEquivalentObjectId;
		} else if (t->extraObjectType == YAFFS_OBJECT_TYPE_FILE) {
			pt->t.byteCount = t->extraFileLength;
		} else {
			pt->t.byteCount = 0;
		}
	}

	//yaffs_DumpPackedTags2(pt);
	//yaffs_DumpTags2(t);

#ifndef YAFFS_IGNORE_TAGS_ECC
	yaffs_ECCCalculateOther((unsigned char *)&pt->t, sizeof(yaffs_PackedTags2TagsPart), &pt->ecc);
#endif
}


