//
// Created by john on 2018/9/15.
//

#ifndef DN_VIDEO_PLAYER_PEOPLEFFMPEG_H
#define DN_VIDEO_PLAYER_PEOPLEFFMPEG_H

#include <pthread.h>
#include "Utils.h"
#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

extern  "C" {
#include <libavformat/avformat.h>
}

class PeopleFFmpeg {

public:
    /**
     * 构造方法
     * @param dataSource
     */
    PeopleFFmpeg(JavaCallHelper *javaCallHelper, const char *dataSource);

    /*
     * 析构方法
     */
    ~PeopleFFmpeg();


    void prepare();
    void _prepare();

    void start();
    void _start();


private:
    char *dataSource;
    pthread_t pthread;
    pthread_t pid_play;
    JavaCallHelper *javaCallHelper;
    AVFormatContext *avFormatContext;
    AudioChannel *audioChannel ;
    VideoChannel *videoChannel ;
    bool isPlaying;
    RenderFrameCallbackListener callbackListener;
public:
    void setCallbackListener(RenderFrameCallbackListener callbackListener);

};


#endif //DN_VIDEO_PLAYER_PEOPLEFFMPEG_H
