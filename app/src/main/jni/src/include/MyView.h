#ifndef MYVIEW_H
#define MYVIEW_H
//MyView继承自View
#include "View.h"
typedef View MyView;

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//创建一个自定义View
MyView newMyView();

//删除一个自定义View
void deleteMyView(MyView thiz);

//设置绘图回调
void MyView_setOnDrawListener(MyView thiz,OnDrawCallback callBack,void *data);

#ifdef __cplusplus
}
#endif

#endif



