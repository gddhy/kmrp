#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H
//ScrollView继承自ViewGroup

//垂直滚动布局
#include <jni.h>
#include "ViewGroup.h"

typedef ViewGroup ScrollView;

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//创建一个垂直滚动布局
ScrollView newScrollView();

//删除一个垂直滚动布局
void deleteScrollView(ScrollView thiz);

#ifdef __cplusplus
}
#endif

#endif



