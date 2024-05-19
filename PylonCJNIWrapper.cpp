#include "PylonCJNIWrapper.h"
#include "PylonCJNI.h"

#include <stdint.h>

#define _STDINT
#define _stdcall __stdcall
extern "C" {
    #include <pylonc/PylonC.h>
}

#include <windows.h>

#include <memory>
#include <functional>
#include <iostream>

using std::string;
using std::function;
using std::unique_ptr;
using std::shared_ptr;
using std::cout;
using std::endl;

PYLON_DEVICE_HANDLE cameraHandle;
PYLON_STREAMGRABBER_HANDLE streamGrabberHandle;

size_t imageSize = 0;
unsigned char* imageBuffer;

PylonGrabResult_t grabResult;

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonInitialize (JNIEnv *env, jobject thisObj)
{
    return PylonInitialize();
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonEnumerateDevices (JNIEnv *env, jobject thisObj)
{
    GENAPIC_RESULT res;
    size_t numDevices = 0;

    res = PylonEnumerateDevices(&numDevices);
    if (res != 0) {
        return -1;
    }

    return numDevices;
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonCreateDeviceByIndex(JNIEnv *env, jobject thisObj, jlong index)
{
    return PylonCreateDeviceByIndex(index, &cameraHandle);
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonDeviceOpen (JNIEnv *env, jobject thisObj)
{
    return PylonDeviceOpen(cameraHandle, PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM);
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonDeviceClose(JNIEnv *env, jobject thisObj)
{
    return PylonDeviceClose(cameraHandle);
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonDestroyDevice(JNIEnv *env, jobject thisObj)
{
    return PylonDestroyDevice(cameraHandle);
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonDeviceGetStreamGrabber(JNIEnv *env, jobject thisObj, jlong index)
{
    return PylonDeviceGetStreamGrabber(cameraHandle, index, &streamGrabberHandle);
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonStreamGrabberOpen(JNIEnv *env, jobject thisObj)
{
    return PylonStreamGrabberOpen(streamGrabberHandle);
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonStreamGrabberClose(JNIEnv *env, jobject thisObj)
{
    return PylonStreamGrabberClose(streamGrabberHandle);
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonStreamGrabberGetPayloadSize(JNIEnv *env, jobject thisObj)
{
    return PylonStreamGrabberGetPayloadSize(cameraHandle, streamGrabberHandle, &imageSize);
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_InitializeImageBuffer(JNIEnv *env, jobject thisObj)
{
    imageBuffer = (unsigned char*) malloc(imageSize);

    if(imageBuffer == NULL)
    {
        return -1;
    }

    return 0;
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonDeviceGrabSingleFrame(JNIEnv *env, jobject thisObj, jint channel, jint timeout)
{
    _Bool bufferReady;

    GENAPIC_RESULT res = PylonDeviceGrabSingleFrame(cameraHandle, channel, imageBuffer, imageSize, &grabResult, &bufferReady, timeout);

    if (GENAPI_E_OK == res && !bufferReady)
    {
        return -1;
    }

    return grabResult.Status;
}

JNIEXPORT jboolean JNICALL Java_PylonCJNI_PylonDeviceFeatureIsAvailable(JNIEnv *env, jobject thisObj, jstring featureName)
{
    _Bool res;
    const char *nativeFeatureName = env->GetStringUTFChars(featureName, 0);

    res = PylonDeviceFeatureIsAvailable(cameraHandle, nativeFeatureName);

    env->ReleaseStringUTFChars(featureName, nativeFeatureName);

    return res;
}

JNIEXPORT jlong JNICALL Java_PylonCJNI_PylonDeviceFeatureFromString(JNIEnv *env, jobject thisObj, jstring featureName, jstring value)
{
    GENAPIC_RESULT res;
    
    const char *nativeFeatureName = env->GetStringUTFChars(featureName, 0);
    const char *nativeValue = env->GetStringUTFChars(value, 0);

    res = PylonDeviceFeatureFromString(cameraHandle, nativeFeatureName, nativeValue);

    env->ReleaseStringUTFChars(featureName, nativeFeatureName);
    env->ReleaseStringUTFChars(value, nativeValue);

    return res;
}