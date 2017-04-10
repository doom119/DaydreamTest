package com.doom119.test.daydreamtest;

import android.app.Activity;
import android.graphics.Canvas;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

/**
 * Created by doom119 on 17/2/26.
 */

public class VulkanTestActivity extends Activity implements SurfaceHolder.Callback {
    TextView textView;
    SurfaceView surfaceView;
    DrawThread mDrawThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_vulkan_test);
        textView = (TextView)findViewById(R.id.text_view);
        surfaceView = (SurfaceView)findViewById(R.id.surface_view);
        surfaceView.getHolder().addCallback(this);
        mDrawThread = new DrawThread(surfaceView.getHolder());
        mDrawThread.setRun(true);
        Vulkan.createInstance(getApplicationContext().getAssets());
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        if(Vulkan.createAndroidSurface(surfaceHolder.getSurface())) {
            if(Vulkan.createDevice()) {
            }
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        mDrawThread.start();

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        mDrawThread.setRun(false);
        Vulkan.shutdown();
    }

    static class DrawThread extends Thread {
        SurfaceHolder mHolder;
        volatile boolean run = false;

        public DrawThread(SurfaceHolder holder) {
            mHolder = holder;
        }

        public void setRun(boolean r) {
            run = r;
        }

        @Override
        public void run() {
            Canvas canvas = null;
            try {
                while(true) {
                    if(run) {
                        synchronized (mHolder) {
                            Vulkan.draw();
                            Thread.sleep(16);
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
