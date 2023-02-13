#include "vm.h"

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "base.h"
#include "msgqueue.h"
#include "message.h"
#include "tools/font_sky16.h"
#include "emulator.h"
#include "vm.h"

/*mrp模拟器
上层实现接口
配合官方模拟器的虚拟接口使用
*/

//线程id
pthread_t vm_therad_id = 0;

char vm_run_mrppath[DSM_MAX_FILE_LEN];

//是否是线程运行
int b_native_thread = 1;

//定时器运行标识
int b_timer_started = 0;

//线程运行标识
int b_thread_running = 0;

int vm_restart_flag = false;

//消息队列
static QUEUE mQueue;

//加载 MRP
int vm_loadMrp(char*path)
{
	mr_printf("vm_loadMrp");
	b_native_thread = FALSE;
	memset(vm_run_mrppath, 0, 1000);
	sprintf(vm_run_mrppath, "%s", path);
	
	mr_start_dsm(vm_run_mrppath);
	return 0;
}

//线程内发消息
#define MSG_DEL(pmsg)\
	if(pmsg->expand) free(pmsg->expand); free(pmsg);

#define MSG_NEW\
	(PT_MSG)malloc(sizeof(T_MSG))

//删除消息
inline static void vm_delMsg(ELEMENT*e)
{
	mr_printf("vm_delMsg()");
	PT_MSG msg = (PT_MSG)e;
	MSG_DEL(msg)
}

//延迟发送消息
inline void vm_sendMsgDelay(int what,int arg0,int arg1,int arg2,void*expand,long ms)
{
	if(!b_thread_running)
		return;
	
	PT_MSG msg = MSG_NEW;
	
	msg->what = what;
	msg->arg0 = arg0;
	msg->arg1 = arg1;
	msg->arg2 = arg2;
	msg->expand = expand;
	
	mr_printf("vm_sendMsgDelay(%d)",what);
	enqueue(mQueue,(ELEMENT)msg,ms);
}

inline void vm_sendMsg(int what,int arg0,int arg1,int arg2,void*expand)
{
	vm_sendMsgDelay(what,arg0,arg1,arg2,expand,0);
}

inline void vm_sendEmptyMsgDelay(int what,long ms)
{
	vm_sendMsgDelay(what,0,0,0,NULL,ms);
}

inline void vm_sendEmptyMsg(int what)
{
	vm_sendMsgDelay(what,0,0,0,NULL,0);
}

//退出 mrp 线程
static void sig_handle(int signo)
{
	mr_printf("timer_handle()");
	if(signo==SIGKILL)
	{
		mr_printf("thread_exit from SIGKILL");
		vm_thread_exit();
		pthread_exit(NULL);
	}
}

//线程启动，发送启动消息
static void vm_thread_run(void*data)
{
	mr_printf("vm_thread_run(%s)", data);
	
	//捕获 SIGKILL 信号，用于强制退出
	signal(SIGKILL,sig_handle);
	
	vm_sendEmptyMsg(VMMSG_ID_START);
	
	//启动主循环
	vm_loop();
	
	//运行到这里说明  MRP 结束了
	vm_thread_exit();
	vm_therad_id = 0;
	
	//退出应用
	vm_exit();
}

static void vm_thread_exit()
{
	mr_printf("vm_thread_exit()");
	mr_stop();
	
	clqueue(mQueue,vm_delMsg);
	mQueue = NULL;
}

/**
 * 强制关闭 native 线程
 */
void vm_exit_foce()
{
	if(b_native_thread)
	{
		mr_printf("native force exit call()");
		int ret = pthread_kill(vm_therad_id,SIGKILL);
		
		if(ret==ESRCH)
			mr_printf("the specified thread did not exists or already quit\n");
		else if(ret==EINVAL)
			mr_printf("signal is invalid\n");
		else
			mr_printf("the specified thread is alive\n");
	}
}

//线程内运行mrp
int vm_loadMrp_thread(char*path)
{
	mr_printf("vm_loadMrp_thread(%s)",path);
	memset(vm_run_mrppath, 0, 1000);
	sprintf(vm_run_mrppath, "%s", path);
	
	//创建消息处理器
	mQueue = new_queue();
	
	//set can sendMsg flag
	b_native_thread = 1;
	b_thread_running = 1;
	
	//启动线程
	int ret = pthread_create(&vm_therad_id,NULL,(void*)vm_thread_run,"Hello");
	if(ret!=0){
		mr_printf("native create pthread FAIL!");
	}
	return ret;
}


//重启虚拟机
void vm_restart(){
	vm_restart_flag = TRUE;
	if(b_native_thread)
		vm_sendEmptyMsg(VMMSG_ID_STOP);
	else
		mr_exit();
	
}

//暂停MRP
void vm_pause()
{
	mr_printf("mr_pauseApp()");
	
	if(b_native_thread)
		vm_sendEmptyMsg(VMMSG_ID_PAUSE);
	else
		mr_pauseApp();
}

//恢复MRP
void vm_resume()
{
	mr_printf("mr_resumeApp()");
	
	if(b_native_thread)
		vm_sendEmptyMsg(VMMSG_ID_RESUME);
	else
		mr_resumeApp();
}


//退出MRP
void vm_exit()
{
	mr_printf("vm_exit() called by user!");
	
	if(b_native_thread){
		mr_stop();
		b_thread_running = 0;
		if(vm_restart_flag){
			vm_restart_flag = FALSE;
			vm_loadMrp_thread(vm_run_mrppath);
			return ;
		}
	}else{
		mr_stop();
		if(vm_restart_flag){
			vm_restart_flag = FALSE;
			vm_loadMrp(vm_run_mrppath);
			return ;
		}
	}
	finish();
}



//虚拟机事件
void vm_event(int code,int p0,int p1)
{
	mr_printf("mr_event(%d, %d, %d)",code,p0,p1);
	
	//线程发送消息，非线程直接发消息
	if(b_native_thread)
		vm_sendMsg(VMMSG_ID_EVENT,code,p0,p1,NULL);
	else
		mr_event(code,p0,p1);
}

extern mrp_screen *real_cache;
//线程主循环，处理消息事物
static void vm_loop()
{
	mr_printf("start mainLoop()...");
	uint16 *bmp;
	Paint paint;
	int32 x, y, w, h, file;
	
	while(1)
	{
		T_MSG*msg = (T_MSG*)dequeue(mQueue);
		mr_printf("vm_loop->msg(%d)",msg->what);
		
		switch(msg->what)
		{
		case VMMSG_ID_START:
			mr_start_dsm(vm_run_mrppath);
			//mr_start_dsm("%dsm_gm.mrp");
			break;
			
		case VMMSG_ID_TIMER_OUT:
			if(b_timer_started)
				mr_timer();
			break;
			
		case VMMSG_ID_PAUSE:
			mr_pauseApp();
			break;
			
		case VMMSG_ID_RESUME:
			mr_resumeApp();
			break;
			
		case VMMSG_ID_EVENT:
			mr_event(msg->arg0,msg->arg1,msg->arg2);
			break;
			
		
		//drawBitmap发消息刷屏可改善撕裂，但是卡页面不会刷新
		case VMMSG_ID_REFRESH:
			mrp_refScreen(real_cache, 0);
			postInvalidate();
			break;	
			
		case VMMSG_ID_GETHOST:
			//((MR_GET_HOST_CB)mr_soc.callBack)(msg->arg0);
			break;
			
		case VMMSG_ID_CALLBACK:
			mr_printf("callback addr=%p arg=%d",msg->arg1,msg->arg0);
			//((MR_CALLBACK)msg->arg1)(msg->arg0);
			break;
		
		case VMMSG_ID_PLAYSOUND:
			vm_playerSound(msg->arg0,msg->arg1);
			
			mr_printf("VMMSG_ID_PLAYSOUND(%d,%d)",msg->arg0,msg->arg1);
			break;
		
		case VMMSG_ID_STOPSOUND:
			vm_stopSound(msg->arg0);
			mr_printf("VMMSG_ID_STOPSOUND(%d)",msg->arg0);
			break;
			
		case VMMSG_ID_STOP:
				MSG_DEL(msg);
				goto end;
		break;
		}
		MSG_DEL(msg);
		
	}
	
	end:
		mr_printf("exit mainLoop...");
}

//定时器相关
int32 b_timer_data = 0;
int32 b_timer_time = 0;

//到时间
void vm_timeOut()
{
	mr_printf("vm_timeOut()");
	if(b_native_thread)
		vm_sendEmptyMsg(VMMSG_ID_TIMER_OUT);
	else
		mr_timer();
}

//虚拟的定时器回调，定时调用他即可
int mrp_timerCb(void *data){
	if(b_timer_started)
	if(mr_getTime()-b_timer_data>=b_timer_time)
	{
		mr_printf("mr_timer_cb(%d)",b_timer_time);
		b_timer_data = mr_getTime();
		vm_timeOut();
	}
	return 0;
}

//音乐播放器
//音乐接口
char* mr_snd_type[5] = {
	"mid", "wav", "mp3", "amr", "mp4"
};

//播放设备5个
jobject vm_mp3[5];

//初始化播放器，应用启动时调用
int32 vm_soundInit(){
	for(int i=0;i<5;i++)
		vm_mp3[i] = createMediaPlayer();
	return MR_SUCCESS;
}

//播放音乐
int32 vm_playerSound(int type, int32 loop){
	char temp[100];
	sprintf(temp, "sdcard/mythroad/temp.%s", mr_snd_type[type]);
	
	mediaPlayerReset(vm_mp3[type]);
	if(mediaPlayerSetSourceFromPath(vm_mp3[type],temp)){
		mediaPlayerSetLooping(vm_mp3[type],loop);
		mediaPlayerSetVolume(vm_mp3[type],1,1);
		mediaPlayerPrepare(vm_mp3[type]);
		mediaPlayerStart(vm_mp3[type]);
		
	}
	return MR_SUCCESS;
}

//获取音乐播放进度
int32 vm_getSoundPos(int type){
	return mediaPlayerGetCurrentPosition(vm_mp3[type]);
}

//设置音乐播放进度
int32 vm_setSoundPos(int type, int pos){
	mediaPlayerSeekTo(vm_mp3[type],pos);
	return 0;
}

//设置播放音量
int32 vm_setSoundVolume(int type, int volume){
	mediaPlayerSetVolume(vm_mp3[type],volume,volume);
}

//停止播放
int32 vm_stopSound(int type){
	mediaPlayerStop(vm_mp3[type]);
	return MR_SUCCESS;
}

//退出播放器，应用退出时调用
int32 vm_soundExit(){
	for(int i=0;i<5;i++)
		deleteMediaPlayer(vm_mp3[i]);
	return MR_SUCCESS;
}



//屏幕缓存地址，用于mythroad内部使用
extern mrp_screen *cache;
//屏幕真实地址，用于绘制文字等操作
extern mrp_screen *real_cache;

//mrp内核屏幕改动
void vm_sizeChange(int32 w, int32 h){
	vm_scrw = w;
	vm_scrh = h;
	if(cache!=NULL){
		mrp_freeScreen(cache);
		mrp_freeScreen(real_cache);
	}
	cache = mrp_createScreen(vm_scrw,vm_scrh);
	real_cache = mrp_createScreen(vm_scrw,vm_scrh);
	
	vm_restart();
}


void vm_sleep(int32 ms){
	b_timer_time+=ms*10;
}



