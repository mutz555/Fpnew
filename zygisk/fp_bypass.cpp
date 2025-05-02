#include <zygisk.hpp>
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "FpBypass"
#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##__VA_ARGS__)

// Hooked native method - always return true
jboolean isHardwareDetected(JNIEnv *env, jobject thiz, jstring pkg) {
    LOGD("Hooked isHardwareDetected called!");
    return JNI_TRUE;
}

jint canAuthenticate(JNIEnv *env, jobject thiz, jint authType) {
    LOGD("Hooked canAuthenticate called!");
    return 0; // BIOMETRIC_SUCCESS
}

static JNINativeMethod methods[] = {
    {"isHardwareDetected", "(Ljava/lang/String;)Z", (void *) isHardwareDetected},
    {"canAuthenticate", "(I)I", (void *) canAuthenticate}
};

class FpBypassModule : public zygisk::ModuleBase {
public:
    void onLoad(zygisk::ModuleLoadContext *ctx) override {
        LOGD("FpBypass: Modul dimuat");
    }

    void postAppSpecialize(const zygisk::AppSpecializeArgs *args) override {
        LOGD("postAppSpecialize");
        JNIEnv *env = nullptr;
        if (app_env->GetJavaVM()->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) return;

        jclass clazz = env->FindClass("android/hardware/biometrics/BiometricManager");
        if (clazz != nullptr) {
            if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) == 0) {
                LOGD("Hook registered successfully");
            } else {
                LOGD("Failed to register hooks");
            }
        } else {
            LOGD("Class not found");
        }
    }
};

REGISTER_ZYGISK_MODULE(FpBypassModule)
