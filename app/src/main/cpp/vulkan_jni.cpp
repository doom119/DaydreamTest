//
// Created by Doom119 on 17/2/28.
//
#include <jni.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Vulkan.h"

#ifdef __cplusplus
extern "C" {
#endif

Vulkan gVulkan;

JNIEXPORT jboolean JNICALL
Java_com_doom119_test_daydreamtest_Vulkan_createAndroidSurface(JNIEnv *env, jclass type, jobject surface)
{
    return gVulkan.createAndroidSurface(ANativeWindow_fromSurface(env, surface));
}

JNIEXPORT jboolean JNICALL
Java_com_doom119_test_daydreamtest_Vulkan_createInstance(JNIEnv *env, jclass type, jobject assets)
{
    return gVulkan.createInstance(AAssetManager_fromJava(env, assets));
}

JNIEXPORT jboolean JNICALL
Java_com_doom119_test_daydreamtest_Vulkan_createDevice(JNIEnv *env, jclass type)
{
    return gVulkan.createDevice();
}

JNIEXPORT void JNICALL
Java_com_doom119_test_daydreamtest_Vulkan_shutdown(JNIEnv *env, jclass type)
{
    gVulkan.shutdown();
}

#ifdef __cplusplus
}
#endif