package com.doom119.test.daydreamtest;

import android.app.Activity;
import android.opengl.GLES20;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.google.vr.sdk.base.Eye;
import com.google.vr.sdk.base.GvrActivity;
import com.google.vr.sdk.base.GvrView;
import com.google.vr.sdk.base.HeadTransform;
import com.google.vr.sdk.base.Viewport;

import javax.microedition.khronos.egl.EGLConfig;

public class MainActivity extends GvrActivity implements GvrView.StereoRenderer{

    public static final String TAG = "rays";

    GvrView mGvrView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Vulkan.stringFromJNI();

        mGvrView = (GvrView)findViewById(R.id.gvr_view);
        mGvrView.setEGLConfigChooser(8, 8, 8, 8, 16, 8);
        mGvrView.setRenderer(this);
        mGvrView.setTransitionViewEnabled(true);
        setGvrView(mGvrView);
    }

    @Override
    public void onDrawEye(Eye eye) {
    }

    @Override
    public void onNewFrame(HeadTransform headTransform) {

    }

    @Override
    public void onFinishFrame(Viewport viewport) {

    }

    @Override
    public void onSurfaceChanged(int i, int i1) {

    }

    @Override
    public void onSurfaceCreated(EGLConfig eglConfig) {
        Log.d(TAG, "onSurfaceCreated");
        GLES20.glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
    }

    @Override
    public void onRendererShutdown() {

    }


}
