#include "fdl_crc.h"

unsigned short frm_chk(const unsigned short *src, int len)
{
    unsigned int sum = 0;
    while(len > 3)
    {
        sum += *src++;
        sum += *src++;
        len -= 4;
    }
    
    switch(len&0x03){
        case 2:
            sum += *src++;
            break;        
        case 3:
            sum += *src++;
            sum += *((unsigned char *)src);
            break;
        case 1:
            sum += *((unsigned char *)src);
            break;
        default:
            break;
    }

    sum = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);
    return (~sum);   
}
//This param src must be 4 byte aligned.
unsigned long Get_CheckSum(const unsigned char *src, int len)
{
    unsigned long sum =0;

    while(len > 3)
    {
       //sum += *((unsigned long *)src)++;
       sum += *src++;
       sum += *src++;
       sum += *src++;
       sum += *src++;
       
       len-=4;
    }
    while(len)
    {
       sum +=*src++;
       len--;
    }
    return sum;
}
