#ifndef LOG_H
#define LOG_H
/**
 * 日志打印工具
 * by 望尘11
 */
#include <android/log.h>

#define _LOG_FILE_ "/sdcard/qlog.txt"
#define LOG_LEVEL_V ANDROID_LOG_VERBOSE
#define LOG_LEVEL_D ANDROID_LOG_DEBUG
#define LOG_LEVEL_I ANDROID_LOG_INFO
#define LOG_LEVEL_W ANDROID_LOG_WARN
#define LOG_LEVEL_E ANDROID_LOG_ERROR

#ifdef  DISABLE_LOG

#define LOG_INIT()
#define LOG(level,tag,...)

#else //!DISABLE_LOG

#define LOG_INIT() do{ \
	FILE *_log_file_ = fopen(_LOG_FILE_,"w"); \
	if(_log_file_==NULL) \
		break; \
	fclose(_log_file_); \
}while(0)

#define LOG(level,tag,...) do{ \
	__android_log_print(level,tag,__VA_ARGS__); \
	FILE *_log_file_ = fopen(_LOG_FILE_,"a"); \
	if(_log_file_==NULL) \
		break; \
	switch(level){ \
	case LOG_LEVEL_V: \
		fprintf(_log_file_,"V/"); \
		break; \
	case LOG_LEVEL_D: \
		fprintf(_log_file_,"D/"); \
		break; \
	case LOG_LEVEL_I: \
		fprintf(_log_file_,"I/"); \
		break; \
	case LOG_LEVEL_W: \
		fprintf(_log_file_,"W/"); \
		break; \
	case LOG_LEVEL_E: \
		fprintf(_log_file_,"E/"); \
		break; \
	} \
	fprintf(_log_file_,"%s: ",tag); \
	fprintf(_log_file_,__VA_ARGS__); \
	fprintf(_log_file_,"\n"); \
	fclose(_log_file_); \
}while(0)
#endif //DISABLE_LOG

#define LOGV(...) LOG(LOG_LEVEL_V,LOG_TAG,__VA_ARGS__)
#define LOGD(...) LOG(LOG_LEVEL_D,LOG_TAG,__VA_ARGS__)
#define LOGI(...) LOG(LOG_LEVEL_I,LOG_TAG,__VA_ARGS__)
#define LOGW(...) LOG(LOG_LEVEL_W,LOG_TAG,__VA_ARGS__)
#define LOGE(...) LOG(LOG_LEVEL_E,LOG_TAG,__VA_ARGS__)

//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif
