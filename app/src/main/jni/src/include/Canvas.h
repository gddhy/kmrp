#ifndef CANVAS_H
#define CANVAS_H

#include <jni.h>
#include "Paint.h"
#include "Rect.h"
#include "Bitmap.h"

typedef jobject Canvas;

#ifdef __cplusplus
extern "C" {
#endif

//通过位图，创建画布
Canvas newCanvas(Bitmap bitmap);

//删除画布对象
void deleteCanvas(Canvas thiz);

//使用指定颜色清屏
void Canvas_drawColor(Canvas thiz,int color);

//画点
void Canvas_drawPoint(Canvas thiz,float x,float y,Paint paint);

//画线
void Canvas_drawLine(Canvas thiz,float startX,float startY,float stopX,float stopY,Paint paint);

//画文本
void Canvas_drawText(Canvas thiz,const char * text,float x,float y,Paint paint);

//画文本
void Canvas_drawJString(Canvas thiz,jstring string,float x,float y,Paint paint);

//画文本
void Canvas_drawJStringEx(Canvas thiz,jstring string,int start,int end,float x,float y,Paint paint);

//画矩形
void Canvas_drawRect(Canvas thiz,float left, float top, float right, float bottom,Paint paint);

//画矩形
void Canvas_drawRectEx(Canvas thiz,RectF rect,Paint paint);

//画圆角矩形
void Canvas_drawRoundRect(Canvas thiz,float left, float top, float right, float bottom,float rx,float ry,Paint paint);

//画圆角矩形
void Canvas_drawRoundRectEx(Canvas thiz,RectF rect,float rx,float ry,Paint paint);

//画圆
void Canvas_drawCircle(Canvas thiz,float cx,float cy,float r,Paint paint);

//画位图
void Canvas_drawBitmap(Canvas thiz,jobject bitmap,int fromLeft,int fromTop,int fromRight,int fromBottom,float toLeft,float toTop,float toRight,float toBottom,Paint paint);

//画位图
void Canvas_drawBitmapEx(Canvas thiz,jobject bitmap,Rect rectFrom,RectF rectTo,Paint paint);

/**
 * 保存当前画布操作，
 * 以便使用Canvas_restore来恢复当前状态。
 * Canvas_save与Canvas_restore一定要配对使用！
 */
int Canvas_save(Canvas thiz);

/**
 * 恢复之前保存的画布状态，
 * 与Canvas_save配对使用。
 */
void Canvas_restore(Canvas thiz);

/**
 * 裁剪矩形，裁剪后所有的绘图操作只会在这个区域生效。
 */
void Canvas_clipRect(Canvas thiz,int left,int top,int right,int bottom);

/**
 * 裁剪矩形，裁剪后所有的绘图操作只会在这个区域生效。
 */
void Canvas_clipRectEx(Canvas thiz,Rect rect);

/**
 * 旋转画布，
 * 将画布绕着px,py旋转degress度
 */
void Canvas_rotate(Canvas thiz,float degrees,float px,float py);

/**
 * 平移画布，
 * 将画布平移dx,dy。
 */
void Canvas_translate(Canvas thiz,float dx,float dy);

/**
 * 缩放画布
 * 将画布以px,py点为中心，横向缩放sx，纵向缩放sy。
 */
void Canvas_scale(Canvas thiz,float sx,float sy,float px,float py);

#ifdef __cplusplus
}
#endif

#endif



