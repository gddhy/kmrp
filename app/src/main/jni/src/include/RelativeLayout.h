#ifndef RELATIVELAYOUT_H
#define RELATIVELAYOUT_H
//RelativeLayout继承自ViewGroup

//相对布局
#include <jni.h>
#include "ViewGroup.h"
typedef ViewGroup RelativeLayout;

//相对布局的布局参数,继承自LayoutParams
typedef LayoutParams RLayoutParams;

#define RL_TRUE		-1  //真
#define RL_LEFT_OF		0   //左边挨着谁
#define RL_RIGHT_OF	1   //右边挨着谁
#define RL_ABOVE		2   //在谁之上
#define RL_BELOW		3   //在谁之下
#define RL_ALIGN_BASELINE	4  //对齐谁的基线
#define RL_ALIGN_LEFT	5  //对齐谁的左边
#define RL_ALIGN_TOP	6  //对齐谁的上边
#define RL_ALIGN_RIGHT	7  //对齐谁的右边
#define RL_ALIGN_BOTTOM	8  //对齐谁的下边
#define RL_ALIGN_PARENT_LEFT	9  //是否对齐父布局的左边
#define RL_ALIGN_PARENT_TOP	10 //是否对齐父布局的上边
#define RL_ALIGN_PARENT_RIGHT	11 //是否对齐父布局的右边
#define RL_ALIGN_PARENT_BOTTOM	12 //是否对齐父布局的下边
#define RL_CENTER_IN_PARENT	13 //是否在父布局中居中
#define RL_CENTER_HORIZONTAL	14 //是否在父布局中水平居中
#define RL_CENTER_VERTICAL		15 //是否在父布局中垂直居中

//以下划线打头的函数请不要手动调用
#ifdef __cplusplus
extern "C" {
#endif

//创建相对布局
RelativeLayout newRelativeLayout();

//删除相对布局
void deleteRelativeLayout(RelativeLayout thiz);

//创建一个相对布局参数
RLayoutParams newRLayoutParams(int width,int height);

//删除一个相对布局参数
void deleteRLayoutParams(RLayoutParams thiz);

//添加布局规则
void RLayoutParams_addRule(RLayoutParams thiz,int verb,int anchor);

//添加布局规则
void RLayoutParams_addRuleEx(RLayoutParams thiz,int verb);

#ifdef __cplusplus
}
#endif

#endif



