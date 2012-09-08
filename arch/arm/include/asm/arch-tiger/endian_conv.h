#ifndef _ENDIAN_CONV_H_
#define _ENDIAN_CONV_H_
/* Endianess stuff */
#define my_swab32(data32_ptr) \
	do{\
		    uint8_t* data8_ptr = (uint8_t*)data32_ptr;\
		    uint8_t tmp;\
		    tmp = data8_ptr[3];\
		    data8_ptr[3] = data8_ptr[0];\
		    data8_ptr[0] = tmp;\
		\
		    tmp = data8_ptr[2];\
		    data8_ptr[2] = data8_ptr[1];\
		    data8_ptr[1] = tmp;\
	}while(0)

#define my_swab16(data16_ptr)\
	do{\
		    uint8_t* data8_ptr = (uint8_t*)data16_ptr;\
		    uint8_t tmp;\
		    tmp = data8_ptr[1];\
		    data8_ptr[1] = data8_ptr[0];\
		    data8_ptr[0] = tmp;\
	}while(0)

#define _INT_SWAP(type, ptr) \
	   do{ if(sizeof(type) == 4) {  \
		               my_swab32(ptr); \
		           }else if(sizeof(type) == 2) { \
					           my_swab16(ptr);}   \
		       }while(0)

#ifdef CMDLINE_NEED_CONV
#define INT_SWAP(data) _INT_SWAP(data, &(data))
#else
#define INT_SWAP(data)
#endif

#endif // end of _ENDIAN_CONV_H_
