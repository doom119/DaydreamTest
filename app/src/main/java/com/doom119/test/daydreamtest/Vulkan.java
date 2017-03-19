package com.doom119.test.daydreamtest;

import android.util.Log;
import android.view.Surface;

/**
 * Created by doom119 on 17/2/26.
 */

public class Vulkan {
    public static final String TAG = "Vulkan-rays";
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        Log.d(TAG, "loadLibrary Success");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public static native boolean createInstance();
    public static native boolean createAndroidSurface(Surface surface);
    public static native boolean createDevice();
    public static native void shutdown();
}
