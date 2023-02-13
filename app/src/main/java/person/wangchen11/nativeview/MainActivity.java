//
// Decompiled by Jadx - 881ms
//
package person.wangchen11.nativeview;

import android.app.*;
import android.content.*;
import android.inputmethodservice.*;
import android.os.*;
import android.preference.*;
import android.view.*;
import android.view.View.*;
import android.widget.*;
import java.io.*;
import net.gddhy.openmrp.*;

public class MainActivity extends net.gddhy.openmrp.BaseActivity {

	static final int KY_DOWN = 0;
    static final int KY_UP = 1;
    static final int MENU_RET = 5;
    static final int MENU_SLT = 4;
    static final int MR_DIALOG = 6;
    static final int MS_DOWN = 2;
    static final int MS_MOVE = 12;
    static final int MS_UP = 3;

    NativeView mNativeView;
    private RelativeLayout mRelativeLayout = null;
    private View mFakeContentView = null;
	
	private int vKeyType = 0;
	SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		//不存在dsm时创建dsm
		File dsm = net.gddhy.openmrp.MainActivity.getDSM_GM_File();
		if(!dsm.exists()){
			try {
				InputStream inputStream = getAssets().open("dsm.mrp");
				net.gddhy.openmrp.MainActivity.streamToDSM(inputStream);
			}catch (IOException e){
				
			}
		}
		
		
        this.mNativeView = new NativeView(this);
        this.mRelativeLayout = new RelativeLayout(this);
        this.mRelativeLayout.addView(this.mNativeView);
        super.setContentView(this.mRelativeLayout);
        NativeInterface.initActivity(this);
		
		//针对多亲1s调整 全屏显示
		if(Build.MODEL.equals("Qin 1s+")){
			getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		}
		
		sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
		if(sharedPreferences.getBoolean("虚拟键盘",false)){
			//虚拟键盘显示
			//虚拟键盘改到linear中显示，实现全面屏键盘提高
			final LinearLayout linearlayout = new LinearLayout(this);
			RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT);
			linearlayout.setOrientation(LinearLayout.VERTICAL);
			linearlayout.setGravity(Gravity.CENTER);
			layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);//设置置底
			
			
			final VKeyBoardView vKey = new VKeyBoardView(this,null);
			RelativeLayout.LayoutParams lpVkey = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
			//lpVkey.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);//设置置底
			linearlayout.addView(vKey);
			final Button button = new Button(this);
			button.setLayoutParams(lpVkey);
			button.setText("显示虚拟键盘");
			button.setVisibility(View.GONE);
			
			vKeyType = sharedPreferences.getInt("vKeyType",0);
			vKeyType--;
			vKeySwitch(vKey,button);
			
			linearlayout.addView(button);
			
			int fullScrType = sharedPreferences.getInt("全面屏键盘优化",0);
			for(int i = 0;i<fullScrType;i++){
				TextView textView = new TextView(this);
				linearlayout.addView(textView,lpVkey);
			}
			
			
			mRelativeLayout.addView(linearlayout,layoutParams);
			final Vibrator vibrator = (Vibrator)getSystemService(VIBRATOR_SERVICE);
			vKey.setOnKeyboardActionListener(new KeyboardView.OnKeyboardActionListener() {
					@Override
					public void onPress(int i) {
						//按键按下
						vibrator.vibrate(10);
						if(i>=0){
							NativeInterface.keyEvent(KY_DOWN, i);
						}
					}

					@Override
					public void onRelease(int i) {
						//按键抬起
						if(i>=0){
							NativeInterface.keyEvent(KY_UP, i);
						}
					}

					@Override
					public void onKey(int i, int[] ints) {
						//按键事件
						switch (i){
							case -1:
								vKeySwitch(vKey,button);
								break;
							case -2:
								new AlertDialog.Builder(MainActivity.this)
									.setTitle("温馨提示")
									.setMessage("你确定要退出吗？\nMrp程序将会终止，注意保存游戏数据")
									.setPositiveButton("确定", new DialogInterface.OnClickListener() {
										@Override
										public void onClick(DialogInterface dialogInterface, int i) {
											finish();
										}
									})
									.setNegativeButton("取消",null)
									.setNeutralButton("全面屏键盘优化", new DialogInterface.OnClickListener(){
										@Override
										public void onClick(DialogInterface dialogInterface, int i) {
											alertDialogFullScr(linearlayout,vKey,button);
										}
									})
									.show();
								break;
							case -3:
								alertDialogFullScr(linearlayout,vKey,button);
								break;
						}
					}

					@Override
					public void onText(CharSequence charSequence) {

					}

					//按下后滑动
					@Override
					public void swipeLeft() {

					}

					@Override
					public void swipeRight() {

					}

					@Override
					public void swipeDown() {

					}

					@Override
					public void swipeUp() {

					}
				});
			button.setOnClickListener(new OnClickListener(){
				@Override
				public void onClick(View v){
					vKeySwitch(vKey,button);
				}
			});
		}
    }
	
	private void vKeySwitch(VKeyBoardView vKey,Button button){
		vKeyType++;
		if(vKeyType >4){
			vKeyType = 0;
		}
		switch(vKeyType){
			case 0:
				vKey.switchLeft();
				vKey.setVisibility(View.VISIBLE);
				button.setVisibility(View.GONE);
				break;
			case 1:
				vKey.switchRight();
				vKey.setVisibility(View.VISIBLE);
				button.setVisibility(View.GONE);
				break;
			case 2:
				vKey.switchMini();
				vKey.setVisibility(View.VISIBLE);
				button.setVisibility(View.GONE);
				break;
			case 3:
				vKey.switchMiniT9();
				vKey.setVisibility(View.VISIBLE);
				button.setVisibility(View.GONE);
				break;
			default:
			//case 3
				vKey.setVisibility(View.GONE);
				button.setVisibility(View.VISIBLE);
		}
		SharedPreferences.Editor editor = sharedPreferences.edit();
		editor.putInt("vKeyType",vKeyType);
		editor.apply();
	}
	
	private void alertDialogFullScr(final LinearLayout linearlayout,final VKeyBoardView vKeyView,final Button button){
		final NumberPicker numberPicker = new NumberPicker(this);
		numberPicker.setMinValue(0);
		numberPicker.setMaxValue(10);
		numberPicker.setWrapSelectorWheel(false);
		numberPicker.setValue(sharedPreferences.getInt("全面屏键盘优化",0));
		
		new AlertDialog.Builder(MainActivity.this)
			.setTitle("全面屏键盘优化")
			.setMessage("全面屏手机可能出现键盘过于贴近屏幕下方，触摸不变，可以在此处提高键盘高度")
			.setView(numberPicker)
			.setPositiveButton("确定", new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialogInterface, int value) {
					SharedPreferences.Editor editor = sharedPreferences.edit();
					editor.putInt("全面屏键盘优化",numberPicker.getValue());
					editor.apply();
					
					linearlayout.removeAllViews();
					linearlayout.addView(vKeyView);
					linearlayout.addView(button);
					
					for(int i = 0;i<numberPicker.getValue();i++){
						TextView textView = new TextView(MainActivity.this);
						linearlayout.addView(textView,new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
					}
				}
			})
			.setNegativeButton("取消",null)
			.show();
	}
	
	@Override
	public void finish(){
		super.finish();
		File file = net.gddhy.openmrp.MainActivity.getDSM_GM_File();
		if (file.exists()){
        	file.delete();
        }
		android.os.Process.killProcess(android.os.Process.myPid());
	}

    @Override
    protected void onPause() {
        NativeInterface.onPause();
        super.onPause();
    }

    @Override
    protected void onResume() {
        NativeInterface.onResume();
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        NativeInterface.destroy();
        super.onDestroy();
    }

    @Override
    public void onBackPressed() {
        if (!NativeInterface.backPressed()) {
            super.onBackPressed();
        }
    }

    @Override
    public void setContentView(View view) {
        if (this.mFakeContentView != null) {
            this.mRelativeLayout.removeView(this.mFakeContentView);
            this.mFakeContentView = null;
        }
        this.mFakeContentView = view;
        this.mRelativeLayout.addView(view, -1, -1);
    }
	
	private void showQinMenu(){
		new AlertDialog.Builder(this)
			.setItems(new String[]{"声音设置","关闭模拟器"}, new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialog, int which) {
					switch (which){
						case 0:
							Intent intent = new Intent("android.settings.SOUND_SETTINGS");
							startActivity(Intent.createChooser(intent,"音量设置"));
							break;
						case 1:
							finish();
							break;
					}
				}
			})
			.show();
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent keyEvent) {
		//多亲1s拨号键跳转音量设置
		if(Build.MODEL.equals("Qin 1s+")&&keyCode==KeyEvent.KEYCODE_CALL){
			showQinMenu();
			return true;
		}
		
		int mrpKey = MR_KEY_CODE.androidKeyToMrpKey(keyCode);
		if(mrpKey != MR_KEY_CODE.MR_KEY_ERR){
        	NativeInterface.keyEvent(KY_DOWN, mrpKey);
        	return true;
		} else {
			return super.onKeyDown(keyCode,keyEvent);
		}
    }

    public boolean onKeyUp(int keyCode, KeyEvent keyEvent) {
		//多亲1s屏蔽拨号键抬起
		if(Build.MODEL.equals("Qin 1s+")&&keyCode==KeyEvent.KEYCODE_CALL){
			return true;
		}
		int mrpKey = MR_KEY_CODE.androidKeyToMrpKey(keyCode);
        if(mrpKey != MR_KEY_CODE.MR_KEY_ERR){
        	NativeInterface.keyEvent(KY_UP, mrpKey);
        	return true;
		} else {
			return super.onKeyUp(keyCode,keyEvent);
		}
    }
}
