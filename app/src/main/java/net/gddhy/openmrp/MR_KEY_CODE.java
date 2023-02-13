package net.gddhy.openmrp;

import android.view.*;

public class MR_KEY_CODE{
	final public static int MR_KEY_ERR = -1;//未匹配到码值
	final static int MR_KEY_0 = 0;//按键 0
	final static int MR_KEY_1 = 1;//按键 1
	final static int MR_KEY_2 = 2;//按键 2
	final static int MR_KEY_3 = 3;//按键 3
	final static int MR_KEY_4 = 4;//按键 4
	final static int MR_KEY_5 = 5;//按键 5
	final static int MR_KEY_6 = 6;//按键 6
	final static int MR_KEY_7 = 7;//按键 7
	final static int MR_KEY_8 = 8;//按键 8
	final static int MR_KEY_9 = 9;//按键 9
	final static int MR_KEY_STAR = 10;//按键 *
	final static int MR_KEY_POUND = 11;//按键 #
	final static int MR_KEY_UP = 12;//按键 上
	final static int MR_KEY_DOWN = 13;//按键 下
	final static int MR_KEY_LEFT = 14;//按键 左
	final static int MR_KEY_RIGHT = 15;//按键 右
	final static int MR_KEY_POWER = 16;//按键 挂机键
	final static int MR_KEY_SOFTLEFT = 17;//按键 左软键
	final static int MR_KEY_SOFTRIGHT = 18;//按键 右软键
	final static int MR_KEY_SEND = 19;//按键 接听键
	final static int MR_KEY_SELECT = 20;//按键 确认/选择（若方向键中间有确认键，建议设为该键）
	final static int MR_KEY_VOLUME_UP = 21;//按键 侧键上
	final static int MR_KEY_VOLUME_DOWN = 22;//按键 侧键下
	final static int MR_KEY_CLEAR = 23;
	final static int MR_KEY_A = 24;//游戏模拟器A键
	final static int MR_KEY_B = 25;//游戏模拟器B键
	final static int MR_KEY_CAPTURE = 26;//拍照键
	final static int MR_KEY_NONE = 27;//按键 保留
	
	//电源键/音量键交给系统处理，不传递给mrp
	
	public static int androidKeyToMrpKey(int androidKeyCode){
		int mrpKey;
		switch(androidKeyCode){
			case KeyEvent.KEYCODE_0:
				mrpKey = MR_KEY_0;
				break;
			case KeyEvent.KEYCODE_1:
				mrpKey = MR_KEY_1;
				break;
			case KeyEvent.KEYCODE_2:
				mrpKey = MR_KEY_2;
				break;
			case KeyEvent.KEYCODE_3:
				mrpKey = MR_KEY_3;
				break;
			case KeyEvent.KEYCODE_4:
				mrpKey = MR_KEY_4;
				break;
			case KeyEvent.KEYCODE_5:
				mrpKey = MR_KEY_5;
				break;
			case KeyEvent.KEYCODE_6:
				mrpKey = MR_KEY_6;
				break;
			case KeyEvent.KEYCODE_7:
				mrpKey = MR_KEY_7;
				break;
			case KeyEvent.KEYCODE_8:
				mrpKey = MR_KEY_8;
				break;
			case KeyEvent.KEYCODE_9:
				mrpKey = MR_KEY_9;
				break;
			case KeyEvent.KEYCODE_STAR:
				mrpKey = MR_KEY_STAR;
				break;
			case KeyEvent.KEYCODE_POUND:
				mrpKey = MR_KEY_POUND;
				break;
			case KeyEvent.KEYCODE_DPAD_UP:
				mrpKey = MR_KEY_UP;
				break;
			case KeyEvent.KEYCODE_DPAD_DOWN:
				mrpKey = MR_KEY_DOWN;
				break;
			case KeyEvent.KEYCODE_DPAD_LEFT:
				mrpKey = MR_KEY_LEFT;
				break;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
				mrpKey = MR_KEY_RIGHT;
				break;
			case KeyEvent.KEYCODE_SOFT_LEFT:
			case KeyEvent.KEYCODE_MENU:
				mrpKey = MR_KEY_SOFTLEFT;
				break;
			case KeyEvent.KEYCODE_SOFT_RIGHT:
			case KeyEvent.KEYCODE_BACK:
				mrpKey = MR_KEY_SOFTRIGHT;
				break;
			case KeyEvent.KEYCODE_CALL:
				mrpKey = MR_KEY_SEND;
				break;
			case KeyEvent.KEYCODE_DPAD_CENTER:
			case KeyEvent.KEYCODE_ENTER:
				mrpKey = MR_KEY_SELECT;
				break;
			case KeyEvent.KEYCODE_CAMERA:
				mrpKey = MR_KEY_CAPTURE;
				break;
			case KeyEvent.KEYCODE_BUTTON_A:
				mrpKey = MR_KEY_A;
				break;
			case KeyEvent.KEYCODE_BUTTON_B:
				mrpKey = MR_KEY_B;
				break;
			default:
				mrpKey = MR_KEY_ERR;
		}
		return mrpKey;
	}
}
