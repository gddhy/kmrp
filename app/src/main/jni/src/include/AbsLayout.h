#ifndef ABSLAYOUT_H
#define ABSLAYOUT_H
//AbsLayout继承自ViewGroup
//绝对布局
#include <jni.h>
#include "ViewGroup.h"
typedef ViewGroup AbsLayout;

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//创建一个绝对布局
AbsLayout newAbsLayout();

//删除一个绝对布局
void deleteAbsLayout(AbsLayout thiz);

#ifdef __cplusplus
}
#endif

#endif



