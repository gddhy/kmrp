#ifndef LINEARLAYOUT_H
#define LINEARLAYOUT_H
//LinearLayout继承自ViewGroup

//线性布局
#include <jni.h>
#include "ViewGroup.h"
typedef ViewGroup LinearLayout;

//线性布局的布局参数,继承自LayoutParams
typedef LayoutParams LLayoutParams;


#define HORIZONTAL 0 //水平布局
#define VERTICAL   1 //垂直布局

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//创建一个线性布局
LinearLayout newLinearLayout();

//删除一个线性布局
void deleteLinearLayout(LinearLayout thiz);

//设置线性布局方向
void LinearLayout_setOrientation(LinearLayout thiz,int orientation);

//获取线性布局方向
int  LinearLayout_getOrientation(LinearLayout thiz);

//创建一个线性布局的参数
LLayoutParams newLLayoutParams(int width,int height);

//删除一个线性布局
void deleteLLayoutParams(LLayoutParams thiz);

//设置线性布局权重
void LLayoutParams_setWeight(LLayoutParams thiz,float weight);

//得到线性布局权重
float LLayoutParams_getWeight(LLayoutParams thiz);

#ifdef __cplusplus
}
#endif

#endif



