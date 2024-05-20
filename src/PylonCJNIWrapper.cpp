#include "PylonCJNIWrapper.h"
#include "org_nktek_PylonCJNI.h"

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

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonInitialize (JNIEnv *env, jobject thisObj)
{
    return PylonInitialize();
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonEnumerateDevices (JNIEnv *env, jobject thisObj)
{
    GENAPIC_RESULT res;
    size_t numDevices = 0;

    res = PylonEnumerateDevices(&numDevices);
    if (res != 0) {
        return -1;
    }

    return numDevices;
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonCreateDeviceByIndex(JNIEnv *env, jobject thisObj, jlong index)
{
    return PylonCreateDeviceByIndex(index, &cameraHandle);
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonDeviceOpen (JNIEnv *env, jobject thisObj)
{
    return PylonDeviceOpen(cameraHandle, PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM);
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonDeviceClose(JNIEnv *env, jobject thisObj)
{
    return PylonDeviceClose(cameraHandle);
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonDestroyDevice(JNIEnv *env, jobject thisObj)
{
    return PylonDestroyDevice(cameraHandle);
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonDeviceGetStreamGrabber(JNIEnv *env, jobject thisObj, jlong index)
{
    return PylonDeviceGetStreamGrabber(cameraHandle, index, &streamGrabberHandle);
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonStreamGrabberOpen(JNIEnv *env, jobject thisObj)
{
    return PylonStreamGrabberOpen(streamGrabberHandle);
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonStreamGrabberClose(JNIEnv *env, jobject thisObj)
{
    return PylonStreamGrabberClose(streamGrabberHandle);
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonStreamGrabberGetPayloadSize(JNIEnv *env, jobject thisObj)
{
    return PylonStreamGrabberGetPayloadSize(cameraHandle, streamGrabberHandle, &imageSize);
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_initializeImageBuffer(JNIEnv *env, jobject thisObj)
{
    imageBuffer = (unsigned char*) malloc(imageSize);

    if(imageBuffer == NULL)
    {
        return -1;
    }

    return 0;
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonDeviceGrabSingleFrame(JNIEnv *env, jobject thisObj, jint channel, jint timeout)
{
    _Bool bufferReady;

    GENAPIC_RESULT res = PylonDeviceGrabSingleFrame(cameraHandle, channel, imageBuffer, imageSize, &grabResult, &bufferReady, timeout);

    if (GENAPI_E_OK == res && !bufferReady)
    {
        return -1;
    }

    return grabResult.Status;
}

JNIEXPORT jbyteArray JNICALL Java_org_nktek_PylonCJNI_getBufferedImage(JNIEnv *env, jobject thisObj)
{
    if (grabResult.Status == 2) {
        jbyteArray byteArray = env->NewByteArray(grabResult.PayloadSize);
        env->SetByteArrayRegion(byteArray, 0, grabResult.PayloadSize, reinterpret_cast<jbyte*>(imageBuffer));

        return byteArray;
    }

    return NULL;
}

JNIEXPORT jint JNICALL Java_org_nktek_PylonCJNI_getBufferedImageWidth(JNIEnv *env, jobject thisObj)
{
    if (grabResult.Status == 2) {
        return grabResult.SizeX;
    }

    return -1;
}

JNIEXPORT jint JNICALL Java_org_nktek_PylonCJNI_getBufferedImageHeight(JNIEnv *env, jobject thisObj)
{
    if (grabResult.Status == 2) {
        return grabResult.SizeY;
    }

    return -1;
}


JNIEXPORT jboolean JNICALL Java_org_nktek_PylonCJNI_pylonDeviceFeatureIsAvailable(JNIEnv *env, jobject thisObj, jstring featureName)
{
    _Bool res;
    const char *nativeFeatureName = env->GetStringUTFChars(featureName, 0);

    res = PylonDeviceFeatureIsAvailable(cameraHandle, nativeFeatureName);

    env->ReleaseStringUTFChars(featureName, nativeFeatureName);

    return res;
}

JNIEXPORT jlong JNICALL Java_org_nktek_PylonCJNI_pylonDeviceFeatureFromString(JNIEnv *env, jobject thisObj, jstring featureName, jstring value)
{
    GENAPIC_RESULT res;
    
    const char *nativeFeatureName = env->GetStringUTFChars(featureName, 0);
    const char *nativeValue = env->GetStringUTFChars(value, 0);

    res = PylonDeviceFeatureFromString(cameraHandle, nativeFeatureName, nativeValue);

    env->ReleaseStringUTFChars(featureName, nativeFeatureName);
    env->ReleaseStringUTFChars(value, nativeValue);

    return res;
}