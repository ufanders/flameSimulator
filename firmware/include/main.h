/* 
 * File:   main.h
 * Author: ufanders
 *
 * Created on December 18, 2014, 12:34 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

void matrixTest(void);
void bitmapTest(void);
void bitmapPutPixel(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
void bitmapGetPixel(unsigned char x, unsigned char y, unsigned char *, unsigned char *, unsigned char *);
void bitmapDrawChar(signed char, signed char, unsigned char, unsigned char, unsigned char, unsigned char);
void bitmapClear(void);
void showTime(void);
void setTime(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

