#define M_WIDTH 8
#define M_HEIGHT 8

#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "fire.h"

typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

//a color with 3 components: h, s and v
typedef struct
{
  unsigned char h;
  unsigned char s;
  unsigned char v;
} ColorHSV;

//these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[M_WIDTH][M_HEIGHT]={
    {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
    {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
    {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
    {128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
    {160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
    {192, 128, 96 , 64 , 64 , 96 , 128, 192},
    {255, 160, 128, 96 , 96 , 128, 160, 255},
    {255, 192, 160, 128, 128, 160, 192, 255}
};

//these are the hues for the fire,
//should be between 0 (red) to about 13 (yellow)
const unsigned char hueMask[M_WIDTH][M_HEIGHT]={
    {1, 4, 7, 9, 9, 8, 4, 1},
    {1, 3, 5, 7, 9, 7, 3, 1},
    {1, 3, 5, 6, 7, 6, 3, 1},
    {1, 2, 4, 5, 5, 5, 2, 1},
    {1, 2, 4, 4, 4, 4, 2, 1},
    {0, 1, 2, 3, 3, 2, 1, 0},
    {0, 0, 1, 2, 2, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
};

unsigned char matrix[M_WIDTH][M_HEIGHT];
unsigned char line[M_WIDTH];
//int pcnt = 0;


//Converts an HSV color to RGB color
void HSVtoRGB(void *vRGB, void *vHSV)
{
  ColorRGB *colorRGB=(ColorRGB *)vRGB;
  ColorHSV *colorHSV=(ColorHSV *)vHSV;

  float r, g, b, h, s, v; //this function works with floats between 0 and 1
  float f, p, q, t;
  int i;
  
  h = (float)(colorHSV->h / 256.0);
  s = (float)(colorHSV->s / 256.0);
  v = (float)(colorHSV->v / 256.0);

  //if saturation is 0, the color is a shade of grey
  if(s == 0.0) {
    b = v;
    g = b;
    r = g;
  }
  //if saturation > 0, more complex calculations are needed
  else
  {
    h *= 6.0; //to bring hue to a number between 0 and 6, better for the calculations
    i = (int)(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
    f = h - i;//the fractional part of h

    p = (float)(v * (1.0 - s));
    q = (float)(v * (1.0 - (s * f)));
    t = (float)(v * (1.0 - (s * (1.0 - f))));

    switch(i)
    {
      case 0: r=v; g=t; b=p; break;
      case 1: r=q; g=v; b=p; break;
      case 2: r=p; g=v; b=t; break;
      case 3: r=p; g=q; b=v; break;
      case 4: r=t; g=p; b=v; break;
      case 5: r=v; g=p; b=q; break;
      default: r = g = b = 0; break;
    }
  }

  colorRGB->r = (int)(r * 255.0);
  colorRGB->g = (int)(g * 255.0);
  colorRGB->b = (int)(b * 255.0);

  /*
  unsigned int h, s, v;

  h = (unsigned int)colorHSV->h;
  s = (unsigned int)colorHSV->s;
  v = (unsigned int)colorHSV->v;
  
  HSL_to_RGB(h, s, v, colorRGB->r, colorRGB->g, colorRGB->b);
  */
  
}

void HSL_to_RGB(unsigned int hue, unsigned int sat, unsigned int lum, unsigned int* r, unsigned int* g, unsigned int* b)
{
    unsigned int v;

    v = (lum < 128) ? (lum * (256 + sat)) >> 8 :
          (((lum + sat) << 8) - lum * sat) >> 8;
    if (v <= 0) {
        *r = *g = *b = 0;
    } else {
        int m;
        int sextant;
        int fract, vsf, mid1, mid2;

        m = lum + lum - v;
        hue *= 6;
        sextant = hue >> 8;
        fract = hue - (sextant << 8);
        vsf = v * fract * (v - m) / v >> 8;
        mid1 = m + vsf;
        mid2 = v - vsf;
        switch (sextant) {
           case 0: *r = v; *g = mid1; *b = m; break;
           case 1: *r = mid2; *g = v; *b = m; break;
           case 2: *r = m; *g = v; *b = mid1; break;
           case 3: *r = m; *g = mid2; *b = v; break;
           case 4: *r = mid1; *g = m; *b = v; break;
           case 5: *r = v; *g = m; *b = mid2; break;
        }
    }
}

void setPixel(unsigned char x, unsigned char y, unsigned char colorR, unsigned char colorG, unsigned char colorB){

    bitmapPutPixel(M_HEIGHT-y-1, x, colorR, colorG, colorB);
}

/**
 * randly generate the next line (matrix row)
 */
void generateLine(){
  for(unsigned char x=0;x<M_HEIGHT;x++) {
      line[x] = rand()%63;
  }
}

/**
 * shift all values in the matrix up one row
 */
void shiftUp(){
  ColorRGB colorRGB;

  for (unsigned char y=M_WIDTH-1;y>0;y--) {
    for(unsigned char x=0;x<M_HEIGHT;x++) {
        matrix[x][y] = matrix[x][y-1];
    }
  }

  for(unsigned char x=0;x<M_HEIGHT;x++) {
      matrix[x][0] = line[x];
  }
}

/**
 * draw a frame, interpolating between 2 "key frames"
 * @param pcnt percentage of interpolation
 */
void drawFrame(int pcnt){
  ColorRGB colorRGB;
  ColorHSV colorHSV;
  int nextv;

  //each row interpolates with the one before it
  for (unsigned char y=M_WIDTH-1;y>0;y--) {
    for (unsigned char x=0;x<M_HEIGHT;x++) {
        colorHSV.h = hueMask[y][x];
        colorHSV.s = 255;
        nextv =
            (((100-pcnt)*matrix[x][y]
          + pcnt*matrix[x][y-1])/100)
          - valueMask[y][x];
        colorHSV.v = (unsigned char)max(0, nextv);

        HSVtoRGB(&colorRGB, &colorHSV);
        setPixel(x, y, colorRGB.r, colorRGB.g, colorRGB.b);

    }
  }

  //first row interpolates with the "next" line
  for(unsigned char x=0;x<M_HEIGHT;x++) {
      colorHSV.h = hueMask[0][x];
      colorHSV.s = 255;
      colorHSV.v = (unsigned char)(((100-pcnt)*matrix[x][0] + pcnt*line[x])/100);

      HSVtoRGB(&colorRGB, &colorHSV);
      setPixel(x, 0, colorRGB.r, colorRGB.g, colorRGB.b);

  }
}

void setup()
{
  srand(12345);
  generateLine();

  //init all pixels to zero
  for (unsigned char y=0;y<M_WIDTH;y++) {
    for(unsigned char x=0;x<M_HEIGHT;x++) {
        matrix[x][y] = 0;
    }
  }
}

/*
void loop()
{
    if (pcnt >= 100){
        shiftUp();
        generateLine();
        pcnt = 0;
    }
    drawFrame(pcnt);
#if BOARD == 'c'
    Colorduino.FlipPage(); // swap screen buffers to show it
#endif
    pcnt+=30;
}
*/