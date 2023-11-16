// C++与java交互的实现
#include <jni.h>
// C++的string
#include <string>
#include <android/log.h>
// 引入ANativeWindow
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <GLES3//gl3.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define LOG_TAG "ndk-build"
#define LOGI(...)   __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)   __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

bool initOpenGLES();

EGLDisplay display;

extern "C" JNIEXPORT jstring JNICALL
Java_com_ricardo_openglel_1study_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    ANativeWindow * window;
    initOpenGLES();
    //只要没有log错误，并且下面的字符串成功显示到界面上，就证明成功了
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

// 创建EGL上下文
bool initOpenGLES() {
    // 获取显示设备
    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        LOGE("eglGetDisplay returned err %d", eglGetError());
        return false;
    }

    // 初始化显示设备
    if (!eglInitialize(display, 0, 0)) {        // 不关心版本号，直接传0
        LOGE("eglInitialize returned err %d", eglGetError());
        return false;
    }

    // 获取config，比如色彩格式、像素格式、SurfaceType等
    EGLConfig config;
    int numConfigs;
    const EGLint attribs[] = {EGL_BUFFER_SIZE, 32,
                              EGL_ALPHA_SIZE, 8,
                              EGL_BLUE_SIZE, 8,
                              EGL_GREEN_SIZE, 8,
                              EGL_RED_SIZE, 8,
                              EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
                              EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                              EGL_NONE};

    // 获取配置选项信息
    if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
        LOGE("eglChooseConfig returned err %d", eglGetError());
        return false;
    }
    EGLint attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    EGLContext context;
    // 创建上下文环境context
    if (!(context = eglCreateContext(display, config, NULL, attributes))) {
        LOGE("eglCreateContext returned err %d", eglGetError());
        return false;
    }
    EGLSurface surface = NULL;
    EGLint format;
    if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
        LOGE("eglGetConfigAttrib returned err %d", eglGetError());
        return false;
    }
    LOGI("好高兴啊，我成功了???");
    return true;
}