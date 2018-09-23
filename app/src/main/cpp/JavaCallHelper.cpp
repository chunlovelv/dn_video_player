//
// Created by john on 2018/9/16.
//

#include "JavaCallHelper.h"
#include "Utils.h"

JavaCallHelper::JavaCallHelper(JavaVM *vm, JNIEnv *env, jobject instance) {
    this->jniEnv = env;
    this->javaVM = vm;
    this->mGlobelInstance = env->NewGlobalRef(instance);
    this->mClass = env->GetObjectClass(instance);
    this->mOnErrorId = env->GetMethodID(mClass,"onError","(I)V");
    mOnPrepareId = env->GetMethodID(mClass,"onPrepare","()V");
}

JavaCallHelper::~JavaCallHelper() {
    if(mGlobelInstance != NULL){
        this->jniEnv->DeleteGlobalRef(mGlobelInstance);
    }

}


void JavaCallHelper::onError(int thread, int errorCode) {
    //回调java的onError方法
    if(thread == THREAD_MAIN){
        jniEnv->CallVoidMethod(mGlobelInstance, mOnErrorId, errorCode);
    }else if(thread == THREAD_CHILD){
        //子线程
        JNIEnv *childEnv;
        //将当前线程附加到java线程中
        javaVM->AttachCurrentThread(&childEnv,NULL);
        childEnv->CallVoidMethod(mGlobelInstance, mOnErrorId, errorCode);
        javaVM->DetachCurrentThread();
    }
}


void JavaCallHelper::onPrepare(int thread) {
    if(thread == THREAD_MAIN){
        jniEnv->CallVoidMethod(mGlobelInstance, mOnPrepareId);
    } else if(thread == THREAD_CHILD){
//子线程
        JNIEnv *childEnv;
        //将当前线程附加到java线程中
        javaVM->AttachCurrentThread(&childEnv,NULL);
        childEnv->CallVoidMethod(mGlobelInstance, mOnPrepareId);
        javaVM->DetachCurrentThread();
    }
}



