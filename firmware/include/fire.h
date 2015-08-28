/* 
 * File:   fire.h
 * Author: ufanders
 *
 * Created on December 18, 2014, 12:38 AM
 */

#ifndef FIRE_H
#define	FIRE_H

#ifdef	__cplusplus
extern "C" {
#endif

void HSVtoRGB(void *vRGB, void *vHSV);
void setPixel(unsigned char x, unsigned char y, unsigned char colorR, unsigned char colorG, unsigned char colorB);
void generateLine(void);
void shiftUp(void);
void drawFrame(int pcnt);
void setup(void);
void HSL_to_RGB(unsigned int hue, unsigned int sat, unsigned int lum, unsigned int* r, unsigned int* g, unsigned int* b);


#ifdef	__cplusplus
}
#endif

#endif	/* FIRE_H */

