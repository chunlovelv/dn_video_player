//
// Created by john on 2018/9/16.
//

#include "AudioChannel.h"

AudioChannel::AudioChannel(int id, AVCodecContext *avCodecContext)
        : BaseChannel(id, avCodecContext) {

}

AudioChannel::~AudioChannel() {}


void AudioChannel::play() {}
void AudioChannel::decode() {}