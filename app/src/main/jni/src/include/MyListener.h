#ifndef MYLISTENER_H
#define MYLISTENER_H

//通用监听接口

#include <jni.h>
#include "Canvas.h"

typedef jobject View;
typedef jobject MyListener;

//点击回调
typedef void (*OnClickCallback)(View view,void *data);

//长按回调
typedef jboolean (*OnLongClickCallback)(View view,void *data);

//按键回调
typedef jboolean (*OnKeyCallback)(View view,int key,jobject keyEvent,void *data);

//触摸回调
typedef jboolean (*OnTouchCallback)(View view,int action,float x,float y,jobject event,void *data);

//绘图回调
typedef void (*OnDrawCallback)(View view,Canvas canvas,void *data);

//多媒体播放完毕回调
typedef void (*OnCompletionCallback)(jobject mediaPlayer,void *data);


//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//创建监听接口
MyListener newMyListener(int funAddr,int dataAddr);

//删除
void deleteMyListener(MyListener thiz);

//创建一个单击回调
MyListener newOnClickListener(OnClickCallback callback,void *data);

//创建一个长按回调
MyListener newOnLongClickListener(OnLongClickCallback callback,void *data);

//创建一个按键回调
MyListener newOnKeyListener(OnKeyCallback callback,void *data);

//创建一个触摸回调
MyListener newOnTouchListener(OnTouchCallback callback,void *data);

//创建一个绘图回调
MyListener newOnDrawListener(OnDrawCallback callback,void *data);

//创建一个多媒体播放完成回调
MyListener newOnCompletionListener(OnCompletionCallback callback,void *data);

#ifdef __cplusplus
}
#endif

#endif



