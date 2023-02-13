#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <libgen.h>


#include "mrpoid/mrporting.h"
#include "tools/encode.h"
#include "tools/font_sky16.h"

#include "base.h"
#include "screen.h"
#include "vm.h"

/*
mrp模拟器官方.a提供的接口，基本必须实现
实现大部分接口即可正常启动mrp软件
基础实现*/

#ifdef __cplusplus
extern "C"{
#endif

/*********其他系统基本接口*********/
char mr_printf_data[1000];

//自定义printf
extern char*mr_getFullPath(const char*path);
void mr_printf(const char*format,...)
{
	char*temp;
	va_list args;
	va_start(args,format);
	
	vsprintf(mr_printf_data,format,args);
	strcat(mr_printf_data,"\n");

	/*if(1)
	{
		//写入文件
		temp = mr_getFullPath(mr_log_file);
		int32 file = open(temp,O_RDWR|O_CREAT,0777);
		if(file>=0){
			lseek(file,0,MR_SEEK_END);
			write(file,mr_printf_data,strlen(mr_printf_data));
			close(file);
		}
	}*/
}

//自定义sprintf
int mr_sprintf(char*str,char*format,...)
{
	va_list args;
	va_start(args,format);
	vsprintf(str,format,args);
	return MR_SUCCESS;
}

//绘制BITMAP
extern mrp_screen*real_cache;
//行内存是连续的，所以可以一行一行的复制
void mr_drawBitmap(uint16*bmp,int16 x,int16 y,uint16 w,uint16 h)
{
	//可以理解为特效层，在缓存层上方
	if(x>=vm_scrw||y>=vm_scrh||w<=0||h<=0)
		return;
	
	
	if(1){
		uint16*p = mrp_getScrBuff(real_cache);
		for(int32 i = y;i<y+h;i++){
			memcpy((p+(vm_scrw*i)+x),(bmp+(vm_scrw*i)+x),w*2);
		}
	}else{
		mrp_screen scr;
		scr.data = bmp;
		scr.w = w;
		scr.h = h;
		
		mrp_drawScrToScr(&scr,real_cache,x,y);
	}
	
	//vm_sendEmptyMsg(VMMSG_ID_REFRESH);
	mr_printf("mr_drawBitmap()");
}


//绘制图片
void mr_drawImg(char*filename,int32 x,int32 y)
{
	Bitmap bitmap = decodeBitmapFromFile(filename);
	drawBitmap(bitmap,0,0,240,320,0,0,SCRW,SCRW*320/240);
	deleteBitmap(bitmap);
	mr_printf("mr_drawImg(%s)",filename);
}

//获取字符点整。
const char*mr_getCharBitmap(uint16 ch,uint16 fontSize,int*width,int*height){
	mr_printf("mr_getCharBitmap(%d,%d)",ch,fontSize);
	xl_font_sky16_charWidthHeight(ch,width,height);
	char*result = xl_font_sky16_getChar(ch);
	return result;
}

//绘制字符
void mr_platDrawCharReal(uint16 ch,int32 x,int32 y,uint16 color)
{
	//不需要消息，直接画即可
	xl_font_sky16_drawChar(ch,x,y,color);
	//mr_printf("mr_platDrawCharReal(%c)",ch);
}


//启动震动
int32 mr_startShake(int32 ms){
	
	return MR_SUCCESS;
}

int32 mr_stopShake(){
	
	return MR_SUCCESS;
}

int32 mr_playSound(int type,const void*data,uint32 dataLen,int32 loop){
	//存为临时文件，安卓最好不要操作sd卡
	char temp[100];
	sprintf(temp,"temp.%s",mr_snd_type[type]);
	
	mr_remove(temp);
	int32 file = mr_open(temp,MR_FILE_RDWR|MR_FILE_CREATE);
	mr_write(file,data,dataLen);
	mr_close(file);
	
	mr_printf("mr_playSound(%d, %p, %d, %d)", type, data, dataLen, loop);
	if(b_native_thread)
		vm_sendMsg(VMMSG_ID_PLAYSOUND,type,loop,0,NULL);
	else
		;//vm_playerSound(type, loop);
	return MR_SUCCESS;
}

//停止播放音频
int32 mr_stopSound(int type){
	mr_printf("mr_stopSound(%d)",type);
	if(b_native_thread)
		vm_sendMsg(VMMSG_ID_STOPSOUND,type,0,0,NULL);
	else
		;//vm_stopSound(type);
	return MR_SUCCESS;
}


//定时器启动
int32 mr_timerStart(uint16 t){
	mr_printf("mr_timerStart(%d)",t);
	b_timer_started = TRUE;
	b_timer_time = t;
	b_timer_data = mr_getTime();
	return MR_SUCCESS;
}

//停止定时器
int32 mr_timerStop(void){
	mr_printf("mr_timerStop()");
	b_timer_started = FALSE;
	b_timer_data = mr_getTime();
	return MR_SUCCESS;
}

//取得时间，单位ms
uint32 mr_getTime(void)
{
	struct timeval t;
	gettimeofday(&t,NULL);
	uint32 s = t.tv_sec*1000+t.tv_usec/1000;
	//mr_printf("mr_getTime(%ld)",s);
	return s;
}

//获取系统日期时间。
int32 mr_getDatetime(mr_datetime*datetime){
	mr_printf("mr_getDatetime()");
	if(!datetime)
		return MR_FAILED;
	
	time_t now;
	struct tm*t;
	
	time(&now);
	t = localtime(&now);
	
	datetime->year = t->tm_year+1900;
	datetime->month = t->tm_mon+1;
	datetime->day = t->tm_mday;
	datetime->hour = t->tm_hour;
	datetime->minute = t->tm_min;
	datetime->second = t->tm_sec;
	
	return MR_SUCCESS;
}

//取得手机相关信息。
#define MAKE_PLAT_VERSION(plat,ver,card,impl,brun)(100000000+(plat)*1000000+(ver)*10000+(card)*1000+(impl)*10+(brun))
int32 mr_getUserInfo(mr_userinfo*info){
	memset(info,0,sizeof(mr_userinfo));
	strncpy((char*)info->IMEI,"000000000000000",15);
	strncpy((char*)info->IMSI,"000000000000000",15);
	strncpy((char*)info->manufactory,"aux",7);
	strncpy((char*)info->spare,(char*)"cricket",12);
	strncpy((char*)info->type,"m250",7);
	info->ver = MAKE_PLAT_VERSION(1,8,0,18,1);
	return MR_SUCCESS;
}

/*********平台扩展接口*********/
#define MR_PLAT_VALUE_BASE 1000

//屏幕缓存地址
extern mrp_screen*cache;
//屏幕上层
extern mrp_screen*real_cache;

//基本的平台调用接口
int32 mr_plat(int32 code,int32 param){
	mr_printf("mr_plat(%d,%d)",code,param);
	int32 ret;
	
	switch(code)
	{
		
		//旋转屏幕
	case 101:
		//90度，内存一致，直接修改缓存宽高
		return MR_SUCCESS;
		break;
		
		//设置SIM卡
	case 1004:
		
		return MR_SUCCESS;
		
	case MR_GET_FILE_POS://获取文件读写指针
		ret = lseek(param,0,SEEK_CUR);
		if(ret>=0)
			return(ret+MR_PLAT_VALUE_BASE);
		else
			return MR_FAILED;
		break;
		
		//取随机数
	case MR_GET_RAND:
		srand(mr_getTime());
		return rand()%param;
		break;
		
		//是否支持触屏
	case MR_CHECK_TOUCH:
		return MR_TOUCH_SCREEN;
		break;
		
	case 1327://查询 WIFI 是否可用
		return MR_SUCCESS;
		
		
		//时间
	case MR_SET_SOCTIME:
		return MR_SUCCESS;
		
		//联网 1001
	case MR_CONNECT:
		return mr_getSocketState();
		break;
		
	case 1391:// 后台
		return MR_SUCCESS;
		break;
		
	case 1302:// 设置音量
		
		return MR_SUCCESS;
		
	case 1214:
		return MR_SUCCESS;
		
		//获取语言
	case MR_GET_HANDSET_LG:
		return MR_CHINESE;
		break;
		
	case 1011://设置收到新短信的时候是否显示提示界面。1提示
		return MR_SUCCESS;
		
	case 1218:
		return MR_MSDC_OK;
		break;
	}
	
	return MR_IGNORE;
}

//增强的平台扩展接口
extern mrp_screen*cache;

typedef void(*MR_PLAT_EX_CB)(uint8*output,int32 output_len);
int32 mr_platEx(int32 code,uint8*input,int32 input_len,uint8**output,int32*output_len,MR_PLAT_EX_CB*cb){
	mr_printf("mr_platEx(%d)",code);
	mr_screeninfo s;
	char*tmp;
	static char buf[32];
	
	
	//文字宽高
	int32 en_w,en_h,cn_w,cn_h,file,type = 0;
	int len;
	
	//sd卡信息
	static T_DSM_DISK_INFO dsmDiskInfo;
	
	//路径存储
	static char path[500];
	memset(path,0,sizeof(path));
	
	LOAD_AUDIO_DATA_REQ*audio;
	
	switch(code)
	{
		
	case MR_SWITCHPATH://切换目录
		switch(input[0])
		{
		case 'Y'://获取当前工作绝对路径
		case 'y':
			{
				static char buf[1000];
				memset(buf,0,sizeof(buf));
				char*p;
				
				if((p = strstr("mythroad/","sdcard/"))!=NULL){
					//在A盘下
					p+=strlen("sdcard/");//a/...
					if(p){
						if(*(p+2))
							snprintf(buf,sizeof(buf),"%c:/%s",*p,(p+2));
						else
							snprintf(buf,sizeof(buf),"%c:/",*p);
					}else{
						//说明不是 .disk/a/ 形式，未知错误
						mr_printf("dsmWorkPath ERROR!");
						
						strcpy(buf,"a:/");//给他个默认值
					}
				}else{
					snprintf(buf,sizeof(buf),"c:/%s","mythroad/");
				}
				
				*output = (uint8*)buf;
				*output_len = strlen(buf);
			}
			return MR_SUCCESS;
		}
		break;
		
		//屏幕缓冲
	case MR_MALLOC_EX://1001
		*output = (uint8*)mrp_getScrBuff(cache);
		mr_getScreenInfo(&s);
		*output_len = s.width*s.height*4;
		mr_printf("MR_MALLOC_EX(%d)->%d",input_len,*output_len);
		return MR_SUCCESS;
		
		break;
		
	case MR_MFREE_EX://1002
		//free(input);
		return MR_SUCCESS;
		break;
		
	case 1012://申请内部cache
	case MR_MALLOC_SCRRAM://申请拓展内存
		*output_len = 1024*1024*8;
		*output = malloc(*output_len);
		mr_printf("MR_MALLOC_SCRRAM");
		return MR_SUCCESS;
		break;
		
	case 1013://释放内部cache
	case MR_FREE_SCRRAM://释放拓展内存
		free(input);
		mr_printf("MR_FREE_SCRRAM");
		return MR_SUCCESS;
		break;
		
	case MR_GET_FREE_SPACE://内存信息
		dsmDiskInfo.total = 1024;
		dsmDiskInfo.tUnit = 1024*1024;
		dsmDiskInfo.account = 2000;
		dsmDiskInfo.unit = 1024*1024;
		
		*output = (uint8*)&dsmDiskInfo;
		*output_len = sizeof(T_DSM_DISK_INFO);
		return MR_SUCCESS;
		break;
		
	case MR_TUROFFBACKLIGHT://关闭背光常亮
	case MR_TURONBACKLIGHT://开启背光常亮
		return MR_SUCCESS;
		break;
		
	case 1116://获取编译时间
		len = sprintf(buf,"2023/1/1 09:17");
		*output = (uint8*)buf;//"2013/3/21 21:36";
		*output_len = len+1;
		return MR_SUCCESS;
		
		
	case 3010://画图
		{
			T_DRAW_DIRECT_REQ*xl_input = (T_DRAW_DIRECT_REQ*)input;
			mr_drawImg(xl_input->src,xl_input->ox,xl_input->oy);
			return MR_SUCCESS;
		}
		break;
		
		
	case MR_UCS2GB://1207
		
		if(!input||input_len==0){
			mr_printf("mr_platEx(1207) input err");
			return MR_FAILED;
		}
		
		if(!*output){
			mr_printf("mr_platEx(1207) ouput err");
			return MR_FAILED;
		}
		
		//input is bigend
		{
			int len = UCS2_strlen(input);
			char*buf = malloc(len+2);
			
			int gbBufLen = len+1;
			char*gbBuf = malloc(gbBufLen);
			
			memcpy(buf,input,len+2);
			UCS2ByteRev(buf);
			UCS2ToGBString((uint16*)buf,gbBuf,gbBufLen);
			strcpy(*output,gbBuf);
			free(buf);
		}
		
		return MR_SUCCESS;
		
	default:
		break;
	}
	
	//媒体相关
	code/=10;
	switch(code){
		
		//初始化媒体
	case MR_MEDIA_FREE:
	case MR_MEDIA_INIT:
		return MR_SUCCESS;
		
	case MR_MEDIA_BUF_LOAD:
		sprintf(path,"temp.%s","mid");
		mr_printf("MR_MEDIA_BUF_LOAD(%s,%d)",input,input_len);
		audio = (LOAD_AUDIO_DATA_REQ*)input;
		
		mr_remove(path);
		file = mr_open(path,MR_FILE_RDWR|MR_FILE_CREATE);
		mr_write(file,audio->buf,audio->buf_len);
		mr_close(file);
		return MR_SUCCESS;
		
	case MR_MEDIA_FILE_LOAD:
		mr_printf("MR_MEDIA_FILE_LOAD(%s,%d)",input,input_len);
		sprintf(path,"temp.%s","mid");
		mr_remove(path);
		tmp = (char*)input;
		mr_rename(tmp,path);
		return MR_SUCCESS;
		
	case MR_MEDIA_PLAY_CUR_REQ:
		if(input_len>=sizeof(T_DSM_MEDIA_PLAY)&&input!=NULL){
			T_DSM_MEDIA_PLAY*pt = (T_DSM_MEDIA_PLAY*)input;
			if(b_native_thread)
				vm_sendMsg(VMMSG_ID_PLAYSOUND,0,pt->loop,0,NULL);
			else
				;//vm_playerSound(type, loop);
			
		}else{
			if(b_native_thread)
				vm_sendMsg(VMMSG_ID_PLAYSOUND,0,0,0,NULL);
			else
				;//vm_playerSound(type, loop);
		}
		return MR_SUCCESS;
		
	case MR_MEDIA_PAUSE_REQ:
	case MR_MEDIA_RESUME_REQ:
	case MR_MEDIA_STOP_REQ:
	case MR_MEDIA_CLOSE:
		mr_stopSound(0);
		return MR_SUCCESS;
		
	}
	return MR_IGNORE;
}

//任务睡眠，单位ms
int32 mr_sleep(uint32 ms){
	mr_printf("mr_sleep(%d)",ms);
	vm_sleep(ms);
	return MR_SUCCESS;
}

//格式化路径
char*mr_formatPath(char*path,char sep)
{
	char*p,*q;
	int flag = 0;
	
	if(NULL==path)
		return NULL;
	for(p = q = path;'\0'!=*p;p++)
	{
		if('\\'==*p||'/'==*p)
		{
			if(0==flag)
				*q++ = sep;
			flag = 1;
		}
		else
		{
			*q++ = *p;
			flag = 0;
		}
	}
	*q = '\0';
	return path;
}

//获取完整路径
char*mr_getFullPath(const char*path)
{
	char*sys = "/sdcard/mythroad/";
	int len = strlen(sys)+strlen(path)+10;
	char*data = malloc(sizeof(char)*len);
	memset(data,'\0',len);
	mr_sprintf(data,"%s%s",sys,path);
	char*out = malloc(sizeof(char)*len);
	GBToUTF8String((uint8*)data,(uint8*)out,len);
	mr_formatPath(out,'/');
	free(data);
	return out;
}


//文件操作
MR_FILE_HANDLE mr_open(const char*filename,uint32 mode){
	int32 result;
	char*newdir;
	mr_printf("mr_open(%s)",filename);
	
	int new_mode = 0;
	
	if(mode&MR_FILE_RDONLY)
		new_mode = O_RDONLY;
	if(mode&MR_FILE_WRONLY)
		new_mode = O_WRONLY;
	if(mode&MR_FILE_RDWR)
		new_mode = O_RDWR;
	
	//获取真实路径
	char*fullpath = mr_getFullPath(filename);
	
	//如果文件存在 带此标志会导致错误
	if ((mode & MR_FILE_CREATE) && (0 != access(fullpath, F_OK)))
		new_mode|=O_CREAT;
	
	mr_printf("%d/%d->%s",mode,new_mode,fullpath);
	//创建上级文件夹
	
	/*
	if(new_mode&O_CREAT){
		newdir = dirname(filename);
		mr_mkDir(newdir);
		//free(newdir);
	}*/
	
	//读取文件
	result = open(fullpath,new_mode,0777);
	free(fullpath);
	
	if(result<0){
		mr_printf("mr_open_error");
		return-1;
	}
	result+=5;
	
	return result;
}


//取得文件长度
int32 mr_getLen(const char*filename){
	int32 result;
	struct stat s1;
	int ret;
	
	char*fullpath = mr_getFullPath(filename);
	ret = stat(fullpath,&s1);
	result = s1.st_size;
	if(ret!=0)
		result = -1;
	free(fullpath);
	return result;
}

//读取文件
int32 mr_read(MR_FILE_HANDLE f,void*p,uint32 l){
	int32 result;
	result = read(f-5,p,(size_t)l);
	if(result<0){
		return MR_FAILED;
	}
	return result;
}

//写入文件
int32 mr_write(MR_FILE_HANDLE f,void*p,uint32 l){
	int32 result;
	mr_printf("mr_write(%d)->%d",f,l);
	result = write(f-5,p,(size_t)l);
	if(result<0){
		result = MR_FAILED;
	}
	return result;
}

//关闭文件
int32 mr_close(MR_FILE_HANDLE f){
	int32 result;
	result = close(f-5);
	if(result!=0){
		result = MR_FAILED;
	}
	return result;
}

//设置文件指针
int32 mr_seek(MR_FILE_HANDLE f,int32 pos,int method){
	int32 result;
	
	result = lseek(f-5,(off_t)pos,method);
	if(result<0){
		return MR_FAILED;
	}
	return result>=0?MR_SUCCESS:MR_FAILED;
}

//删除文件
int32 mr_remove(const char*filename){
	int32 result;
	mr_printf("mr_remove(%s)",filename);
	char*fullname = mr_getFullPath(filename);
	result = remove(fullname);
	if(result!=0&&errno!=2){
		result MR_FAILED;
	}
	free(fullname);
	return result;
}

//重命名文件
int32 mr_rename(const char*oldname,const char*newname){
	int32 result,ret;
	result = MR_SUCCESS;
	mr_printf("mr_rename(%s)->%s",oldname,newname);
	char*fullname1 = mr_getFullPath(oldname);
	char*fullname2 = mr_getFullPath(newname);
	
	ret = rename(fullname1,fullname2);
	if(ret!=0){
		result = MR_FAILED;
	}
	return result;
}


//创建多级文件夹
int mr_mkdirs(const char*muldir)
{
	int i,len;
	char str[512];
	strncpy(str,muldir,512);
	len = strlen(str);
	for(i = 0;i<len;i++)
	{
		if(str[i]=='/')
		{
			str[i] = '\0';
			if(access(str,0)!=0)
			{
				mkdir(str,0775);
			}
			str[i] = '/';
		}
	}
	if(len>0&&access(str,0)!=0)
	{
		mkdir(str,0775);
	}
	return 0;
}

//创建文件夹
int32 mr_mkDir(const char*name){
	int32 result;
	mr_printf("mr_mkDir(%s)",name);
	
	char*fullname = mr_getFullPath(name);
	if(access(fullname,F_OK)==0)
		result = MR_SUCCESS;
	else
	{
		result = mr_mkdirs(fullname);
		if(result!=0)
			result = MR_FAILED;
	}
	free(fullname);
	
	return result;
}

//删除文件夹
int32 mr_rmDir(const char*name){
	int32 result;
	
	char*fullname = mr_getFullPath(name);
	result = rmdir(fullname);
	if(result!=0)
		result = MR_FAILED;
	free(fullname);
	return result;
}

//查找文件
MR_SEARCH_HANDLE mr_findStart(const char*name,char*buffer,uint32 len){
	MR_SEARCH_HANDLE fp = MR_FAILED;
	mr_printf("mr_findStart(%s)",name);
	
	if(!name||!buffer||len==0)
		return MR_FAILED;
	
	DIR*pDir = NULL;
	struct dirent*pDt;
	memset(buffer,0,len);
	char*fullname = mr_getFullPath(name);
	if((pDir = opendir(fullname))!=NULL){
		if((pDt = readdir(pDir))!=NULL){
			UTF8ToGBString((uint8*)pDt->d_name,(uint8*)buffer,len);
		}
		fp = (MR_SEARCH_HANDLE)pDir;
	}
	free(fullname);
	return-fp;
}

//停止查找
int32 mr_findStop(MR_SEARCH_HANDLE search_handle){
	if(!-search_handle||-search_handle==MR_FAILED)
		return MR_FAILED;
	
	DIR*pDir = (DIR*)-search_handle;
	closedir(pDir);
	return MR_SUCCESS;
}

//查找下一个
int32 mr_findGetNext(MR_SEARCH_HANDLE search_handle,char*buffer,uint32 len){
	mr_printf("mr_findGetNext");
	if(!-search_handle||-search_handle==MR_FAILED||!buffer||len==0)
		return MR_FAILED;
	
	DIR*pDir = (DIR*)-search_handle;
	struct dirent*pDt;
	
	memset(buffer,0,len);
	if((pDt = readdir(pDir))!=NULL){
		UTF8ToGBString((uint8*)pDt->d_name,(uint8*)buffer,len);
		mr_printf("next(%s)",buffer);
		return MR_SUCCESS;
	}
	return MR_FAILED;
}

//文件信息
int32 mr_info(const char*filename)
{
	struct stat s1;
	int ret;
	
	char*fullname = mr_getFullPath(filename);
	ret = stat(fullname,&s1);
	mr_printf("mr_info(%s)->%s",filename,fullname);
	free(fullname);
	
	if(ret!=0){
		mr_printf("  is err");
		return MR_IS_INVALID;
	}
	
	if(s1.st_mode&S_IFDIR){
		mr_printf("  is dir");
		return MR_IS_DIR;
	}else if(s1.st_mode&S_IFREG){
		mr_printf("  is file");
		return MR_IS_FILE;
	}else{
		mr_printf("  is other");
		return MR_IS_INVALID;
	}
}

//文件失败
int32 mr_ferrno(void){
	mr_printf("mr_ferrno.");
	return MR_SUCCESS;
}

char*vm_mem_base;
int vm_mem_len;


//刷新缓存
int32 mr_cacheSync(void*addr,int32 len)
{
	mr_printf("mr_cacheSync(%d, %d)",addr,len);
	cacheflush((long)addr,(long)(addr+len),0);
	return MR_SUCCESS;
}

//内存刷新
void mr_cacheFlush(int id)
{
	mr_printf("mr_cacheFlush()");
	cacheflush((long)vm_mem_base,(long)(vm_mem_base+vm_mem_len),0);
}


//内存申请
int32 mr_mem_get(char**mem_base,uint32*mem_len){
	char*buffer;
	int pagesize,pagecount;
	int len = 8*1024*1024;
	
	pagesize = sysconf(_SC_PAGE_SIZE);
	if(pagesize==-1)
		mr_printf("sysconf");
	
	pagecount = len/pagesize;
	len = pagesize*pagecount;
	buffer = memalign(pagesize,len);
	if(buffer==NULL)
		mr_printf("memalign");
	
	//设置内存可执行权限
	if(mprotect(buffer,len,PROT_EXEC|PROT_WRITE|PROT_READ)==-1){
		free(buffer);
		mr_printf("mprotect");
	}
	
	*mem_base = buffer;
	*mem_len = len;
	
	vm_mem_base = *mem_base;
	vm_mem_len = *mem_len;
	mr_printf("mr_mem_get(%ld,%d).",*mem_base,*mem_len);
	return MR_SUCCESS;
}

//内存释放
int32 mr_mem_free(char*mem,uint32 mem_len)
{
	mr_printf("mr_mem_free()");
	free(mem);
	vm_mem_base = NULL;
	vm_mem_len = 0;
	return MR_SUCCESS;
}

//退出平台



/*********通信接口*********/
//发送一条短消息
int32 mr_sendSms(char*pNumber,char*pContent,int32 encode){
	
	return MR_SUCCESS;
}

//拨打电话
void mr_call(char*number){
	mr_printf("mr_call().");
}

//连接WAP
void mr_connectWAP(char*wap){
	vm_sendEmptyMsg(VMMSG_ID_START);
	mr_printf("mr_connectWAP().");
}

//取得网络ID
int32 mr_getNetworkID(void){
	mr_printf("mr_getNetworkID().");
	return MR_NET_ID_MOBILE;
}


/*********本地UI接口*********/

//菜单接口
int32 mr_menuCreate(const char*title,int16 num){
	
	return MR_SUCCESS;
}

int32 mr_menuSetItem(int32 menu,const char*text,int32 index){
	
	return MR_SUCCESS;
}

int32 mr_menuShow(int32 menu){
	
	return MR_SUCCESS;
}

int32 mr_menuRelease(int32 menu){
	
	return MR_SUCCESS;
}

int32 mr_menuRefresh(int32 menu){
	
	return MR_SUCCESS;
}

//对话框接口
int32 mr_dialogCreate(const char*title,const char*text,int32 type){
	
	return MR_SUCCESS;
}

int32 mr_dialogRelease(int32 dialog){
	
	return MR_SUCCESS;
}

int32 mr_dialogRefresh(int32 dialog,const char*title,const char*text,int32 type){
	
	return MR_SUCCESS;
}

//文本框接口
int32 mr_textCreate(const char*title,const char*text,int32 type){
	
	return MR_SUCCESS;
}

int32 mr_textRelease(int32 text){
	
	return MR_SUCCESS;
}

int32 mr_textRefresh(int32 handle,const char*title,const char*text){
	
	return MR_SUCCESS;
}

//编辑框接口
int32 mr_editCreate(const char*title,const char*text,int32 type,int32 max_size){
	
	return MR_SUCCESS;
}

int32 mr_editRelease(int32 edit){
	
	return MR_SUCCESS;
}

const char*mr_editGetText(int32 edit){
	
	return "";
}

//可扩展窗体
int32 mr_winCreate(void){
	mr_printf("mr_winCreate().");
	return MR_IGNORE;
}

int32 mr_winRelease(int32 win){
	mr_printf("mr_winRelease().");
	return MR_IGNORE;
}

//获取屏幕信息
int32 mr_getScreenInfo(mr_screeninfo*s)
{
	if(s){
		s->width = vm_scrw;
		s->height = vm_scrh;
		s->bit = 32;
	}
	return MR_SUCCESS;
}

//退出虚拟机
int32 mr_exit(void)
{
	mr_printf("mr_exit() called by mythroad!");
	
	if(b_native_thread){
		vm_sendEmptyMsg(VMMSG_ID_STOP);
	}else{
		vm_exit();
	}
	return MR_SUCCESS;
}


void dsmGB2UCS2(uint8*src,uint8*dest)
{
	mr_printf("dsmGB2UCS2(%s)",src);
	gbToUCS2(src,dest);
}



#ifdef __cplusplus
}
#endif
