//
// Decompiled by Jadx - 659ms
//
package person.wangchen11.nativeview;

import android.graphics.Canvas;
import android.media.MediaPlayer;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import person.wangchen11.nativeview.MyView;

public class MyListener implements View.OnClickListener, View.OnLongClickListener, View.OnKeyListener, View.OnTouchListener, MyView.OnDrawListener, MediaPlayer.OnCompletionListener {
    private int mDataAddr;
    private int mFunAddr;

    public static native void draw(int i, int i2, View view, Canvas canvas);

    public static native void onClick(int i, int i2, View view);

    public static native void onCompletion(int i, int i2, MediaPlayer mediaPlayer);

    public static native boolean onKey(int i, int i2, View view, int i3, KeyEvent keyEvent);

    public static native boolean onLongClick(int i, int i2, View view);

    public static native boolean onTouch(int i, int i2, View view, int i3, float f, float f2, MotionEvent motionEvent);

    public MyListener(int fun, int data) {
        this.mFunAddr = 0;
        this.mDataAddr = 0;
        this.mFunAddr = fun;
        this.mDataAddr = data;
    }

    @Override
    public void onClick(View view) {
        onClick(this.mFunAddr, this.mDataAddr, view);
    }

    @Override
    public boolean onLongClick(View view) {
        return onLongClick(this.mFunAddr, this.mDataAddr, view);
    }

    @Override
    public boolean onKey(View view, int key, KeyEvent keyEvent) {
        return onKey(this.mFunAddr, this.mDataAddr, view, key, keyEvent);
    }

    @Override
    public boolean onTouch(View view, MotionEvent event) {
        return onTouch(this.mFunAddr, this.mDataAddr, view, event.getAction(), event.getX(), event.getY(), event);
    }

    public void draw(View view, Canvas canvas) {
        draw(this.mFunAddr, this.mDataAddr, view, canvas);
    }

    @Override
    public void onCompletion(MediaPlayer mp) {
        onCompletion(this.mFunAddr, this.mDataAddr, mp);
    }
}
