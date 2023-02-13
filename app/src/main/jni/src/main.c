#include "base.h"
#include "mrpoid/mrporting.h"
#include "tools/font_sky16.h"

#include "vm.h"
#include "screen.h"

//如果要使用LOGI,和LOGE请定义LOG_TAG这个宏
#define LOG_TAG "main"

//屏幕缓存地址，用于mythroad内部使用
mrp_screen *cache = NULL;
//屏幕真实地址，用于绘制文字等操作
mrp_screen *real_cache = NULL;

//手机屏幕真实宽高
int32 SCRW = 240, SCRH = 320;

//实际绘制图片画面宽高
int32 real_scrw = 240, real_scrh = 320;

//mrp系统屏幕宽高
int32 vm_scrw = 240, vm_scrh = 320;


void mrp_refresh(){
	//postInvalidate();
}

//将mrp内一个文件注册为固化应用，返回是否注册成功
//最多注册20个
void *vm_app_data[20]={0};
int vm_app_num = 0;
int addAssetsApp(char *name){
	int len = 0, result;
	void *data = readAllFromAssets(name, &len);
	 
	if(len!=-1){
		vm_app_data[vm_app_num] = data;
		result = mr_registerAPP(data, len, vm_app_num);
		if(result==MR_SUCCESS){
			vm_app_num++;
			//mr_save_mrp(data, len);
			return MR_SUCCESS;
		}else{
			free(data);
		}
	}
	return MR_FAILED;
}

//释放注册的固化应用
void freeAssetsApp(){
	int i;
	for(i=0; i<vm_app_num; i++){
		if(vm_app_data[i]!=NULL)
		free(vm_app_data[i]);
	}
	mr_printf("freeAssetsApp(%d)", i);
}


//在程序启动时调用
void onCreate()
{
	//初始化音乐
	vm_soundInit();
	
	SCRW = getScreenWidth();
	SCRH = getScreenHeight();
	
	mr_remove(mr_log_file);
	xl_font_sky16_init();
	
	
	//addAssetsApp("sapp.mrp");
	//addAssetsApp("cookie.mrp");
	//addAssetsApp("rx.mrp");
	//addAssetsApp("opdesk.mrp");
	addAssetsApp("dsm.mrp");
	
	mr_printf("onCreate(scrw:%d, scrh:%d)", SCRW, SCRH);
	loadKaypad();
	updateKeypad(0,0,800,960);
}

//触摸事件调用
void onTouchEvent(int action,float x,float y,int index,int count,float pointersX[],float pointersY[],int pointersId[])
{
	int32 zz = real_scrw*vm_scrh/vm_scrw;
	int32 xx = (int)x*vm_scrw/real_scrw;
	int32 yy = (int)y*vm_scrh/zz;
	
	mr_printf("onTouchEvent(%d,%d,%d)",action, xx, yy);
		
	if(action==2)
		action = 12;
	else if(action==0)
		action = 2;
	else if(action==1)
		action = 3;
	else
		return;
	
	if(xx<=vm_scrw&&yy<=vm_scrh){
		vm_event(action, xx, yy);
	}
}

//只能在这里绘图
void onDraw(int left,int top,int right,int bottom,Canvas canvas)
{	
	Bitmap bmp;
	Paint paint = newPaint();
	mr_screeninfo s;
	mr_getScreenInfo(&s);
	
	
	bmp = mrp_screenBitmap(cache);
	Canvas_drawBitmap(canvas,bmp,0,0,s.width,s.height,0,0,real_scrw,real_scrh,paint);
	deleteBitmap(bmp);
	
	bmp = mrp_screenBitmap(real_cache);
	Canvas_drawBitmap(canvas,bmp,0,0,s.width,s.height,0,0,real_scrw,real_scrh,paint);
	deleteBitmap(bmp);
	
	deletePaint(paint);
	//mr_printf("onDraw");
}

//应用层屏幕变动
void vm_appSizeChange(int32 w, int32 h){
	
	int32 temp;
	SCRW = w;
	SCRH = h;
	
	mr_screeninfo s;
	mr_getScreenInfo(&s);
	
	mr_printf("vm_appSizeChange(scrw:%d, scrh:%d)", SCRW, SCRH);
	
	
	//横竖屏切换
	if(SCRW<SCRH){
		real_scrw = SCRW;
		real_scrh = SCRW*s.height/s.width;
	}else{
		real_scrw = SCRH*s.width/s.height;
		real_scrh = SCRH;
	}
	
	if(SCRW<SCRH){
		//计算最低高度/三分屏幕高度
		temp = SCRH-real_scrh;
		temp = temp<SCRH/3?SCRH/3:temp;
		temp = temp>SCRH/2?SCRH/2:temp;
		updateKeypad(0, SCRH-temp, SCRW, temp);
	}else{
		temp = SCRW-real_scrw;
		temp = temp<SCRW/3?SCRW/3:temp;
		temp = temp>SCRW/2?SCRW/2:temp;
		updateKeypad(SCRW-temp, 0, temp, SCRH);
	}
}

//屏幕大小发生变化时被调用 ，程序在启动后会调用一次
void onSizeChange(int w,int h,int oldw,int oldh,float density)
{
	mr_printf("onSizeChange(scrw:%d, scrh:%d)", w, h);
	vm_appSizeChange(w, h);
	
	if(cache==NULL){
		cache = mrp_createScreen(vm_scrw,vm_scrh);
		real_cache = mrp_createScreen(vm_scrw,vm_scrh);

		//vm_loadMrp("*A");
		if(mr_info("dsm_gm.mrp")==MR_IS_FILE)
			vm_loadMrp_thread("%dsm_gm.mrp");
		else
			vm_loadMrp_thread("*A");
	}
}

//系统每1毫秒就会调用这个方法1次
void onLoopCall()
{
	postInvalidate();
	mrp_timerCb(0);
}

//返回键被按下事件，如果 返回0则退出程序，
int onBackPressed()
{
	vm_event(1, 18, 0);
	return -1;
}

//应用被暂停时调用，例如被后台运行
void onPause()
{
	vm_pause();
}

//应用被恢复时调用，例如从后台运行转为前台运行
void onResume()
{
	vm_resume();
}

//程序在被销毁时调用
void onDestroy()
{
	if(cache!=NULL){
		mrp_freeScreen(cache);
		mrp_freeScreen(real_cache);
	}
	xl_font_sky16_close();
	vm_soundExit();
	freeAssetsApp();
}
