package com.doom119.test.daydreamtest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

/**
 * Created by doom119 on 17/2/26.
 */

public class VulkanTestActivity extends Activity implements SurfaceHolder.Callback {
    TextView textView;
    SurfaceView surfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_vulkan_test);
        textView = (TextView)findViewById(R.id.text_view);
        surfaceView = (SurfaceView)findViewById(R.id.surface_view);
        surfaceView.getHolder().addCallback(this);

        Vulkan.init();
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        Vulkan.setSurface(surfaceHolder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }
}
