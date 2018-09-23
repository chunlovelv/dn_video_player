//
// Created by john on 2018/9/15.
//

#ifndef DN_VIDEO_PLAYER_UTILS_H
#define DN_VIDEO_PLAYER_UTILS_H

#include <android/log.h>

static bool debug = true;

#define LOGTAG "PeopleLog"
#define LOG_D(...) if(debug) __android_log_print(ANDROID_LOG_DEBUG, LOGTAG, __VA_ARGS__)
#define LOG_V(...) if(debug) __android_log_print(ANDROID_LOG_VERBOSE, LOGTAG, __VA_ARGS__)
#define LOG_E(...) if(debug) __android_log_print(ANDROID_LOG_ERROR, LOGTAG, __VA_ARGS__)
#define LOG_I(...) if(debug) __android_log_print(ANDROID_LOG_INFO, LOGTAG, __VA_ARGS__)
#define LOG_W(...) if(debug) __android_log_print(ANDROID_LOG_WARN, LOGTAG, __VA_ARGS__)

//宏函数
#define DELETE(obj) if(obj){delete obj; obj = 0;}


#define THREAD_MAIN 0 //主线程
#define THREAD_CHILD 1 //子线程


//错误代码类型
//打不开视频链接地址
#define FFMPEG_CAN_NOT_OPEN_URL 0
//找不到视频流
#define FFMPEG_CAN_NOT_FIND_STREAM 1
//找不到解码器
#define FFMPEG_FIND_DECODER_FAIL 2
//找不到解码器上下文
#define FFMPEG_FIND_DECODER_CONTEXT_FAIL 3

//根据流信息配置上下文参数失败
#define FFMPEG_CODE_CONTEXT_PARAMETERS_FAIL 4

//打开解码器失败
#define FFMPEG_OPEN_DECODER_FAIL 5

//没有音频
#define FFMPEG_NO_MEDIA 6







#endif //DN_VIDEO_PLAYER_UTILS_H
