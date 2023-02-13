//
// Decompiled by Jadx - 1036ms
//
package person.wangchen11.nativeview;

import android.content.Context;
import android.graphics.Canvas;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;

public class NativeView extends View {
    private static final int MAX_POINTER_NUMBER = 10;
    private float[] pointersX = new float[MAX_POINTER_NUMBER];
    private float[] pointersY = new float[MAX_POINTER_NUMBER];
    private int[] pointersId = new int[MAX_POINTER_NUMBER];

    public NativeView(Context context) {
        super(context);
        NativeInterface.initView(this);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int count = event.getPointerCount();
        if (count > MAX_POINTER_NUMBER) {
            count = MAX_POINTER_NUMBER;
        }
        for (int i = 0; i < count; i++) {
            this.pointersX[i] = event.getX(i);
            this.pointersY[i] = event.getY(i);
            this.pointersId[i] = event.getPointerId(i);
        }
        return NativeInterface.touchEvent(event.getAction(), event.getX(), event.getY(), event.getActionIndex(), count, this.pointersX, this.pointersY, this.pointersId);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        NativeInterface.startDraw(canvas);
        NativeInterface.stopDraw();
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        NativeInterface.sizeChange(w, h, oldw, oldh, getContext().getResources().getDisplayMetrics().density);
        super.onSizeChanged(w, h, oldw, oldh);
    }

    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        outAttrs.imeOptions = NativeInterface.mImeOption;
        outAttrs.inputType = NativeInterface.mInputType;
        return new BaseInputConnection(this, true);
    }

    public void showSoftKeyboard() {
        setFocusableInTouchMode(true);
        requestFocus();
        InputMethodManager inputMethodManager = (InputMethodManager) getContext().getSystemService("input_method");
        inputMethodManager.showSoftInput(this, 2);
    }

    public void closeInputMethod() {
        InputMethodManager imm = (InputMethodManager) getContext().getSystemService("input_method");
        boolean isOpen = imm.isActive();
        if (isOpen) {
            imm.hideSoftInputFromWindow(getApplicationWindowToken(), 0);
        }
    }
}
