#ifndef _SCREEN_H_
#define _SCREEN_H_

/*模拟器屏幕
用图片缓存数据，可自由绘制和拉伸屏幕
*/

#include "emulator.h"

typedef struct screen
{
	uint16* data;//图片
	int w;
	int h;//真实宽高
	
	int dx;
	int dy;//绘制位置
	int dw;
	int dh;//绘制宽高
}mrp_screen;

//创建屏幕
extern mrp_screen *mrp_createScreen(int w,int h);

//获取屏幕缓存
extern uint16* mrp_getScrBuff(mrp_screen*scr);

//Screen转Bitmap，需要释放内存
extern Bitmap mrp_screenBitmap(mrp_screen *scr);

//绘制一个screen到另外一个screen上，起始坐标x, y
extern int mrp_drawScrToScr(mrp_screen *from, mrp_screen *to, int x, int y);


//绘制一个32位bitmap数据到screen
int mrp_drawArgbToScr(mrp_screen *to, int32 *data, int w, int h, int dx, int dy);

//释放屏幕缓存
extern void mrp_freeScreen(mrp_screen*scr);

extern void mrp_refScreen(mrp_screen*scr, int16 bgr);

#endif
