/*模拟器屏幕
用图片缓存数据，可自由绘制和拉伸屏幕
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "screen.h"

#define MAKERGB(r, g, b) 	 (((r>>3) << 11) | ((g>>2) << 5) | ((b>>3)) )

//创建屏幕
mrp_screen* mrp_createScreen(int w,int h)
{
	mrp_screen* scr = malloc(sizeof(mrp_screen));
	scr->data = malloc(w*h*2);

	scr->w = w;
	scr->h = h;
	
	scr->dx = 0;
	scr->dy = 0;
	scr->dw = real_scrw;
	scr->dh = h*real_scrh/real_scrw;
	return scr;
}


//设置屏幕绘制宽高
int32 mrp_setScreenDW(mrp_screen *scr, int32 w, int32 h){
	
	scr->dw = w;
	scr->dh = h;
	return 0;
}

//获取屏幕缓存，16位图片数据
uint16* mrp_getScrBuff(mrp_screen*scr)
{
	return scr->data;
}

//转换为32位图片数据
uint32* mrp_scrBuffToArgb(mrp_screen*scr){
	return NULL;
}

//绘制一个screen到另外一个screen上，起始坐标x, y
int mrp_drawScrToScr(mrp_screen *from, mrp_screen *to, int x, int y){
	int32 dw = x+from->w-to->w;
	int32 dh = y+from->h-to->h;
	int32 ww = dw>0?(to->w-x):from->w;
	int32 hh = dh>0?(to->h-y):from->h;
	
	//按行复制内存
	uint16*p = mrp_getScrBuff(to);
	for(int32 i = y;i<y+hh;i++){
		memcpy((p+(to->w*i)+x),(from->data+(to->w*i)),ww*2);
	}
	return 0;
}


//16位565颜色转32位
int32 mrp_c16ToArgb(int16 rgb16){
	int32 tmp = rgb16;
	tmp = ((tmp<<19)&0x00f80000)|((tmp<<5)&0x0000fc00)|(tmp>>8&0x000000f8);
	tmp |= 0xff000000;
	return tmp;
}

//32位argb颜色转16位565
int16 mrp_argbToC16(int32 argb){
	int16 c16 = (argb&0x001f0000)>>5|(argb&0x00003f00)>>3|(argb&0x0000001f);
	return c16;
}


int mrp_drawArgbToScr(mrp_screen *to, int32 *data, int w, int h, int dx, int dy){
	mrp_screen *from = mrp_createScreen(w, h);
	for(int32 i=0; i<w; i++)
	for(int32 j=0; j<h; h++)
		*(from->data+j*w+i) = 0xf5550;//mrp_argbToC16(*(data+j*w+i));
	int32 result = mrp_drawScrToScr(from, to, dx, dy);
	mrp_freeScreen(from);
	return result;
}

//rgb bgr
//565 11100000 11000000 11100000
//     e0  c0  e0
Bitmap mrp_dataToBitmap(uint16*data,int w,int h, int32 alpha)
{
	//创建图片
	Bitmap bit = createBitmap(w,h);
	AndroidBitmapInfo info;
	getBitmapInfo(bit,&info);
	uint32* ptr = Bitmap_lockPixels(bit);
	
	//16位565转32位
	int32 tmp;
	for(int i=0;i<w*h; i++)
		*(ptr+i) = mrp_c16ToArgb(*(data+i));
		
	Bitmap_unlockPixels(bit);
	return bit;
}


//Screen转Bitmap，需要释放内存
Bitmap mrp_screenBitmap(mrp_screen *scr){
	Bitmap bitmap = mrp_dataToBitmap(scr->data,scr->w,scr->h, 0xff000000);
	return bitmap;
}

//用颜色清屏
void mrp_refScreen(mrp_screen*scr, int16 bgr){
	int16* tmp = scr->data;
	for(int i=0;i<scr->w*scr->h; i++)
	{
		*tmp = bgr;
	}
}

//释放屏幕缓存
void mrp_freeScreen(mrp_screen*scr)
{
	free(scr->data);
	free(scr);
}
