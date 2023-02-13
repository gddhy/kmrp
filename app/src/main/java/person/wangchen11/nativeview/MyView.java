//
// Decompiled by Jadx - 536ms
//
package person.wangchen11.nativeview;

import android.content.Context;
import android.graphics.Canvas;
import android.view.View;

public class MyView extends View {
    private OnDrawListener mDrawListener = null;

    public MyView(Context context) {
        super(context);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (this.mDrawListener == null) {
            super.onDraw(canvas);
        } else {
            this.mDrawListener.draw(this, canvas);
        }
    }

    public void setOnDrawListener(OnDrawListener drawListener) {
        this.mDrawListener = drawListener;
    }
    
    public interface OnDrawListener {
    void draw(View view, Canvas canvas);
    
    }
}
