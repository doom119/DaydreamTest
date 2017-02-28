//
// Created by Doom119 on 17/2/28.
//
#include <jni.h>
#include <android/native_window_jni.h>
#include "Vulkan.h"

#ifdef __cplusplus
extern "C" {
#endif

Vulkan gVulkan;

JNIEXPORT jboolean JNICALL
Java_com_doom119_test_daydreamtest_Vulkan_setSurface(JNIEnv *env, jclass type, jobject surface)
{
    return gVulkan.setSurface(ANativeWindow_fromSurface(env, surface));
}

JNIEXPORT jboolean JNICALL
Java_com_doom119_test_daydreamtest_Vulkan_init(JNIEnv *env, jclass type)
{
    return gVulkan.init();
}

#ifdef __cplusplus
}
#endif