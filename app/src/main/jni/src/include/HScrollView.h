#ifndef HSCROLLVIEW_H
#define HSCROLLVIEW_H
//HScrollView继承自ViewGroup

//水平滚动布局
#include <jni.h>
#include "ViewGroup.h"

typedef ViewGroup HScrollView;

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//创建一个垂直滚动布局
HScrollView newHScrollView();

//删除一个垂直滚动布局
void deleteHScrollView(HScrollView thiz);

#ifdef __cplusplus
}
#endif

#endif



