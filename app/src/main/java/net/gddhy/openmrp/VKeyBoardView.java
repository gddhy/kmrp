package net.gddhy.openmrp;

import android.content.Context;
import android.inputmethodservice.Keyboard;
import android.inputmethodservice.KeyboardView;
import android.util.AttributeSet;

import net.yzjlb.sai.kmrp.qin.*;
import android.widget.RelativeLayout.*;

public class VKeyBoardView extends KeyboardView {
	private Context context;

    public VKeyBoardView(Context context, AttributeSet attrs) {
        super(context, attrs);
		this.context = context;
        Keyboard keyboard = new Keyboard(context,R.xml.keyboard);
        setKeyboard(keyboard);
        setPreviewEnabled(false);
    }
	
	public void switchRight(){
		Keyboard keyboard = new Keyboard(context,R.xml.keyboard_right);
        setKeyboard(keyboard);
	}
	
	public void switchLeft(){
		Keyboard keyboard = new Keyboard(context,R.xml.keyboard);
        setKeyboard(keyboard);
	}
	
	public void switchMini(){
		Keyboard keyboard = new Keyboard(context,R.xml.keyboard_mini);
        setKeyboard(keyboard);
	}
	
	public void switchMiniT9(){
		Keyboard keyboard = new Keyboard(context,R.xml.keyboard_mini_t9);
        setKeyboard(keyboard);
	}
}

