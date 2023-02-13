#ifndef PAINT_H
#define PAINT_H
#include <jni.h>

typedef jobject Paint;

enum {
	STYLE_FILL = 0,//填充
	STYLE_FILL_AND_STROKE,//?
	STYLE_STROKE,//中空
};

enum {//字体样式
	TYPEFACE_DEFAULT = 0,//默认
	TYPEFACE_DEFAULT_BOLD,//
	TYPEFACE_SANS_SERIF,//无衬线
	TYPEFACE_SERIF,//衬线
	TYPEFACE_MONOSPACE,//等宽字体
};

#ifdef __cplusplus
extern "C" {
#endif

//创建画笔对象，用完后需要通过deletePaint释放
Paint newPaint();

//释放画笔对象
void deletePaint(Paint thiz);

//设置画笔颜色
void Paint_setColor(Paint thiz,int color);

//获取画笔颜色
int  Paint_getColor(Paint thiz);

//设置画笔尺寸
void Paint_setTextSize(Paint thiz,float size);

//得到画笔尺寸
float Paint_getTextSize(Paint thiz);

//设置画笔粗细
void  Paint_setStrokeWidth(Paint thiz,float width);

//得到画笔粗细
float Paint_getStrokeWidth(Paint thiz);

//设置画笔类型中空，还是填满
void Paint_setStyle(Paint thiz,int style);

//设置字体样式
void Paint_setTypeface(Paint thiz,int typeface);

//测量文本宽度
float Paint_measureText(Paint thiz,const char *text);

//测量文本宽度
float Paint_measureJString(Paint thiz,jstring str);

//测量文本宽度
float Paint_measureJStringEx(Paint thiz,jstring str,int start,int end);

//得到。。。
float Paint_getDescent(Paint thiz);

#ifdef __cplusplus
}
#endif

#endif



