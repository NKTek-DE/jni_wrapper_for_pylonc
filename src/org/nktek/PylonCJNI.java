package org.nktek;

import java.util.Scanner;

public class PylonCJNI {

    static {
        System.loadLibrary("pyloncjni");
    }

    // Directly mapped Pylon SDK functions
    private native long pylonInitialize();
    private native long pylonEnumerateDevices();
    private native long pylonCreateDeviceByIndex(long index);
    private native long pylonDeviceOpen();
    private native long pylonDeviceClose();
    private native long pylonDestroyDevice();
    private native long pylonDeviceGetStreamGrabber(long index);
    private native long pylonStreamGrabberOpen();
    private native long pylonStreamGrabberClose();
    private native long pylonStreamGrabberGetPayloadSize();
    private native long pylonDeviceGrabSingleFrame(int channel, int timeout);

    private native boolean pylonDeviceFeatureIsAvailable(String featureName);
    private native long pylonDeviceFeatureFromString(String featureName, String value);
    
    // Custom functions of the JNI wrapper
    private native long initializeImageBuffer();
    private native byte[] getBufferedImage();
    private native int getBufferedImageWidth();
    private native int getBufferedImageHeight();

    public static void main(String[] args) {
        PylonCJNI pylon = new PylonCJNI();

        // Initialize the Pylon SDK
        if(pylon.pylonInitialize() != 0){
            System.out.println("Library initialization failed");
            return;
        }

        // Enumerate devices
        if(pylon.pylonEnumerateDevices() == 0){
            System.out.println("Device enumeration failed");
            return;
        }

        // Create a device by index 0
        if(pylon.pylonCreateDeviceByIndex(0) != 0){
            System.out.println("Device creation failed");
            return;
        }

        // Open the device
        if(pylon.pylonDeviceOpen() != 0){
            System.out.println("Device open failed");
            return;
        }

        // Set the pixel format to Mono8 if available
        if (pylon.pylonDeviceFeatureIsAvailable("EnumEntry_PixelFormat_Mono8"))
        {
            if(pylon.pylonDeviceFeatureFromString("PixelFormat", "Mono8") != 0)
            {
                System.out.println("Could not set pixel format to Mono8");
                return;
            }
        }
        
        // Disable acquisition start trigger if available
        if (pylon.pylonDeviceFeatureIsAvailable("EnumEntry_TriggerSelector_AcquisitionStart" ))
        {
            if(pylon.pylonDeviceFeatureFromString("TriggerSelector", "AcquisitionStart") != 0)
            {
                System.out.println("Could not disable acquisition start trigger");
                return;
            }

            if(pylon.pylonDeviceFeatureFromString("TriggerMode", "Off") != 0)
            {
                System.out.println("Could not disable acquisition start trigger");
                return;
            }
        }

        // Disable frame burst start trigger if available
        if (pylon.pylonDeviceFeatureIsAvailable("EnumEntry_TriggerSelector_FrameBurstStart" ))
        {
            if(pylon.pylonDeviceFeatureFromString("TriggerSelector", "FrameBurstStart") != 0)
            {
                System.out.println("Could not disable frame burst start trigger");
                return;
            }

            if(pylon.pylonDeviceFeatureFromString("TriggerMode", "Off") != 0)
            {
                System.out.println("Could not disable frame burst start trigger");
                return;
            }
        }

        // Disable frame start trigger if available
        if (pylon.pylonDeviceFeatureIsAvailable("EnumEntry_TriggerSelector_FrameBurstStart" ))
        {
            if(pylon.pylonDeviceFeatureFromString("TriggerSelector", "FrameStart") != 0)
            {
                System.out.println("Could not disable frame start trigger");
                return;
            }

            if(pylon.pylonDeviceFeatureFromString("TriggerMode", "Off") != 0)
            {
                System.out.println("Could not disable frame start trigger");
                return;
            }
        }

        if(pylon.pylonDeviceGetStreamGrabber(0) != 0){
            System.out.println("Stream grabber retrieval failed");
            return;
        }

        if(pylon.pylonStreamGrabberOpen() != 0){
            System.out.println("Stream grabber open failed");
            return;
        }

        if(pylon.pylonStreamGrabberGetPayloadSize() != 0){
            System.out.println("Payload size retrieval failed");
            return;
        }

        if(pylon.pylonStreamGrabberClose() != 0){
            System.out.println("Stream grabber close failed");
            return;
        }

        if(pylon.initializeImageBuffer() != 0){
            System.out.println("Image buffer initialization failed");
            return;
        }

        if(pylon.pylonDeviceGrabSingleFrame(0, 1000) != 2){
            System.out.println("Frame grab failed");
            return;
        }

        // Wait for user input
        Scanner scan = new Scanner(System.in);
        scan.nextLine();
    }
}