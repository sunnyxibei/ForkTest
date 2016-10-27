#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bspatch.h"

// 获取数组的大小
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

// 指定要注册的类，对应完整的java类名
//#define JNIREG_CLASS "com/chan/ypatchcore/YPatch"
#define JNIREG_CLASS "com/sunnyxibei/forktest/MainActivity"


#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

JNIEXPORT void JNICALL
Java_com_sunnyxibei_forktest_MainActivity_patch
        (JNIEnv *env, jclass clazz, jstring oldFile, jstring newFile, jstring patchFile) {
    int args = 4;
    char *argv[args];
    argv[0] = "bspatch";
    argv[1] = (char *) ((*env)->GetStringUTFChars(env, oldFile, 0));
    argv[2] = (char *) ((*env)->GetStringUTFChars(env, newFile, 0));
    argv[3] = (char *) ((*env)->GetStringUTFChars(env, patchFile, 0));

    //此处executePathch()就是上面我们修改出的
    run_patch(oldFile, newFile, patchFile);

    (*env)->ReleaseStringUTFChars(env, oldFile, argv[1]);
    (*env)->ReleaseStringUTFChars(env, newFile, argv[2]);
    (*env)->ReleaseStringUTFChars(env, patchFile, argv[3]);
}

JNIEXPORT void JNICALL
Java_com_sunnyxibei_forktest_MainActivity_getFork(JNIEnv *env, jobject instance) {

    int pid = fork();
    FILE *f;

    if (pid > 0) {
        //大于0的时候 创建成功
        LOGD("pid = %d", pid);
    } else if (pid == 0) {
        //int i = 0;
        while (1) {
            sleep(1);
            LOGD("pid = %d", pid);
            LOGD("ForkTest状态？ = %s", "正常");
            //获取 父进程ID
            int ppid = getppid();
            //判断父进程ID 如果Fork的父进程变成ID = 1 说明 要么卸载  要么被杀掉了
            if (ppid == 1) {
                f = fopen("/data/data/com.sunnyxibei.forktest", "r");
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
    return (*env)->NewStringUTF(env, "Hello from JNI !");
}

JNIEXPORT void JNICALL
Java_com_sunnyxibei_forktest_MainActivity_testExeclp(JNIEnv *env, jobject instance) {

    execlp("am", "am", "start", "--user", "0", "-a",
           "android.intent.action.VIEW", "-d",
           "https://sunnyxibei.github.io/", (char *) NULL);
}

// Java和JNI函数的绑定表
static JNINativeMethod method_table[] = {
        {"patch", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
                (void *) Java_com_sunnyxibei_forktest_MainActivity_patch}
};

// 注册native方法到java中
static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods,
                                 int numMethods) {
    jclass clazz;
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

int register_ndk_load(
        JNIEnv *env) {    // 调用注册方法
    return registerNativeMethods(env, JNIREG_CLASS, method_table, NELEM(method_table));
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }
    register_ndk_load(env);
    return JNI_VERSION_1_4;
}
