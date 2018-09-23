//
// Created by john on 2018/9/22.
//

#ifndef DN_VIDEO_PLAYER_BASECHANNEL_H
#define DN_VIDEO_PLAYER_BASECHANNEL_H

#include "safe_queue.h"

extern "C"{
#include <libavcodec/avcodec.h>
};

class BaseChannel{
public:BaseChannel(int id, AVCodecContext *avCodecContext):id(id),avCodecContext(avCodecContext){}
    //抽象函数的析构函数必须是虚函数,子类必须要调用父类的析构函数
    virtual ~BaseChannel(){
        packets.setReleaseCallback(BaseChannel::releaseAvPacket);
        packets.clear();

    }

    /**
     * @param avPacket 指针的引用
     * AVPacket* 代表一个指针，添加了&代表了一个指针的引用
     */
    static void releaseAvPacket(AVPacket* & avPacket){
        if(avPacket){
            av_packet_free(&avPacket);
            //指针的指针可以修改指针的指向
            avPacket = 0;
        }
    }

    static void releaseAvFrame(AVFrame* & avFrame){
        if(avFrame){
            av_frame_free(&avFrame);
            //指针的指针可以修改指针的指向
            avFrame = 0;
        }
    }


    virtual void play() = 0;

    virtual void decode() = 0;

    int id;
    SafeQueue<AVPacket*> packets;
    pthread_t pid_decode;//解码
    pthread_t pid_render;//渲染
    bool isPlaying;
    AVCodecContext *avCodecContext;
    SafeQueue<AVFrame*> avframes;
};

#endif //DN_VIDEO_PLAYER_BASECHANNEL_H
