#ifndef SIO_API_H
#define SIO_API_H

#define BAUD_4800			4800              
#define BAUD_9600           9600    
#define BAUD_19200          19200   
#define BAUD_38400          38400   
#define BAUD_57600          57600   
#define BAUD_115200         115200   
#define BAUD_230400         230400 
#define BAUD_460800         460800
#define BAUD_921600         921600   

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 * sio_open
 *
 * If buadrate is BAUD_NONE, the default baudrate BAUD_115200 is used.
 ******************************************************************************/
void sio_open(unsigned int baudrate);

/******************************************************************************
 * sio_set_baudrate
 ******************************************************************************/
void sio_set_baudrate(unsigned int baudrate);

/******************************************************************************
 * sio_write
 ******************************************************************************/
int sio_write(const void * buf, int len);

/******************************************************************************
 * sio_read
 *
 * Returns:
 *		The number of characters read.
 ******************************************************************************/
int sio_read(void * buf, int len);

/******************************************************************************
 * sio_putstr
 ******************************************************************************/
int sio_putstr(const char * str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SIO_API_H */
