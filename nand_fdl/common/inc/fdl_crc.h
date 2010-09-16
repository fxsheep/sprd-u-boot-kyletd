#ifndef FDL_CRC_H
#define FDL_CRC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//CRC
#define HDLC_FLAG               0x7E
#define HDLC_ESCAPE             0x7D
#define HDLC_ESCAPE_MASK        0x20

/******************************************************************************
 * frm_chk
 ******************************************************************************/
unsigned short frm_chk(const unsigned short *src, int len);
unsigned long Get_CheckSum(const unsigned char *src, int len);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FDL_CRC_H */
