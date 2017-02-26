//
// Created by Doom119 on 17/2/26.
//

#ifndef DAYDREAMTEST_LOG_H
#define DAYDREAMTEST_LOG_H

#include <android/log.h>

static const char* kTAG = "vulkan-rays";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

#endif //DAYDREAMTEST_LOG_H
