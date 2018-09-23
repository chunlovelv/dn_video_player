//
// Created by john on 2018/9/16.
//

#ifndef DN_VIDEO_PLAYER_AUDIOCHANNEL_H
#define DN_VIDEO_PLAYER_AUDIOCHANNEL_H


#include "BaseChannel.h"

class AudioChannel: public BaseChannel {
public:
    AudioChannel(int id,AVCodecContext *avCodecContext);
    ~AudioChannel();
    void play();
    void decode();
};


#endif //DN_VIDEO_PLAYER_AUDIOCHANNEL_H
