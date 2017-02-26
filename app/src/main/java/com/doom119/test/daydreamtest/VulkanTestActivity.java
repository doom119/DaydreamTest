package com.doom119.test.daydreamtest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

/**
 * Created by doom119 on 17/2/26.
 */

public class VulkanTestActivity extends Activity {
    TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_vulkan_test);
        textView = (TextView)findViewById(R.id.text_view);
        textView.setText(Vulkan.stringFromJNI());
    }
}
