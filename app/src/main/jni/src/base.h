#ifndef BASE_H
#define BASE_H

#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <android/bitmap.h>
#include "include/Canvas.h"
#include "include/Paint.h"
#include "include/Bitmap.h"
#include "include/Log.h"
#include "include/Timer.h"
#include "include/LinkedList.h"
#include "include/View.h"
#include "include/TextView.h"
#include "include/ViewGroup.h"
#include "include/AbsLayout.h"
#include "include/Button.h"
#include "include/EditText.h"
#include "include/RelativeLayout.h"
#include "include/LinearLayout.h"
#include "include/ScrollView.h"
#include "include/HScrollView.h"
#include "include/MyView.h"
#include "include/MyListener.h"

#ifdef __cplusplus
extern "C" {
#endif


enum {
	ACTION_MASK = 255,
	ACTION_DOWN = 0,	//按下事件
	ACTION_UP = 1,		//抬起事件
	ACTION_MOVE = 2,	//移动事件
	ACTION_CANCEL = 3,
	ACTION_OUTSIDE = 4,
	ACTION_POINTER_DOWN = 5,
	ACTION_POINTER_UP = 6,
	ACTION_POINTER_INDEX_MASK = 65280,
	ACTION_POINTER_INDEX_SHIFT = 8,
	ACTION_POINTER_1_DOWN = 5,
	ACTION_POINTER_2_DOWN = 261,
	ACTION_POINTER_3_DOWN = 517,
	ACTION_POINTER_1_UP = 6,
	ACTION_POINTER_2_UP = 262,
	ACTION_POINTER_3_UP = 518,
	ACTION_POINTER_ID_MASK = 65280,
	ACTION_POINTER_ID_SHIFT = 8,
};

//获取屏幕宽度
int  getScreenWidth();

//获取屏幕高度
int  getScreenHeight();

//获取屏幕像素密度
float getScreenDensity();

/**
 * 通过    透明(a) 红(r) 绿(g) 蓝(b)
 * 来构造颜色
 */
#define ARGB(a,r,g,b) ( ((a&0xff)<<24) | ((r&0xff)<<16) | ((g&0xff)<<8) | (b&0xff) )

/**
 * 通过    红(r) 绿(g) 蓝(b)
 * 来构造颜色
 */
#define RGB(r,g,b) ARGB(0xff,r,g,b)

/**
 * 设置画笔粗细
 */
//@不推荐使用,参见Graphics/Paint.h
void setStrokeWidth(float width);

/**
 * 设置画笔中空
 */
//@不推荐使用,参见Graphics/Paint.h
void setStroke(jboolean stroke);

/**
 * 用color这个颜色清除屏幕
 */
//@不推荐使用,参见Graphics/Canvas.h
void drawColor(int color);

/**
 * 设置画笔颜色:color
 * 设置了之后，以后所有绘图操作都是这个颜色
 */
//@不推荐使用,参见Graphics/Paint.h
void setColor(int color);

/**
 * 设置绘图的字体大小为:size
 */
//@不推荐使用,参见Graphics/Paint.h
void setTextSize(float size);

/**
 * 画线
 * startX:起点的横坐标
 * startY:起点的纵坐标
 * stopX:终点的横坐标
 * stopY:终点的纵坐标
 */
//@不推荐使用,参见Graphics/Canvas.h
void drawLine( float startX,float startY,float stopX,float stopY);

/**
 * 画文字
 * text:文本内容
 * x:起点的横坐标
 * x:起点的纵坐标
 */
//@不推荐使用,参见Graphics/Canvas.h
void drawText(const char * text,float x,float y);

//@不推荐使用,参见Graphics/Canvas.h
void drawString(jstring string,float x,float y);

//@不推荐使用,参见Graphics/Canvas.h
void drawStringEx(jstring string,int start,int end,float x,float y);

/**
 * 不可在onDraw里面调用。
 * 重新绘图
 */
void invalidate();

/**
 * 通知系统需要重新绘图
 */
void postInvalidate();

/**
 * 不可在onDraw里面调用。
 * left,top,right,bottom:绘图区域
 * 重新绘图
 */
void invalidateRect(int left,int top,int right,int bottom);

/**
 * left,top,right,bottom:绘图区域
 * 通知系统需要重新绘图
 */
void postInvalidateRect(int left,int top,int right,int bottom);

/**
 * 画矩形
 */
//@不推荐使用,参见Graphics/Canvas.h
void drawRect(float left, float top, float right, float bottom);


/**
 * 画圆角矩形
 */
//@不推荐使用,参见Graphics/Canvas.h
void drawRoundRect(float left, float top, float right, float bottom,float rx,float ry);

/**
 * 从assets中解码图片
 * 如果解码失败返回NULL
 * 返回值需要通过deleteBitmap释放
 */
Bitmap decodeBitmapFromAssets(const char * name);

/**
 * 从文件中解码图片
 * 如果解码失败返回NULL
 * 返回值需要通过deleteBitmap释放
 */
Bitmap decodeBitmapFromFile(const char * path);

/**
 * 创建一个位图
 * 返回值需要通过deleteBitmap释放
 */
Bitmap createBitmap(int width,int height);

/**
 * 释放位图
 */
void deleteBitmap(Bitmap bitmap);

/**
 * 获取位图的宽度高度等信息
 */
void getBitmapInfo(Bitmap bitmap,AndroidBitmapInfo *bitmapInfo);

/**
 * 画位图
 * bitmap:需要画的位图
 * fromLeft,fromTop,fromRight,fromBottom : 从位图的哪一部分开始画
 * toLeft,toTop,toRight,toBottom : 画到屏幕的哪一部分
 */
//@不推荐使用,参见Graphics/Canvas.h
void drawBitmap(Bitmap bitmap,int fromLeft,int fromTop,int fromRight,int fromBottom,float toLeft,float toTop,float toRight,float toBottom);

/**
 * 测量文本宽度
 */
//@不推荐使用,参见Graphics/Paint.h
float measureTextEx(jstring string,int start,int end);

/**
 * 测量文本宽度
 */
//@不推荐使用,参见Graphics/Paint.h
float measureText(jstring string);

/**
 * 结束当前程序
*/
void finish();

/**
 * 用于编辑框回调的指针接口
 * string : 文本内容，需要通过deleteJString释放。
 * cur : 当前光标位置
 */
typedef void (*EditTextCallback) ( jstring string,int cur );

/**
 * 请求编辑文字
 * string : 文本内容
 * cur : 当前光标位置
 * onSure : 按下确定键的回调
 * onCancle : 按下取消键的回调
 */
int requestEditText(jstring title,jstring string,int cur,EditTextCallback onSure,EditTextCallback onCancel);

/**
 * 显示一个提示
 * string : 要显示的文本
 * time: 0:短暂  	1:长
 */
void showToast(jstring string,int time);

void showToastText(const char *text,int time);

/**
 * 创建java字符串
 * 返回值需要通过deleteJString释放
 */
jstring createJString(const char* text);

/**
 * 删除java字符串
 */
void deleteJString(jstring string);

/**
 * 得到java字符串长度
 */
int lengthOfJString(jstring string);

/**
 * 得到JString的拷贝，返回的内存需要使用free函数释放。
 */
char * getJStringChars(jstring string);

/**
 * 当前时间
 * 从1970年1月1号到现在的毫秒数
 */
long long currentTimeMillis();

/**
 * 获取系统启动时间,单位纳秒
 */
long long nanoTime();

/**
 * 从Assets中读取文件的全部内容。
 * 通过outLength输出长度，如果outLength返回-1那么读取失败。
 * 返回的内存需要使用free释放，如果返回NULL那么读取失败。
 */
void *readAllFromAssets(const char *name,int *outLength);

/**
 * 创建一个媒体播放器对象
 */
jobject createMediaPlayer();

/**
 * 删除一个媒体播放器对象
 */
jboolean deleteMediaPlayer(jobject mediaPlayer);

/**
 * 设置媒体播放器的数据源，（从Assert中获取）
 */
jboolean mediaPlayerSetSourceFromAssert(jobject mediaPlayer,const char *path);

/**
 * 设置媒体播放器的数据源，（从文件或网络中获取）
 */
jboolean mediaPlayerSetSourceFromPath(jobject mediaPlayer,const char *path);

/**
 * 准备播放媒体文件
 */
jboolean mediaPlayerPrepare(jobject mediaPlayer);

/**
 * 开始播放媒体文件
 */
jboolean mediaPlayerStart(jobject mediaPlayer);

/**
 * 停止播放媒体文件
 */
jboolean mediaPlayerStop(jobject mediaPlayer);

/**
 * 重置媒体播放器
 */
jboolean mediaPlayerReset(jobject mediaPlayer);

/**
 * 暂停播放器
 */
jboolean mediaPlayerPause(jobject mediaPlayer);

/**
 * 设置是否循环播放
 */
jboolean mediaPlayerSetLooping(jobject mediaPlayer,jboolean looping);

/**
 * 判断当前是否循环播放
 */
jboolean mediaPlayerIsLooping(jobject mediaPlayer);

/**
 * 判断当前是否正在播放
 */
jboolean mediaPlayerIsPlaying(jobject mediaPlayer);

/**
 * 设置音量
 */
jboolean mediaPlayerSetVolume(jobject mediaPlayer,float leftVolume,float rightVolume);

/**
 * 设置播放位置
 */
jboolean mediaPlayerSeekTo(jobject mediaPlayer,int ms);

/**
 * 获取持续时间
 */
int mediaPlayerGetDuration(jobject mediaPlayer);

/**
 * 获取当前播放的时间
 */
int mediaPlayerGetCurrentPosition(jobject mediaPlayer);

//////////////////开始画布变换接口，和所有绘图操作一样，只能在onDraw中调用////////////////////
/**
 * 保存当前画布操作，
 * 以便使用canvasRestore来恢复当前状态。
 * canvasSave与canvasRestore一定要配对使用！
 */
//@不推荐使用,参见Graphics/Canvas.h
void canvasSave();

/**
 * 恢复之前保存的画布状态，
 * 与canvasSave配对使用。
 */
//@不推荐使用,参见Graphics/Canvas.h
void canvasRestore();

/**
 * 裁剪矩形，裁剪后所有的绘图操作只会在这个区域生效。
 */
//@不推荐使用,参见Graphics/Canvas.h
void canvasClipRect(float left,float top,float right,float bottom);

/**
 * 旋转画布，
 * 将画布绕着px,py旋转degress度
 */
//@不推荐使用,参见Graphics/Canvas.h
void canvasRotate(float degrees,float px,float py);

/**
 * 平移画布，
 * 将画布平移dx,dy。
 */
//@不推荐使用,参见Graphics/Canvas.h
void canvasTranslate(float dx,float dy);

/**
 * 缩放画布
 * 将画布以px,py点为中心，横向缩放sx，纵向缩放sy。
 */
//@不推荐使用,参见Graphics/Canvas.h
void canvasScale(float sx,float sy,float px,float py);
//////////////////结束画布变换接口////////////////////////////////////////////////////////

JNIEnv* Android_JNI_GetEnv(void);

//设置主控件
void  setContentView(View view);

////需要实现的函数开始////

//在程序启动时调用
void onCreate();

//触摸事件调用
void onTouchEvent(int action,float x,float y,int index,int count,float pointersX[],float pointersY[],int pointersId[]);

//只能在这里绘图
void onDraw(int left,int top,int right,int bottom,Canvas canvas);

//屏幕大小发生变化时被调用 ，程序在启动后会调用一次
void onSizeChange(int w,int h,int oldw,int oldh,float density);

//系统每1毫秒就会调用这个方法1次
void onLoopCall();

//返回键被按下事件，如果 返回0则退出程序，
int onBackPressed();

//应用被暂停时调用，例如被后台运行
void onPause();

//应用被恢复时调用，例如从后台运行转为前台运行
void onResume();

//程序在被销毁时调用
void onDestroy();

////需要实现的函数结束////

#ifdef __cplusplus
}
#endif

#endif



