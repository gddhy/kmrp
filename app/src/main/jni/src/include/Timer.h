#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

//初始化定时器
void _initTimer();

//循环调用定时器
void _loopCallTimer();

//定时器回调的函数指针
typedef int (*TimerCallBack)(void* data);

//设置一个定时器任务
//delayMs:延时
//callBack:回调
//data:回调时的参数
void setTimer(long long delayMs,TimerCallBack callBack,void *data);

//设置一个定时器任务
void setTimerAt(long long atMs,TimerCallBack callBack,void *data);

//提前取消定时器回调，如果成功返回data,否则返回NULL
void* unsetTimer(TimerCallBack callBack);

#ifdef __cplusplus
}
#endif

#endif

