#ifndef BUTTON_H
#define BUTTON_H

#include "TextView.h"

//按钮继承自TextView，因此所有的TextView函数都能使用。
typedef TextView Button;

#ifdef __cplusplus
extern "C" {
#endif

//创建一个按钮
Button newButton();

//删除一个Button
void deleteButton(Button thiz);

#ifdef __cplusplus
}
#endif

#endif



