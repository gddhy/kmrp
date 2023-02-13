#ifndef VIEWGROUP_H
#define VIEWGROUP_H
//ViewGroup 继承自View
//绝对布局
#include "View.h"
typedef View ViewGroup;

//布局参数
typedef jobject LayoutParams;

#define FILL_PARENT  -1
#define MATCH_PARENT -1
#define WRAP_CONTENT -2

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//删除
void deleteViewGroup(ViewGroup thiz);

//添加View
void ViewGroup_addView(ViewGroup thiz,View child);

//添加View
void ViewGroup_addViewAt(ViewGroup thiz,View child,int index);

//添加View
void ViewGroup_addViewWH(ViewGroup thiz,View child,int width,int height);

//添加View
void ViewGroup_addViewParams(ViewGroup thiz,View child,LayoutParams params);

//得到子视图数量
int  ViewGroup_getChildCount(ViewGroup thiz);

//得到子视图
View ViewGroup_getChildAt(ViewGroup thiz,int index);

//移除子视图
void ViewGroup_removeView(ViewGroup thiz,View child);

//移除子视图
void ViewGroup_removeViewAt(ViewGroup thiz,int index);

//移除全部子视图
void ViewGroup_removeAllViews(ViewGroup thiz);

//创建一个LayoutParams
LayoutParams newLayoutParams(int width,int height);

//删除一个LayoutParams
void deleteLayoutParams(LayoutParams thiz);

//设置布局参数的宽度
void LayoutParams_setWidth(LayoutParams thiz,int width);

//设置布局参数的高度
void LayoutParams_setHeight(LayoutParams thiz,int height);

//得到布局参数的宽度
int  LayoutParams_getWidth(LayoutParams thiz);

//得到布局参数的高度
int  LayoutParams_getHeight(LayoutParams thiz);

#ifdef __cplusplus
}
#endif

#endif



