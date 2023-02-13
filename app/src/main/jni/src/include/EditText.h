#ifndef EDITTEXT_H
#define EDITTEXT_H

#include "TextView.h"

//编辑框继承自TextView，因此所有的TextView函数都能使用。
typedef TextView EditText;

#ifdef __cplusplus
extern "C" {
#endif

//创建编辑框
EditText newEditText();

//删除编辑框
void deleteEditText(EditText thiz);

#ifdef __cplusplus
}
#endif

#endif



