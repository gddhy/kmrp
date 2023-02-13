#include "emulator.h"
#include "vm.h"

#define key_pad_num 22
AbsLayout key_layout_father;

//虚拟键盘布局
AbsLayout key_layout;

//虚拟按键
Button key_btn[key_pad_num];

//功能按键
Button key_btn_tool;


//切换分辨率布局
AbsLayout size_layout;
#define size_btn_num 7
#define size_btn_id 100
Button size_btn[size_btn_num];
char*size_btn_text[size_btn_num] = {
	"128*160","176*220","240*320","240*400","320*240","320*480","480*800"
};
int32 size_btn_data[size_btn_num][2] = {
	{
		128,160
	},{
		176,220
	},{
		240,320
	},{
		240,400
	},{
		320,240
	},{
		320,480
	},{
		480,800
	}
};


//屏幕缓存地址，用于mythroad内部使用
extern mrp_screen*cache;
//屏幕真实地址，用于绘制文字等操作
extern mrp_screen*real_cache;


//监听keypad
jboolean keypad_onclick_callback(View view,int action,float x,float y,jobject event,void*data){
	
	if(action==0||action==1){
		switch(View_getId(view)){
		case 16:
			View_setVisibility(key_btn_tool,INVISIBLE);
			View_setVisibility(key_layout,INVISIBLE);
			View_setVisibility(size_layout,VISIBLE);
			break;
			
		case 21:
			vm_sendEmptyMsg(VMMSG_ID_STOP);
			break;
			
		case 22:
			memset(vm_run_mrppath,0,1000);
			sprintf(vm_run_mrppath,"%s","*B");
			
			vm_appSizeChange(SCRW,SCRH);
			vm_restart();
			break;
			
		default:
			vm_event(action,View_getId(view),0);
			break;
		}
	}
	return 0;
}

void keybtn_onclick_callback(View view,void*data){
	int32 show = View_getVisibility(key_layout);
	int32 vid = View_getId(view);
	switch(vid){
	case key_pad_num:
		View_setVisibility(key_layout,show==VISIBLE?INVISIBLE:VISIBLE);
		break;
		
	default:
		vm_scrw = size_btn_data[vid-size_btn_id][0];
		vm_scrh = size_btn_data[vid-size_btn_id][1];
		if(cache!=NULL){
			mrp_freeScreen(cache);
			mrp_freeScreen(real_cache);
		}
		cache = mrp_createScreen(vm_scrw,vm_scrh);
		real_cache = mrp_createScreen(vm_scrw,vm_scrh);
		
		View_setVisibility(key_btn_tool,VISIBLE);
		View_setVisibility(key_layout,VISIBLE);
		View_setVisibility(size_layout,INVISIBLE);
		
		//memset(vm_run_mrppath,0,1000);
		//sprintf(vm_run_mrppath,"%s","*A");
		
		vm_appSizeChange(SCRW,SCRH);
		vm_restart();
		break;
	}
}

void V_setY(View v,int32 y){
	View_setY(v,SCRH-y);
}


//更新按键位置
void updateKeypad(int32 x,int32 y,int32 w,int32 h){
	int ww = w/6;
	int hh = h/4;
	char data[100];
	
	//虚拟键盘
	for(int i = 0;i<key_pad_num;i++){
		View_setY(key_btn[i],y);
		View_setVisibility(key_btn[i],VISIBLE);
	}
	
	for(int i = 0;i<3;i++)
		for(int j = 0;j<3;j++){
		View_setX(key_btn[i*3+j+1],x+j*ww);
		View_setY(key_btn[i*3+j+1],y+i*hh);
		sprintf(data," %d ",i*3+j+1);
		TextView_setTextEx(key_btn[i*3+j+1],data);
	}
	View_setX(key_btn[0],x+ww*1);
	View_setY(key_btn[0],y+hh*3);
	TextView_setTextEx(key_btn[0]," 0 ");
	
	View_setX(key_btn[10],x+ww*0);
	View_setY(key_btn[10],y+hh*3);
	TextView_setTextEx(key_btn[10]," * ");
	
	View_setX(key_btn[11],x+ww*2);
	View_setY(key_btn[11],y+hh*3);
	TextView_setTextEx(key_btn[11]," # ");
	
	
	View_setX(key_btn[17],x+ww*3);
	View_setY(key_btn[17],y+hh*0);
	TextView_setTextEx(key_btn[17],"选择");
	
	View_setX(key_btn[18],x+ww*5);
	View_setY(key_btn[18],y+hh*0);
	TextView_setTextEx(key_btn[18],"返回");
	
	
	View_setX(key_btn[12],x+ww*4);
	View_setY(key_btn[12],y+hh*0);
	TextView_setTextEx(key_btn[12]," ↑ ");
	
	
	View_setX(key_btn[14],x+ww*3);
	View_setY(key_btn[14],y+hh*1);
	TextView_setTextEx(key_btn[14]," ← ");
	
	View_setX(key_btn[20],x+ww*4);
	View_setY(key_btn[20],y+hh*1);
	TextView_setTextEx(key_btn[20],"  +  ");
	
	View_setX(key_btn[15],x+ww*5);
	View_setY(key_btn[15],y+hh*1);
	TextView_setTextEx(key_btn[15]," → ");
	
	View_setX(key_btn[19],x+ww*3);
	View_setY(key_btn[19],y+hh*2);
	TextView_setTextEx(key_btn[19],"接听");
	
	View_setX(key_btn[13],x+ww*4);
	View_setY(key_btn[13],y+hh*2);
	TextView_setTextEx(key_btn[13]," ↓ ");
	
	View_setX(key_btn[16],x+ww*3);
	View_setY(key_btn[16],y+hh*3);
	TextView_setTextEx(key_btn[16],"设置");
	
	View_setX(key_btn[21],x+ww*5);
	View_setY(key_btn[21],y+hh*2);
	TextView_setTextEx(key_btn[21],"退出");
	
	/*
	View_setX(key_btn[22],x+ww*4);
	View_setY(key_btn[22],y+hh*3);
	TextView_setTextEx(key_btn[22],"桌面");
	*/
	
	//功能键
	View_setX(key_btn_tool,x+ww*5);
	View_setY(key_btn_tool,y+hh*3);
	TextView_setTextEx(key_btn_tool,"隐藏");
	
	//分辨率切换
	for(int i = 0;i<size_btn_num;i++){
		View_setX(size_btn[i],0);
		View_setY(size_btn[i],i*120);
		TextView_setTextEx(size_btn[i],size_btn_text[i]);
		View_setVisibility(size_btn[i],VISIBLE);
	}
}


int32 loadKaypad(){
	
	key_layout_father = newAbsLayout();
	setContentView(key_layout_father);
	
	//隐藏键盘
	View_setVisibility(key_layout_father,INVISIBLE);
	
	
	
	//切换分辨率布局
	size_layout = newAbsLayout();
	View_setX(size_layout,0);
	View_setY(size_layout,0);
	ViewGroup_addViewWH(key_layout_father,size_layout,MATCH_PARENT,MATCH_PARENT);
	View_setVisibility(size_layout,INVISIBLE);
	
	for(int i = 0;i<size_btn_num;i++){
		size_btn[i] = newButton();
		View_setId(size_btn[i],i+size_btn_id);
		ViewGroup_addViewWH(size_layout,size_btn[i],MATCH_PARENT,WRAP_CONTENT);
		View_setOnClickListener(size_btn[i],keybtn_onclick_callback,NULL);
	}
	
	//虚拟键盘布局
	key_layout = newAbsLayout();
	View_setX(key_layout,0);
	View_setY(key_layout,0);
	ViewGroup_addViewWH(key_layout_father,key_layout,MATCH_PARENT,MATCH_PARENT);
	
	for(int i = 0;i<key_pad_num;i++){
		key_btn[i] = newButton();
		View_setId(key_btn[i],i);
	}
	
	for(int i = 0;i<key_pad_num;i++){
		View_setOnTouchListener(key_btn[i],keypad_onclick_callback,NULL);
	}
	
	for(int i = 0;i<key_pad_num;i++){
		ViewGroup_addView(key_layout,key_btn[i]);
	}
	
	//功能键盘
	key_btn_tool = newButton();
	View_setId(key_btn_tool,key_pad_num);
	View_setOnClickListener(key_btn_tool,keybtn_onclick_callback,NULL);
	ViewGroup_addView(key_layout_father,key_btn_tool);
	
	return MR_SUCCESS;
}

int32 freeKaypad(){
	//以下仅仅删除了在c中的引用，java中任然有引用
	for(int i = 0;i<key_pad_num;i++){
		deleteButton(key_btn[i]);
	}
	for(int i = 0;i<size_btn_num;i++){
		deleteButton(size_btn[i]);
	}
	deleteButton(key_btn_tool);
	deleteAbsLayout(key_layout);
	deleteAbsLayout(key_layout_father);
	return 0;
}