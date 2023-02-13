#ifndef FONT_SKY16_H
#define FONT_SKY16_H

#include "../mrpoid/mrporting.h"

extern void* mr_getScreenBuf(void);
extern uint16 *screenBuf; //屏幕缓冲区地址

extern int xl_font_sky16_init();
extern int xl_font_sky16_close();

extern void xl_font_sky16_dpoint(int x,int y,int color);
extern int xl_font_sky16_getfontpix(char *buf,int n);
extern char* xl_font_sky16_getChar(uint16 id);
extern char* xl_font_sky16_drawChar(uint16 id, int x, int y, uint16 color);
extern void xl_font_sky16_textWidthHeight(char *text, int32 *width, int32 *height);
extern int xl_font_sky16_textWidthHeightLines(uint8 *pcText, uint16 showWidth, int32 *width, int32 *height, int32 *lines); 
extern void xl_font_sky16_charWidthHeight(uint16 id, int32 *width, int32 *height);

#endif


