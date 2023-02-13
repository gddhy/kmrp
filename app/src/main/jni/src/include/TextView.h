#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "View.h"
//TextView继承自View，可以使用View的全部方法
typedef View TextView;

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//新建一个文本视图
TextView newTextView();

//删除一个文本视图
void deleteTextView(TextView thiz);

//设置文本
void TextView_setText(TextView thiz,jstring text);

//设置文本
void TextView_setTextEx(TextView thiz,const char *text);

//得到文本内容，需要通过deleteJString释放
jstring TextView_getText(TextView thiz);

//得到文本内容，需要通过free释放
char *  TextView_getTextEx(TextView thiz);

//设置文字大小
void    TextView_setTextSize(TextView thiz,float size);

//设置文字颜色
void    TextView_setTextColor(TextView thiz,int color);

//设置文本是否为可选择的，默认为false
void	TextView_setTextIsSelectable(TextView thiz,bool selectable);

#ifdef __cplusplus
}
#endif

#endif



