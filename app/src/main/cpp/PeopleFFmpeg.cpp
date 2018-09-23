//
// Created by john on 2018/9/15.
//
#include <string.h>
#include "PeopleFFmpeg.h"

void *task_prepare(void *args) {
    PeopleFFmpeg *peopleFFmpeg = static_cast<PeopleFFmpeg *>(args);
    //最新版本可以不使用av_register_all()
//    av_register_all();
    peopleFFmpeg->_prepare();
    return 0;
}

void *task_play(void *args) {
    PeopleFFmpeg *peopleFFmpeg = static_cast<PeopleFFmpeg *>(args);
    peopleFFmpeg->_start();
    return 0;
}

PeopleFFmpeg::PeopleFFmpeg(JavaCallHelper *javaCallHelper, const char *dataSource) {
    //字符串拷贝(不能直接使用=，应为指针会被回收掉)
    this->dataSource = new char[strlen(dataSource) + 1];
    this->javaCallHelper = javaCallHelper;
    strcpy(this->dataSource, dataSource);
}

PeopleFFmpeg::~PeopleFFmpeg() {
    //释放内存
    delete this->dataSource;

    DELETE(dataSource);
    DELETE(javaCallHelper);
}

//准备播放环境（开启子线程进行网络操作）
void PeopleFFmpeg::prepare() {
    pthread_create(&pthread, NULL, task_prepare, this);
}

void PeopleFFmpeg::_prepare() {
    //使ffmpeg联网
    avformat_network_init();
    avFormatContext = 0;
    int ret = avformat_open_input(&avFormatContext, this->dataSource, NULL, NULL);
    if (ret != 0) {
        javaCallHelper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        return;
    }

    //查找媒体中的音视频流
    ret = avformat_find_stream_info(avFormatContext, NULL);
    if (ret < 0) {
        javaCallHelper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAM);
        return;
    }


    int streamSize = avFormatContext->nb_streams;
    for (int i = 0; i < streamSize; i++) {
        //可能是音频流，可能是视频流，可能是字幕流
        AVStream *avStream = avFormatContext->streams[i];

        //包含了解码这段流的各种参数信息
        AVCodecParameters *avCodecParameters = avStream->codecpar;
        //通过当前流的使用的编码方式，查找解码器
        AVCodec *avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
        if (avCodec == NULL) {
            javaCallHelper->onError(THREAD_CHILD, FFMPEG_FIND_DECODER_FAIL);
            return;
        }

        //获得解码器的上下文信息
        AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
        if (avCodecContext == NULL) {
            javaCallHelper->onError(THREAD_CHILD, FFMPEG_FIND_DECODER_CONTEXT_FAIL);
            return;
        }

        //配置解码器上下文信息
        ret = avcodec_parameters_to_context(avCodecContext, avCodecParameters);
        if (ret < 0) {
            javaCallHelper->onError(THREAD_CHILD, FFMPEG_CODE_CONTEXT_PARAMETERS_FAIL);
            return;
        }

        //打开解码器
        ret = avcodec_open2(avCodecContext, avCodec, NULL);
        if (ret != 0) {
            javaCallHelper->onError(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            return;
        }

        if (avCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {//音频
            audioChannel = new AudioChannel(i, avCodecContext);
        } else if (avCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {//视频
            videoChannel = new VideoChannel(i, avCodecContext);
        }
    }

    if (audioChannel == NULL && videoChannel == NULL) {
        javaCallHelper->onError(THREAD_CHILD, FFMPEG_NO_MEDIA);
        return;
    }

    //准备好了，通知java层可以开始播放了
    javaCallHelper->onPrepare(THREAD_CHILD);
}


void PeopleFFmpeg::start() {

    if (videoChannel) {
        videoChannel->packets.setWork(1);
        videoChannel->play();
    }

    //开启子线程开始播放网络视频
    isPlaying = true;
    pthread_create(&pid_play, 0, task_play, this);
}

void PeopleFFmpeg::_start() {
    int ret = -1;
    //1. 读取媒体数据包（音视频数据包）
    while (isPlaying) {
        AVPacket *packet = av_packet_alloc();
        //从媒体流中读取一帧数据
        ret = av_read_frame(avFormatContext, packet);
        if (ret == 0) {//读取成功
            //stream_index是流的一个标志
            if (audioChannel && packet->stream_index == audioChannel->id) {//是音频包
                //audioChannel->packets.push(packet);
            } else if (videoChannel && packet->stream_index == videoChannel->id) {//视频包
                videoChannel->packets.push(packet);
                videoChannel->setRenderFrameCallbackListener(callbackListener);
            }

        } else if (ret == AVERROR_EOF) {//读取到最后(即读取音视频流完毕)，但有可能还没有播放完毕

        } else {//读取失败

        }
    }

    //2. 解码
}

void PeopleFFmpeg::setCallbackListener(RenderFrameCallbackListener callbackListener) {
    PeopleFFmpeg::callbackListener = callbackListener;
}
