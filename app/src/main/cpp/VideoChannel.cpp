//
// Created by john on 2018/9/16.
//


#include "VideoChannel.h"

VideoChannel::VideoChannel(int id, AVCodecContext *avCodecContext)
        : BaseChannel(id,avCodecContext) {

}

VideoChannel::~VideoChannel(){
    avframes.setReleaseCallback(BaseChannel::releaseAvFrame);
    avframes.clear();
}

void *task_decode(void *args) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->decode();
    return 0;
}

//渲染
void *task_render(void *args) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->render();
    return 0;
}


void VideoChannel::play() {
    isPlaying = true;
    avframes.setWork(1);
    //1.解码
    pthread_create(&pid_decode, 0, task_decode, this);

    //2.播放
    pthread_create(&pid_render, 0, task_render, this);
}

/**
 * 解码
 */
void VideoChannel::decode() {
    AVPacket *avPacket = 0;
    int ret = -1;
    while (isPlaying){

        //取出一个数据包
        ret = packets.pop(avPacket);

        if(!isPlaying){
            break;
        }

        if(!ret){
            continue;
        }

        //将包丢给解码器
        ret = avcodec_send_packet(avCodecContext,avPacket);
        releaseAvPacket(avPacket);
        if(ret != 0){
            break;
        }

        //代表一个图像，现将一个图像输出来
        AVFrame *avFrame = av_frame_alloc();

        //从解码器中读取解码后的数据frame
        ret = avcodec_receive_frame(avCodecContext, avFrame);

        //需要更多的数据才能解码
        if(ret == AVERROR(EAGAIN)){
            continue;
        }else if(ret != 0){
            break;
        }

        //再次开启一个线程来进行播放(目的是为了不要阻塞解码线程)
        avframes.push(avFrame);//线程中的生产消费模式

    }

    releaseAvPacket(avPacket);
}

/**
 * 渲染
 */
void VideoChannel::render() {
    swsContext = sws_getContext(avCodecContext->width,
                                            avCodecContext->height,
    avCodecContext->pix_fmt,avCodecContext->width,avCodecContext->height,AV_PIX_FMT_RGBA,
    SWS_BILINEAR,0,0,0);
    AVFrame *avFrame = 0;
    uint8_t * dst_data[4];
    int dst_lineSize[4];

    //申请内存
    av_image_alloc(dst_data, dst_lineSize,
                   avCodecContext->width,avCodecContext->height,AV_PIX_FMT_RGBA, 1);

    int ret = -1;
    while (isPlaying){
        ret = avframes.pop(avFrame);
        if(!isPlaying){
            break;
        }

        if(!ret){
            continue;
        }


        sws_scale(swsContext,
                  reinterpret_cast<const uint8_t *const *>(avFrame->data),
                  avFrame->linesize,
                  0,
                  avCodecContext->height,
                  dst_data,
        dst_lineSize);

        //将数据交给window去绘制图像
        if(callbackListener){
            callbackListener(dst_data[0],dst_lineSize[0],avCodecContext->width,avCodecContext->height);
        }

        //释放内存
        releaseAvFrame(avFrame);
    }

    av_free(&dst_data[0]);
    releaseAvFrame(avFrame);
}
