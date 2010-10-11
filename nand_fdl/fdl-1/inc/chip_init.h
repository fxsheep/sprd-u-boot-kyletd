#ifndef CHIP_INIT_H
#define CHIP_INIT_H

#ifdef __cpluspus
extern "C" {
#endif /* __cplusplus */

/*****************************************************************************/
//  Description:    Use this function to init chip for all hardware setting ;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void Chip_Init (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CHIP_INIT_H */

