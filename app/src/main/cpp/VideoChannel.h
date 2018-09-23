//
// Created by john on 2018/9/16.
//

#ifndef DN_VIDEO_PLAYER_VIDEOCHANNEL_H
#define DN_VIDEO_PLAYER_VIDEOCHANNEL_H


#include "BaseChannel.h"

extern "C" {

#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
};

//函数指针
typedef void (*RenderFrameCallbackListener)(uint8_t *,int,int,int);

class VideoChannel : public BaseChannel {
public:
    VideoChannel(int id, AVCodecContext *avCodecContext);

    ~VideoChannel();

    void play();

    void decode();

    void render();

    SwsContext *swsContext;
    RenderFrameCallbackListener callbackListener;


    void setRenderFrameCallbackListener(RenderFrameCallbackListener listener){
        this->callbackListener = listener;
    }
};


#endif //DN_VIDEO_PLAYER_VIDEOCHANNEL_H
