#ifndef BITMAP_H
#define BITMAP_H

#include <jni.h>

typedef jobject Bitmap;

enum {
	FORMAT_JPEG=0,
	FORMAT_PNG,
};

#ifdef __cplusplus
extern "C" {
#endif

//创建一个位图
Bitmap newBitmap(int width,int height);

//删除一个位图
void deleteBitmap(Bitmap thiz);

//得到位图的宽度
int Bitmap_getWidth(Bitmap thiz);

//得到位图的高度
int Bitmap_getHeight(Bitmap thiz);

//锁定位图的像素，并返回像素，返回的像素数据可以编辑 ，用完后需要通过Bitmap_unlockPixels释放
unsigned int *Bitmap_lockPixels(Bitmap thiz);

//解锁像素点
void Bitmap_unlockPixels(Bitmap thiz);

//将位图保存到文件，返回0:成功，非0：失败
int Bitmap_saveToFile(Bitmap thiz,const char *path);

//将位图保存到文件，返回0:成功，非0：失败
//format:FORMAT_JPEG,FORMAT_JPEG
//quality:[0-100]
int Bitmap_saveToFileEx(Bitmap thiz,const char *path,int format,int quality);

#ifdef __cplusplus
}
#endif

#endif



