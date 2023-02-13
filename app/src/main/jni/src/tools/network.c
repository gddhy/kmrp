#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <asm/socket.h>
#include <linux/socket.h>
#include <linux/tcp.h>
#include <pthread.h>

#include "../mrpoid/mrporting.h"

#define DSM_SOC_NUM 5

T_DSM_SOC_STAT socStat[DSM_SOC_NUM];
mr_socket_struct mr_soc;

static pthread_t thread_id = 0;
char host_name[1000];

static char*ip2str(uint32 ip,char*out)
{
	mr_sprintf(out,"%d.%d.%d.%d",ip>>24,(ip&0xff0000)>>16,(ip&0xff00)>>8,ip&0xff);
	return out;
}

void dsm_socket_init()
{
	for(int i = 0;i<DSM_SOC_NUM;i++)
		socStat[i].socketId = MR_FAILED;
}

void dsm_socket_close()
{
	int i = 0;
	
	for(i = 0;i<DSM_SOC_NUM;i++){
		if(socStat[i].socketId!=MR_FAILED)
			close(socStat[i].socketId);
		
		socStat[i].socketId = MR_FAILED;
		socStat[i].socStat = DSM_SOC_CLOSE;
		socStat[i].readStat = DSM_SOC_NOREAD;
		socStat[i].writeStat = DSM_SOC_NOWRITE;
	}
}

int32 mr_initNetwork(MR_INIT_NETWORK_CB cb,const char*mode)
{
	mr_printf("mr_initNetwork(mod:%s)",mode);
	
	dsm_socket_init();
	mr_soc.callBack = (void*)cb;
	
	if(0==strcmp(mode,"cmwap"))
		return MR_FAILED;
	return MR_SUCCESS;
}

int32 mr_closeNetwork()
{
	mr_printf("mr_closeNetwork()");
	
	dsm_socket_close();
	if(thread_id!=0){
		pthread_join(thread_id,NULL);
		thread_id = 0;
	}
	return MR_SUCCESS;
}

static char dnsBuf[8192];
static void getHost(char*ptr)
{
	char**pptr;
	struct hostent*hptr,hostinfo;
	char str[32];
	int err;
	int32 ret;
	
	//查询DNS
	ret = gethostbyname_r(host_name,&hostinfo,dnsBuf,sizeof(dnsBuf),&hptr,&err);
	if(ret||hptr==NULL)
	{
		mr_printf("error host %s %d",ptr,err);
		//emu_requestCallback(CALLBACK_GETHOSTBYNAME, MR_FAILED);
		((MR_GET_HOST_CB)mr_soc.callBack)(MR_FAILED);
		return;
	}
	
	//主机规范名
	mr_printf("official hostname:%s",hptr->h_name);
	
	//获取主机别名列表char *[]
	for(pptr = hptr->h_aliases;*pptr!=NULL;pptr++)
	{
		mr_printf("alias:%s",*pptr);
	}
	
	switch(hptr->h_addrtype)
	{
	case AF_INET:
	case AF_INET6:
		{
			mr_printf("first address: %s",inet_ntop(hptr->h_addrtype,hptr->h_addr,str,sizeof(str)));
			
			pptr = hptr->h_addr_list;//IP地址列表 char*[]
			for(;*pptr!=NULL;pptr++){
				mr_printf("address:%s",inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
			}
			memcpy(&ret,hptr->h_addr,4);
			break;
		}
		
	default:
		mr_printf("unknown address type");
		break;
	}
	
	//emu_requestCallback(CALLBACK_GETHOSTBYNAME, ntohl(ret));
	((MR_GET_HOST_CB)mr_soc.callBack)(ntohl(ret));
	thread_id = 0;
}

//#else

int32 mr_getHostByName_block(const char*ptr)
{
	char**pptr;
	struct hostent*hptr;
	char str[64];
	int err;
	int32 ret;
	
	mr_printf("mr_getHostByName_block(%s)",ptr);
	
	//查询DNS
	if((hptr = gethostbyname(ptr))==NULL)
	{
		mr_printf("mr_getHostByName_block error host");
		return MR_FAILED;
	}
	
	//主机规范名
	mr_printf("official hostname:%s",hptr->h_name);
	
	//获取主机别名列表char *[]
	for(pptr = hptr->h_aliases;*pptr!=NULL;pptr++)
		mr_printf("alias:%s",*pptr);
	
	//IP类型
	switch(hptr->h_addrtype)
	{
	case AF_INET:
	case AF_INET6:
		{
			mr_printf("first address: %s",
				inet_ntop(hptr->h_addrtype,hptr->h_addr,str,sizeof(str)));
			
			pptr = hptr->h_addr_list;//IP地址列表 char*[]
			for(;*pptr!=NULL;pptr++)
			{
				mr_printf("address:%s",
					inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
			}
			
			memcpy(&ret,hptr->h_addr,4);
			break;
		}
		
	default:
		mr_printf("unknown address type");
		break;
	}
	
	return ntohl(ret);
}

int32 mr_getHostByName(const char*ptr,MR_GET_HOST_CB cb)
{
	int ret;
	mr_printf("mr_getHostByName(%s)",ptr);
	
	mr_soc.callBack = (void*)cb;
	memset(host_name,0,1000);
	sprintf(host_name,"%s",ptr);
	
	//拦截
	/*
	if(strcmp(ptr,"spd.skymobiapp.com")==0||
			strcmp(ptr,"applist2.sky-mobi.net")==0){
		mr_printf("拦截:%s",ptr);
		memset(host_name,0,1000);
		sprintf(host_name,"%s","sai.yzjlb.net");
	}*/
	
	ret = pthread_create(&thread_id,NULL,(void*)getHost,(void*)ptr);
	if(ret!=0){
		mr_printf("mr_getHostByName pthread_create error!");
		return MR_FAILED;
	}
	return MR_WAITING;
}

static int32 dsmGetSocketFreeIndex(void)
{
	int i = 0;
	
	for(i = 0;i<DSM_SOC_NUM;i++)
	{
		if(socStat[i].socketId==MR_FAILED)
			return i;
	}
	
	return-1;
}

int socket_set_keepalive(int fd)
{
	int ret,error,flag,alive,idle,cnt,intv;
	
	/* Set: use keepalive on fd */
	alive = 1;
	if(setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,&alive,sizeof alive)!=0)
	{
		mr_printf("Set keepalive error: %s.",strerror(errno));
		return-1;
	}
	
	/* 10秒钟无数据，触发保活机制，发送保活包 */
	idle = 10;
	if(setsockopt(fd,SOL_TCP,TCP_KEEPIDLE,&idle,sizeof idle)!=0)
	{
		mr_printf("Set keepalive idle error: %s.",strerror(errno));
		return-1;
	}
	
	/* 如果没有收到回应，则5秒钟后重发保活包 */
	intv = 5;
	if(setsockopt(fd,SOL_TCP,TCP_KEEPINTVL,&intv,sizeof intv)!=0)
	{
		mr_printf("Set keepalive intv error: %s.",strerror(errno));
		return-1;
	}
	
	/* 连续3次没收到保活包，视为连接失效 */
	cnt = 3;
	if(setsockopt(fd,SOL_TCP,TCP_KEEPCNT,&cnt,sizeof cnt)!=0)
	{
		mr_printf("Set keepalive cnt error: %s.",strerror(errno));
		return-1;
	}
	
	return 0;
}

int32 mr_socket(int32 type,int32 protocol)
{
	int newType = SOCK_STREAM;
	int sockfd,index,ret;
	
	mr_printf("mr_socket(type:%d, protocol:%d)",type,protocol);
	
	index = dsmGetSocketFreeIndex();
	if(index==-1)
		return MR_FAILED;
	
	if(type==MR_SOCK_DGRAM){
		newType = SOCK_DGRAM;
	}
	
	sockfd = socket(AF_INET,newType,0);
	if(sockfd>=0)
	{
		socStat[index].socketId = sockfd;
		socStat[index].socStat = DSM_SOC_OPEN;
		
		if(type==MR_SOCK_STREAM){
			socStat[index].readStat = DSM_SOC_NOREAD;
			socStat[index].writeStat = DSM_SOC_NOWRITE;
		}else{
			socStat[index].readStat = DSM_SOC_READABLE;
			socStat[index].writeStat = DSM_SOC_WRITEABLE;
		}
		
		mr_printf("mr_socket ok index=%d",index);
		
		//设置异步模式
		if((ret = fcntl(sockfd,F_GETFL,0))<0){
			mr_printf("socket unblock err1.");
		}
		if(fcntl(sockfd,F_SETFL,ret|O_NONBLOCK)<0){
			mr_printf("socket unblock err2.");
		}
		
		socket_set_keepalive(sockfd);
		
		return index;
	}else{
		mr_printf("mr_socket fail");
		return MR_FAILED;
	}
}

static int32 _selectSocket(int s,long waitms)
{
	int flags,n,error,code,sockfd = socStat[s].socketId;
	fd_set wset;
	struct timeval tval;
	
	FD_ZERO(&wset);
	FD_SET(sockfd,&wset);
	tval.tv_sec = 0;//等待1秒
	tval.tv_usec = waitms;
	
	switch((n = select(sockfd+1,NULL,&wset,NULL,waitms?&tval:NULL)))
	{
	case 0://继续等待
		mr_printf("select time out, waiting...");
		socStat[s].socStat = DSM_SOC_CONNECTING;
		return MR_WAITING;
		
	case-1://错误
		{
			socStat[s].socStat = DSM_SOC_ERR;
			mr_printf("select error, sockfd not set");
			
			return MR_FAILED;
		}
		break;
		
	default:
		{
			if(FD_ISSET(sockfd,&wset)){
				error = 0;
				socklen_t len = sizeof(error);
				
				code = getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&error,&len);
				
				/*
				* 如果发生错误，Solaris实现的getsockopt返回-1，
				* 把pending error设置给errno. Berkeley实现的
				* getsockopt返回0, pending error返回给error.
				* 我们需要处理这两种情况
				*/
				if(code<0||error){
					mr_printf("getsockopt %d err",s);
					socStat[s].socStat = DSM_SOC_ERR;
					
					return MR_FAILED;
				}
				
				socStat[s].socStat = DSM_SOC_CONNECTED;
				socStat[s].readStat = DSM_SOC_READABLE;
				socStat[s].writeStat = DSM_SOC_WRITEABLE;
				
				mr_printf("socket connected!");
				return MR_SUCCESS;
			}
			
			mr_printf("FD_ISSET false");
			socStat[s].socStat = DSM_SOC_ERR;
			
			return MR_FAILED;
		}
	}
}

int32 mr_connect(int32 s,int32 ip,uint16 port,int32 type)
{
	struct sockaddr_in saddr;
	int ret;
	char buf[64];
	//type = 0;
	
	if(port==9999)
		port = 80;
	
	mr_printf("mr_connect(s:%d, ip:%s, port:%u, type:%d)",s,ip2str(ip,buf),port,type);
	
	memset(&saddr,0,sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(ip);
	saddr.sin_port = htons(port);
	
	errno = 0;
	ret = connect(socStat[s].socketId,(struct sockaddr*)&saddr,sizeof(struct sockaddr_in));
	if(ret==0){
		//如果返回0，则表示连接已经建立，这通常是在服务器和客户在同一台主机上时发生。
		socStat[s].socStat = DSM_SOC_CONNECTED;
		socStat[s].readStat = DSM_SOC_READABLE;
		socStat[s].writeStat = DSM_SOC_WRITEABLE;
		
		mr_printf("mr_connect ok connected");
		return MR_SUCCESS;
	}else if(ret<0&&errno!=EINPROGRESS){
		//错误
		mr_printf("mr_connect connect err!");
		socStat[s].socStat = DSM_SOC_ERR;
		return MR_FAILED;
	}else{
		//如果是 10.0.0.172
		if(ip==0x0A0000AC)
		{
			//&& dsmNetType != NETTYPE_CMWAP){
			socStat[s].socStat = DSM_SOC_CONNECTED;
			socStat[s].readStat = DSM_SOC_READABLE;
			socStat[s].writeStat = DSM_SOC_WRITEABLE;
			socStat[s].isProxy = 1;
			socStat[s].realConnected = 0;
			mr_printf("mr_connect needProxy!");
			
			return MR_SUCCESS;
		}
		
		if(type==MR_SOCKET_NONBLOCK){
			mr_printf("mr_connect waiting...");
			
			socStat[s].socStat = DSM_SOC_CONNECTING;
			return MR_WAITING;
		}else{
			//以阻塞方式连接
			mr_printf("mr_connect connect block");//老版本的 MRP 采用阻塞联网，不会查询socket状态
			
			socStat[s].socStat = DSM_SOC_CONNECTED;
			socStat[s].readStat = DSM_SOC_READABLE;
			socStat[s].writeStat = DSM_SOC_WRITEABLE;
			return _selectSocket(s,0);//0ms 阻塞
		}
	}
}

int mr_getSocketState(int s)
{
	mr_printf("getSocketState(%d)",s);
	
	if(socStat[s].socStat==DSM_SOC_CONNECTED){
		//已连接
		mr_printf("socket connected!");
		
		return MR_SUCCESS;
	}else if(socStat[s].socStat==DSM_SOC_CONNECTING){
		//正在连接
		return _selectSocket(s,50);//等 50 ms
	}else{
		mr_printf("socketfd error! %d",s);
		socStat[s].socStat = DSM_SOC_ERR;
		
		return MR_FAILED;
	}
}

int32 mr_closeSocket(int32 s)
{
	if(socStat[s].isProxy){
		mr_closeSocket(socStat[s].realSocketId);
	}
	
	int ret = -1;
	
	if(socStat[s].socketId!=MR_FAILED)
		ret = close(socStat[s].socketId);
	
	mr_printf("mr_closeSocket(%d)",s);
	
	if(ret==0)
	{
		socStat[s].socketId = MR_FAILED;
		socStat[s].socStat = DSM_SOC_CLOSE;
		socStat[s].readStat = DSM_SOC_NOREAD;
		socStat[s].writeStat = DSM_SOC_NOWRITE;
		
		return MR_SUCCESS;
	}
	
	return MR_FAILED;
}

//static void writeRecvData(void *buf, int len)
//{
//	int32 fd = mr_open("net_debug.log", MR_FILE_CREATE|MR_FILE_RDWR);
//	if(fd > 0){
//		mr_seek(fd, 0, MR_SEEK_END);
//		mr_write(fd, "RECV:\n", 6);
//		mr_write(fd, buf, len);
//		mr_write(fd, "\n", 1);
//		mr_close(fd);
//	}
//}

int32 mr_recv(int32 s,char*buf,int len){
	if(socStat[s].isProxy){
		s = socStat[s].realSocketId;
	}
	int read;
	
	//	if(gEmuEnv.showNet)
	//		app_printf("mr_recv(%d)", s);
	
	if(socStat[s].socStat==DSM_SOC_ERR)
		return MR_FAILED;
	
	if(socStat[s].readStat==DSM_SOC_READABLE)
	{
		read = recv(socStat[s].socketId,(void*)buf,len,0);
		
		mr_printf("recv(%d) %s.",socStat[s].socketId,buf);
		
		if(read>0){
			
			return read;
		}else if(read<0&&errno==EWOULDBLOCK){
			//socStat[s].readStat = DSM_SOC_NOREAD;
			return 0;
		}else{
			socStat[s].socStat = DSM_SOC_ERR;
			socStat[s].readStat = DSM_SOC_NOREAD;
			mr_printf("recv err.");
			
			return MR_FAILED;
		}
	}
	
	return 0;
}

//static void writeSendData(int32 s, void *buf, int len)
//{
//	int32 fd = mr_open("net_debug.log", MR_FILE_CREATE|MR_FILE_RDWR);
//	if(fd > 0){
//		char buf[128];
//		int l = 0;
//
//		mr_seek(fd, 0, MR_SEEK_END);
//		l += sprintf(buf, "socket=%d SEND\n", socStat[s].socketId);
//		mr_write(fd, buf, l);
//		mr_write(fd, buf, len);
//		mr_write(fd, "\n", 1);
//		mr_close(fd);
//	}
//}

void readLine(const char*p,char*out)
{
	const char*p1 = p;
	
	while(*p1&&*p1!='\r'&&*p1!='\n')
		p1++;
	
	strncpy(out,p,p1-p);
}

void getRealIP(const char*buf,int len,int32*ip,int32*prot)
{
	const char*p;
	char line[128] = {
		0
	};
	
	p = strstr(buf,"Host:");
	if(p)
	{
		char ipstr[64] = {
			0
		};
		char portstr[8] = "80";
		
		readLine(p,line);
		mr_printf("getRealIP(%s)",line);
		
		sscanf(line,"Host: %[^:]:%s",ipstr,portstr);
		
		*ip = mr_getHostByName_block(ipstr);
		*prot = atoi(portstr);
	}
}

int32 mr_send(int32 s,const char*buf,int len)
{
	mr_printf("mr_send(%s)", buf);
	
	if(socStat[s].isProxy)
	{
		if(!socStat[s].realConnected){
			//没有连接上
			//解析真实IP
			int32 ip,port;
			getRealIP(buf,len,&ip,&port);
			
			socStat[s].realSocketId = mr_socket(MR_SOCK_STREAM,MR_IPPROTO_TCP);
			mr_connect(socStat[s].realSocketId,ip,port,MR_SOCKET_BLOCK);
			mr_printf("mr_send->mr_connect(%d,%d)",ip,port);
			socStat[s].realConnected = 1;
		}
		s = socStat[s].realSocketId;
	}
	int write;
	
	//	if(gEmuEnv.showNet)
	//		app_printf("mr_send(%d)", s);
	
	if(socStat[s].socStat==DSM_SOC_ERR)
		return MR_FAILED;
	
	if(socStat[s].writeStat==DSM_SOC_WRITEABLE)
	{
		write = send(socStat[s].socketId,(void*)buf,len,0);
		
		if(write>=0){
			return write;
		}else if(write<0&&errno==EWOULDBLOCK){
			//socStat[s].writeStat = DSM_SOC_NOWRITE;
			return 0;
		}else{
			socStat[s].socStat = DSM_SOC_ERR;
			socStat[s].writeStat = DSM_SOC_NOWRITE;
			mr_printf("mr_send err");
			return MR_FAILED;
		}
	}
	
	return 0;
}

int32 mr_recvfrom(int32 s,char*buf,int len,int32*ip,uint16*port)
{
	mr_printf("mr_recvfrom %d ip=%s:%d",s,ip,*port);
	
	if(socStat[s].socStat==DSM_SOC_ERR)
		return MR_FAILED;
	
	if(socStat[s].readStat==DSM_SOC_READABLE)
	{
		if(ip!=NULL&&port!=NULL)
		{
			int read;
			struct sockaddr_in fromaddr;
			socklen_t addr_len;
			
			read = recvfrom(socStat[s].socketId,buf,len,0,(struct sockaddr*)&fromaddr,&addr_len);
			
			mr_printf("mr_recvfrom read: %s size=%d",buf,read);
			
			*port = ntohs(fromaddr.sin_port);
			*ip = ntohl(fromaddr.sin_addr.s_addr);
			if(read>=0){
				return read;
			}else if(read<0&&errno==EWOULDBLOCK){
				//socStat[s].readStat = DSM_SOC_NOREAD;
				return 0;
			}else{
				mr_printf("mr_recvfrom fail");
			}
		}
		
		return MR_FAILED;
	}
	return 0;
}

int32 mr_sendto(int32 s,const char*buf,int len,int32 ip,uint16 port)
{
	char str[32];
	mr_printf("mr_sendto %d ip=%s:%d %s)",s,ip2str(ip,str),port,buf);
	
	if(socStat[s].socStat==DSM_SOC_ERR)
		return MR_FAILED;
	
	if(socStat[s].writeStat==DSM_SOC_WRITEABLE)
	{
		struct sockaddr_in toaddr;
		int write;
		
		memset(&toaddr,0,sizeof(toaddr));
		toaddr.sin_family = AF_INET;
		toaddr.sin_port = htons(port);
		toaddr.sin_addr.s_addr = htonl(ip);
		
		write = sendto(socStat[s].socketId,buf,len,0,(struct sockaddr*)&toaddr,sizeof(struct sockaddr));
		mr_printf("mr_sendto writ %d byte",write);
		
		if(write>=0){
			return write;
		}else if(write<0&&errno==EWOULDBLOCK){
			
			return 0;
		}else{
			socStat[s].socStat = DSM_SOC_ERR;
			socStat[s].writeStat = DSM_SOC_NOWRITE;
			mr_printf("mr_sendto fail");
			return MR_FAILED;
		}
	}
	return 0;
}
