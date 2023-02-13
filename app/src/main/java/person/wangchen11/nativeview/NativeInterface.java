//
// Decompiled by Jadx - 974ms
//
package person.wangchen11.nativeview;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.media.MediaPlayer;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.Toast;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;

public class NativeInterface {
    static final String TAG = "NativeInterface";
    private static Activity mActivity;
    private static AlertDialog mAlertDialog;
    private static Rect mBounds;
    private static EditText mEditText;
    public static int mImeOption;
    public static int mInputType;
    private static NativeView mNativeView;
    private static RectF mRectF;
    private static Rect mRectFrom;
    private static RectF mRectTo;
    private static Toast mToast;
    private static Canvas mCanvas = null;
    private static Paint mPaint = new Paint();
    private static float mDescent = 0.0f;

    public static native boolean backPressed();

    public static native void destroy();

    private static native void draw(int i, int i2, int i3, int i4, Canvas canvas);

    private static native void init(Activity activity);

    public static native void onEditTextCancel(String str, int i);

    public static native void onEditTextSure(String str, int i);

    public static native void onLoopCall();

    public static native void onPause();

    public static native void onResume();

    public static native void sizeChange(int i, int i2, int i3, int i4, float f);

    public static native boolean touchEvent(int i, float f, float f2, int i2, int i3, float[] fArr, float[] fArr2, int[] iArr);
	
	public static native boolean keyEvent(int action, int key);

	
    static {
        try {
            System.loadLibrary("NativeActivity");
        } catch (Error e) {
            e.printStackTrace();
        } catch (Exception e2) {
            e2.printStackTrace();
        }
        mBounds = new Rect();
        mRectF = new RectF();
        mRectFrom = new Rect();
        mRectTo = new RectF();
        mImeOption = 0x50000006;
        mInputType = 1;
    }

    public static void initActivity(Activity activity) {
        mActivity = activity;
        try {
            init(mActivity);
        } catch (Throwable e) {
            Toast.makeText(mActivity, e.toString(), 1).show();
        }
    }

    public static void initView(NativeView paramNativeView) {
		mNativeView = paramNativeView;
		mNativeView.postDelayed(new Runnable()
			{
				public void run() {
					try {
						NativeInterface.onLoopCall();
						NativeInterface.mNativeView.postDelayed(this, 1L);
						return;
					} catch (Throwable localThrowable) {
						Toast.makeText(NativeInterface.mActivity, localThrowable.toString(), 1).show();
					}
				}
			}, 1L);
	}


    public static void startDraw(Canvas canvas) {
        mCanvas = canvas;
        canvas.getClipBounds(mBounds);
        draw(mBounds.left, mBounds.top, mBounds.right, mBounds.bottom, canvas);
    }

    public static void stopDraw() {
        mCanvas = null;
    }

    public static void setStrokeWidth(float width) {
        mPaint.setStrokeWidth(width);
    }

    public static void setStroke(boolean stroke) {
        if (stroke) {
            mPaint.setStyle(Paint.Style.STROKE);
        } else {
            mPaint.setStyle(Paint.Style.FILL);
        }
    }

    public static void drawColor(int color) {
        mCanvas.drawColor(color);
    }

    public static void setColor(int color) {
        mPaint.setColor(color);
    }

    public static void setTextSize(float size) {
        mPaint.setTextSize(size);
        mDescent = mPaint.getFontMetrics().descent;
    }

    public static void drawLine(float startX, float startY, float stopX, float stopY) {
        mCanvas.drawLine(startX, startY, stopX, stopY, mPaint);
    }

    public static void drawText(String text, int start, int end, float x, float y) {
        mCanvas.drawText(text, start, end, x, y - mDescent, mPaint);
    }

    public static void drawRect(float left, float top, float right, float bottom) {
        mCanvas.drawRect(left, top, right, bottom, mPaint);
    }

    public static void drawRoundRect(float left, float top, float right, float bottom, float rx, float ry) {
        mRectF.set(left, top, right, bottom);
        mCanvas.drawRoundRect(mRectF, rx, ry, mPaint);
    }

    public static void invalidate() {
        mNativeView.invalidate();
    }

    public static void postInvalidate() {
        mNativeView.postInvalidate();
    }

    public static void invalidateRect(int left, int top, int right, int bottom) {
        mNativeView.invalidate(left, top, right, bottom);
    }

    public static void postInvalidateRect(int left, int top, int right, int bottom) {
        mNativeView.postInvalidate(left, top, right, bottom);
    }

    public static Bitmap decodeBitmapFromAssets(String name) {
        if (DebugInfo.mAssetsPath != null) {
            return decodeBitmapFromFile(String.valueOf(DebugInfo.mAssetsPath) + "/" + name);
        }
        Bitmap bmp = null;
        try {
            InputStream in = mActivity.getAssets().open(name);
            try {
                bmp = BitmapFactory.decodeStream(in).copy(Bitmap.Config.ARGB_8888, true);
            } catch (OutOfMemoryError error) {
                error.printStackTrace();
            }
            in.close();
            return bmp;
        } catch (IOException e) {
            e.printStackTrace();
            return bmp;
        }
    }

    public static Bitmap decodeBitmapFromFile(String path) {
        Bitmap bmp = null;
        try {
            InputStream in = new FileInputStream(path);
            try {
                bmp = BitmapFactory.decodeStream(in).copy(Bitmap.Config.ARGB_8888, true);
            } catch (OutOfMemoryError error) {
                error.printStackTrace();
            }
            try {
                in.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (FileNotFoundException e2) {
            e2.printStackTrace();
        }
        return bmp;
    }

    public static boolean saveBitmapToFile(Bitmap bitmap, String path, int format, int quality) {
        FileOutputStream fileOutputStream;
        Bitmap.CompressFormat compressFormat = Bitmap.CompressFormat.PNG;
        if (format == 0) {
            compressFormat = Bitmap.CompressFormat.JPEG;
        }

        try {
            fileOutputStream = new FileOutputStream(path);
            bitmap.compress(compressFormat, quality, fileOutputStream);
            if (fileOutputStream != null) {
				fileOutputStream.close();  
            }
            return true;
        } catch (Exception e4) {
            return false;
        }
    }

    public static Bitmap createBitmap(int width, int height) {
        try {
            Bitmap bmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            return bmp;
        } catch (OutOfMemoryError e) {
            e.printStackTrace();
            return null;
        }
    }

    public static float measureText(String text, int start, int end) {
        return mPaint.measureText(text, start, end);
    }

    public static void drawBitmap(Bitmap bitmap, int fromLeft, int fromTop, int fromRight, int fromBottom, float toLeft, float toTop, float toRight, float toBottom) {
        mRectFrom.set(fromLeft, fromTop, fromRight, fromBottom);
        mRectTo.set(toLeft, toTop, toRight, toBottom);
        mCanvas.drawBitmap(bitmap, mRectFrom, mRectTo, mPaint);
    }

    public static void finish() {
        mActivity.finish();
    }

    public static boolean requestEditText(String title, String string, int cur) {
        if (mAlertDialog != null && mAlertDialog.isShowing()) {
            return false;
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(mActivity);
        builder.setTitle(title);
        mEditText = new EditText(mActivity);
        mEditText.setText(string);
        mEditText.setSelection(cur);
        ScrollView scrollView = new ScrollView(mActivity);
        LinearLayout linearLayout = new LinearLayout(mActivity);
        linearLayout.addView(mEditText, -1, -2);
        scrollView.addView(linearLayout);
        builder.setView(scrollView);

		builder.setNegativeButton("取消", new DialogInterface.OnClickListener()
			{
				public void onClick(DialogInterface paramAnonymousDialogInterface, int paramAnonymousInt) {
					NativeInterface.onEditTextCancel(NativeInterface.mEditText.getText().toString(), NativeInterface.mEditText.getSelectionEnd());
				}
			});
		builder.setPositiveButton("确定", new DialogInterface.OnClickListener()
			{
				public void onClick(DialogInterface paramAnonymousDialogInterface, int paramAnonymousInt) {
					NativeInterface.onEditTextSure(NativeInterface.mEditText.getText().toString(), NativeInterface.mEditText.getSelectionEnd());
				}
			});

        mAlertDialog = builder.create();
        mAlertDialog.setCancelable(false);
        mAlertDialog.show();
        return true;
    }

    public static void showToast(String string, int time) {
        if (mToast != null) {
            mToast.cancel();
        }
        mToast = Toast.makeText(mActivity, string, time);
        mToast.show();
    }

    public static long currentTimeMillis() {
        return System.currentTimeMillis();
    }

    public static long nanoTime() {
        return System.nanoTime();
    }

    public static byte[] readAllFromAssets(String name) {
        if (DebugInfo.mAssetsPath != null) {
            return readAllFromFile(String.valueOf(DebugInfo.mAssetsPath) + "/" + name);
        }
        byte[] data = null;
        try {
            InputStream inputStream = mActivity.getAssets().open(name);
            data = new byte[inputStream.available()];
            inputStream.read(data);
            inputStream.close();
            return data;
        } catch (IOException e) {
            e.printStackTrace();
            return data;
        }
    }

    public static byte[] readAllFromFile(String name) {
        byte[] data = null;
        try {
            InputStream inputStream = new FileInputStream(new File(name));
            data = new byte[inputStream.available()];
            inputStream.read(data);
            inputStream.close();
            return data;
        } catch (IOException e) {
            e.printStackTrace();
            return data;
        }
    }

    public static MediaPlayer createMediaPlayer() {
        return new MyMediaPlayer();
    }

    public static boolean mediaPlayerSetSourceFromAssert(MediaPlayer mediaPlayer, String path) {
        if (DebugInfo.mAssetsPath != null) {
            return mediaPlayerSetSourceFromPath(mediaPlayer, String.valueOf(DebugInfo.mAssetsPath) + "/" + path);
        }
        try {
            AssetFileDescriptor assetFileDescriptor = mActivity.getAssets().openFd(path);
            if (assetFileDescriptor == null) {
                return false;
            }
            mediaPlayer.setDataSource(assetFileDescriptor.getFileDescriptor(), assetFileDescriptor.getStartOffset(), assetFileDescriptor.getLength());
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        } catch (IllegalArgumentException e2) {
            e2.printStackTrace();
            return false;
        } catch (IllegalStateException e3) {
            e3.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerSetSourceFromPath(MediaPlayer mediaPlayer, String path) {
        try {
            mediaPlayer.setDataSource(path);
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        } catch (IllegalArgumentException e2) {
            e2.printStackTrace();
            return false;
        } catch (IllegalStateException e3) {
            e3.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerPrepare(MediaPlayer mediaPlayer) {
        try {
            mediaPlayer.prepare();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        } catch (IllegalStateException e2) {
            e2.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerStart(MediaPlayer mediaPlayer) {
        try {
            mediaPlayer.start();
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerStop(MediaPlayer mediaPlayer) {
        try {
            mediaPlayer.stop();
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerReset(MediaPlayer mediaPlayer) {
        try {
            mediaPlayer.reset();
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerRlease(MediaPlayer mediaPlayer) {
        try {
            mediaPlayer.release();
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerPause(MediaPlayer mediaPlayer) {
        try {
            mediaPlayer.pause();
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerSetLooping(MediaPlayer mediaPlayer, boolean looping) {
        try {
            mediaPlayer.setLooping(looping);
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerIsLooping(MediaPlayer mediaPlayer) {
        try {
            return mediaPlayer.isLooping();
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerIsPlaying(MediaPlayer mediaPlayer) {
        try {
            return mediaPlayer.isPlaying();
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerSetVolume(MediaPlayer mediaPlayer, float leftVolume, float rightVolume) {
        try {
            mediaPlayer.setVolume(leftVolume, rightVolume);
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean mediaPlayerSeekTo(MediaPlayer mediaPlayer, int msec) {
        try {
            mediaPlayer.seekTo(msec);
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static int mediaPlayerGetDuration(MediaPlayer mediaPlayer) {
        try {
            return mediaPlayer.getDuration();
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return -1;
        }
    }

    public static int mediaPlayerGetCurrentPosition(MediaPlayer mediaPlayer) {
        try {
            return mediaPlayer.getCurrentPosition();
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return -1;
        }
    }

    public static int mediaPlayerGetVedioWidth(MediaPlayer mediaPlayer) {
        try {
            return mediaPlayer.getVideoWidth();
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }
    }

    public static int mediaPlayerGetVedioHeight(MediaPlayer mediaPlayer) {
        try {
            return mediaPlayer.getVideoHeight();
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }
    }

    public static void canvasSave() {
        mCanvas.save();
    }

    public static void canvasRestore() {
        mCanvas.restore();
    }

    public static void canvasClipRect(float left, float top, float right, float bottom) {
        mCanvas.clipRect(left, top, right, bottom);
    }

    public static void canvasRotate(float degrees, float px, float py) {
        mCanvas.rotate(degrees, px, py);
    }

    public static void canvasTranslate(float dx, float dy) {
        mCanvas.translate(dx, dy);
    }

    public static void canvasScale(float sx, float sy, float px, float py) {
        mCanvas.scale(sx, sy, px, py);
    }

    public static void showInputMethod(int imeOptions, int inputType) {
        mImeOption = imeOptions;
        mInputType = inputType;
        mNativeView.showSoftKeyboard();
    }

    public static void closeInputMethod() {
        mNativeView.closeInputMethod();
    }

    public static float getDensity() {
        return mActivity.getResources().getDisplayMetrics().density;
    }
}
