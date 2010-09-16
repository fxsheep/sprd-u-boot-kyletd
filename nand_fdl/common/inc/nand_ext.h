#ifndef READ_NANDBUFFER_H
#define READ_NANDBUFFER_H
/******************************************************************************/
//  Description:   Input the nandflash parameter
//  Parameter:
//      pSrc:   
//	pDst:     
//      len:       
/******************************************************************************/
void NandCopy(uint32 *pSrc,    //Source Address
                  uint32 *pDst,    //Destination Address
                  uint32 len       //the byte length
                  );


/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
char VCOM_GetChar(void);
#endif // READ_NANDBUFFER_H




