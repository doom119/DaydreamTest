//
// Created by Doom119 on 17/2/26.
//

#ifndef DAYDREAMTEST_LOG_H
#define DAYDREAMTEST_LOG_H

#include <android/log.h>

static const char* kTAG = "vulkan-rays";
#define LOGD(...) \
    ((void)__android_log_print(ANDROID_LOG_DEBUG, kTAG, __VA_ARGS__))
#define LOGI(...) \
    ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
    ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
    ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

#define VK_RESULTCHECK(vkFunc, ...) \
    VkResult result = vkFunc(__VA_ARGS__); \
    if(VK_SUCCESS != result) \
    { \
        LOGW("##__FUNCTION__##Failed, result=%d##", result); \
        return false; \
    } \
    LOGI("%s Success", #vkFunc); \
    return true;

#endif //DAYDREAMTEST_LOG_H
