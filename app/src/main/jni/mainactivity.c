#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

JNIEXPORT void JNICALL
Java_com_sunnyxibei_forktest_MainActivity_getFork(JNIEnv *env, jobject instance) {

    // TODO
    int pid = fork();
    FILE *f;

    if (pid > 0) {
        //大于0的时候 创建成功
        LOGD("pid = %d", pid);
    } else if (pid == 0) {
        //int i = 0;
        while (1) {
            sleep(1);
            //LOGD("pid = %d", pid);
            //获取 父进程ID
            int ppid = getppid();
            //判断父进程ID 如果Fork的父进程变成ID = 1 说明 要么卸载  要么被杀掉了
            if (ppid == 1) {
                f = fopen("/data/data/com.sunnyxibei.forktest", "r");
                LOGD("ForkTest状态？ = %s", "正常");
                if (f == NULL) {
                    //被卸载了  弹出一个网页
                    //linux回收的这个进程的时候 会把里面的代码执行完毕 并强行杀死当前进程
                    execlp("am", "am", "start", "--user", "0", "-a",
                           "android.intent.action.VIEW", "-d",
                           "https://sunnyxibei.github.io/", (char *) NULL);
                } else {
                    //被杀掉了 重新开启
                    LOGD("重启代码执行了吗？ = %s", "execlp代码执行前");
                    execlp("am", "am", "start", "--user", "0", "-n",
                           "com.sunnyxibei.fork/com.sunnyxibei.forktest.MainActivity",
                           (char *) NULL);
                    LOGD("重启代码执行了吗？ = %s", "execlp代码执行后");
                }
            }

        }
    } else {
        //小于0 创建失败
        LOGD("pid = %d", pid);
    }
}

JNIEXPORT jstring JNICALL
Java_com_sunnyxibei_forktest_MainActivity_getStringFromJni(JNIEnv *env, jobject instance) {

    // TODO
    return (*env)->NewStringUTF(env, "Hello from JNI !");
}