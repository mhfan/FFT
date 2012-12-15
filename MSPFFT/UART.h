/*
 * UART.h
 *
 *  Created on: Jun 10, 2012
 *      Author: Greg Larmore
 */

#ifndef UART_H_
#define UART_H_

#ifdef __cplusplus
extern "C" {
#endif

void initUART();
int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);

#ifdef __cplusplus
}
#endif

#endif /* UART_H_ */
