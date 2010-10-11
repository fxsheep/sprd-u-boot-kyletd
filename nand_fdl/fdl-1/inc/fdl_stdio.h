#ifndef FDL_STDIO_H
#define FDL_STDIO_H

#ifndef NULL
#ifdef __cplusplus
#define NULL	0
#else
#define NULL 	((void*)0)
#endif /* __cplusplus */
#endif /* NULL */ 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 * FDL_memcpy
 ******************************************************************************/
void *FDL_memcpy(void *dst, const void *src, unsigned int count);

/******************************************************************************
 * FDL_memset
 ******************************************************************************/
void *FDL_memset(void *dst, int c, unsigned int count);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* FDL_STDIO_H */

