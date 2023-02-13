#ifndef VIEW_H
#define VIEW_H

#include <jni.h>

typedef jobject View;

#include "Bitmap.h"
#include "MyListener.h"

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//创建一个View
View newView();

//删除一个View
void deleteView(View thiz);

//设置控件的背景颜色
void View_setBackgroundColor(View thiz,int color);

//设置控件的背景图片
void View_setBackgroundImage(View thiz,Bitmap bitmap);

//设置x
void View_setX(View thiz,float x);

//设置x
void View_setY(View thiz,float y);

//设置ID
void View_setId(View thiz,int id);

//得到ID
int  View_getId(View thiz);

//通过id查找View
View findViewById(View thiz,int id);

//设置padding
void View_setPadding(View thiz,int left,int top,int right,int buttom);

//得到宽度
int  View_getWidth(View thiz);

//得到高度
int  View_getHeight(View thiz);

//使视图无效
void View_invalidate(View thiz);

//使视图无效
void View_invalidateRect(View thiz,int left,int top,int right,int buttom);

//使视图无效
void View_postInvalidate(View thiz);

//使视图无效
void View_postInvalidateRect(View thiz,int left,int top,int right,int buttom);

#define VISIBLE 	0 //可见
#define INVISIBLE 	4 //隐藏
#define GONE 		8 //无效

//设置视图隐藏/显示
void View_setVisibility(View thiz,int vis);

//获取视图显示状态
int  View_getVisibility(View thiz);

//设置布局参数
void View_setLayoutParams(View thiz,jobject params);

//设置单击回调
void View_setOnClickListener(View thiz,OnClickCallback callBack,void *data);

//设置长按回调
void View_setOnLongClickListener(View thiz,OnLongClickCallback callBack,void *data);

//设置长按回调
void View_setOnKeyListener(View thiz,OnKeyCallback callBack,void *data);

//设置触摸回调
void View_setOnTouchListener(View thiz,OnTouchCallback callBack,void *data);

#ifdef __cplusplus
}
#endif

#endif



