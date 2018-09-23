package com.people.health.player;

import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * 作者: 李纯
 * 时间: 2018/9/15
 * 说明:
 */
public class PeoplePlayer implements SurfaceHolder.Callback {
    static {
        System.loadLibrary("native-lib");
    }

    private SurfaceView mSurfaceView;
    private String mDataSource;
    private SurfaceHolder surfaceHolder;

    public PeoplePlayer setOnPrepareListener(OnPrepareListener onPrepareListener) {
        mOnPrepareListener = onPrepareListener;
        return this;
    }

    private OnPrepareListener mOnPrepareListener;


    public void setSurfaceView(SurfaceView surfaceView) {
        mSurfaceView = surfaceView;
        surfaceHolder = mSurfaceView.getHolder();
        native_setSurface(surfaceHolder.getSurface());
        surfaceHolder.addCallback(this);
    }

    public void setDataSource(String dataSource) {
        mDataSource = dataSource;
    }


    public interface OnPrepareListener {
        void onPrepare();
    }


    public void onPrepare() {
        if (mOnPrepareListener != null) {
            mOnPrepareListener.onPrepare();
        }
    }


    /**
     * 准备好要播放的视频
     */
    public void prepare() {
        native_prepare(mDataSource);
    }

    public void start() {
        native_start();
    }

    public void stop() {

    }

    public void pause() {

    }

    public void release() {
        surfaceHolder.removeCallback(this);
    }


    public void onError(int errorCode) {
        Log.d("errorCode==> ", errorCode+"");
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }


    /**
     * 横竖屏切换或者按了home键的时候，画布会变换
     *
     * @param holder
     * @param format
     * @param width
     * @param height
     */
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        native_setSurface(holder.getSurface());
    }


    /**
     * 画布销毁（退出应用或者按了home）
     *
     * @param holder
     */
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }


    native void native_prepare(String dataSource);

    native void native_start();

    native void native_setSurface(Surface surface);
}
