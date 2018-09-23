//
// Created by john on 2018/9/16.
//

#ifndef DN_VIDEO_PLAYER_JAVACALLHELPER_H
#define DN_VIDEO_PLAYER_JAVACALLHELPER_H


#include <jni.h>

class JavaCallHelper {

public:
    JavaCallHelper(JavaVM *vm, JNIEnv *env, jobject instance);

    ~JavaCallHelper();

    void onError(int thread, int errorCode);


    void onPrepare(int thread);


private:
    JavaVM *javaVM;
    JNIEnv *jniEnv;
    jobject mGlobelInstance;
    jclass mClass;
    jmethodID mOnErrorId;
    jmethodID mOnPrepareId;
};


#endif //DN_VIDEO_PLAYER_JAVACALLHELPER_H
