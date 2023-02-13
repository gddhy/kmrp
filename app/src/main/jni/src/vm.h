#ifndef _VM_H
#define _VM_H

#include "emulator.h"

#define VM_NATIVE_THREAD 0
extern int b_native_thread;

#define DSM_MAX_FILE_LEN 1000
extern char vm_run_mrppath[DSM_MAX_FILE_LEN];

//是否是线程运行
extern int b_native_thread;

//定时器运行标识
extern int b_timer_started;

//线程运行标识
extern int b_thread_running;


//线程消息类型
typedef enum {
	VMMSG_ID_TIMER_OUT = 1001,
	VMMSG_ID_START,
	VMMSG_ID_PAUSE,
	VMMSG_ID_RESUME,
	VMMSG_ID_EVENT,
	VMMSG_ID_STOP,
	VMMSG_ID_GETHOST,//网络异步回调
	VMMSG_ID_CALLBACK,
	
	VMMSG_ID_PLAYSOUND,//播放音乐
	VMMSG_ID_STOPSOUND,
	
	VMMSG_ID_REFRESH,  //刷新画布

	VMMSG_ID_MAX
}E_VMMSG_ID;

static void vm_loop();
static void vm_thread_exit();

//发送消息
void vm_sendMsgDelay(int what, int arg0, int arg1, int arg2, void *expand, long ms);
void vm_sendMsg(int what, int arg0, int arg1, int arg2, void *expand);
void vm_sendEmptyMsgDelay(int what, long ms);
void vm_sendEmptyMsg(int what);

//加载mrp
int vm_loadMrp(char* path);
//用pthread加载mrp
int vm_loadMrp_thread(char* path);

void vm_pause();
void vm_resume();
void vm_exit();

void vm_event(int code, int p0, int p1);

//强制退出线程
void vm_exit_foce();

extern void vm_restart();

//定时器相关
extern int32 b_timer_data;
extern int32 b_timer_time;
int mrp_timerCb(void *data);

//音乐播放
extern char* mr_snd_type[5];
extern jobject vm_mp3[5];

int32 vm_soundInit();
int32 vm_playerSound(int type, int32 loop);
int32 vm_stopSound(int type);
int32 vm_soundExit();

#endif
