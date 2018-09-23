#include <jni.h>
#include "PeopleFFmpeg.h"
//本地的窗口
#include <android/native_window_jni.h>

PeopleFFmpeg *peopleFFmpeg = 0;

JavaVM *vm;

ANativeWindow *aNativeWindow = 0;

//将window加锁（因为在主线程和子线程之间有切换）
pthread_mutex_t mLock = PTHREAD_MUTEX_INITIALIZER;

int JNI_OnLoad(JavaVM *v, void *reserved) {
    vm = v;
    return JNI_VERSION_1_6;
}


//视频渲染回调函数
void renderCallback(uint8_t *data, int lineSize, int w, int h) {
    pthread_mutex_lock(&mLock);
    if (!aNativeWindow) {
        pthread_mutex_unlock(&mLock);
        return;
    }

    //设置窗口属性
    ANativeWindow_setBuffersGeometry(aNativeWindow, w, h, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(aNativeWindow, &window_buffer, 0)) {
        ANativeWindow_release(aNativeWindow);
        aNativeWindow = 0;
        return;
    }

    //填充RGB数据给dst_data
    uint8_t *dst_data = static_cast<uint8_t *>(window_buffer.bits);
    //一行多少个数据（RGBA）
    int dst_linesize = window_buffer.stride * 4;
    for (int i = 0; i < window_buffer.height; ++i) {
        memcpy(dst_data + i * dst_linesize, data + i * lineSize, dst_linesize);
    }

    ANativeWindow_unlockAndPost(aNativeWindow);
    pthread_mutex_unlock(&mLock);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_people_health_player_PeoplePlayer_native_1prepare(JNIEnv *env, jobject instance,
                                                           jstring dataSource_) {
    const char *dataSource = env->GetStringUTFChars(dataSource_, 0);

    JavaCallHelper *javaCallHelper = new JavaCallHelper(vm, env, instance);
    //初始化播放器
    peopleFFmpeg = new PeopleFFmpeg(javaCallHelper, dataSource);
    peopleFFmpeg->setCallbackListener(renderCallback);
    peopleFFmpeg->prepare();

    env->ReleaseStringUTFChars(dataSource_, dataSource);
}



extern "C"
JNIEXPORT jstring JNICALL
Java_com_people_health_player_MainActivity_native_1string(JNIEnv *env, jobject instance) {

    // TODO

    return env->NewStringUTF(avcodec_configuration());
}



extern "C"
JNIEXPORT void JNICALL
Java_com_people_health_player_PeoplePlayer_native_1start(JNIEnv *env, jobject instance) {

    // TODO
    peopleFFmpeg->start();

}


extern "C"
JNIEXPORT void JNICALL
Java_com_people_health_player_PeoplePlayer_native_1setSurface(JNIEnv *env, jobject instance,
                                                              jobject surface) {
    pthread_mutex_lock(&mLock);
    //一旦surfaceview变换，则将原来的window释放
    if (aNativeWindow) {
        ANativeWindow_release(aNativeWindow);
        aNativeWindow = 0;
        pthread_mutex_unlock(&mLock);
    }

    aNativeWindow = ANativeWindow_fromSurface(env, surface);
    pthread_mutex_unlock(&mLock);

}