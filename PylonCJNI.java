import java.util.Scanner;

public class PylonCJNI {

    static {
        System.loadLibrary("pyloncjni");
    }

    // Directly mapped Pylon SDK functions
    private native long PylonInitialize();
    private native long PylonEnumerateDevices();
    private native long PylonCreateDeviceByIndex(long index);
    private native long PylonDeviceOpen();
    private native long PylonDeviceClose();
    private native long PylonDeviceDestroy();
    private native long PylonDeviceGetStreamGrabber(long index);
    private native long PylonStreamGrabberOpen();
    private native long PylonStreamGrabberClose();
    private native long PylonStreamGrabberGetPayloadSize();
    private native long PylonDeviceGrabSingleFrame(int channel, int timeout);

    private native boolean PylonDeviceFeatureIsAvailable(String featureName);
    private native long PylonDeviceFeatureFromString(String featureName, String value);
    
    // Custom functions of the JNI wrapper
    private native long InitializeImageBuffer();

    public static void main(String[] args) {
        PylonCJNI pylon = new PylonCJNI();

        // Initialize the Pylon SDK
        if(pylon.PylonInitialize() != 0){
            System.out.println("Library initialization failed");
            return;
        }

        // Enumerate devices
        if(pylon.PylonEnumerateDevices() == 0){
            System.out.println("Device enumeration failed");
            return;
        }

        // Create a device by index 0
        if(pylon.PylonCreateDeviceByIndex(0) != 0){
            System.out.println("Device creation failed");
            return;
        }

        // Open the device
        if(pylon.PylonDeviceOpen() != 0){
            System.out.println("Device open failed");
            return;
        }

        // Set the pixel format to Mono8 if available
        if (pylon.PylonDeviceFeatureIsAvailable("EnumEntry_PixelFormat_Mono8"))
        {
            if(pylon.PylonDeviceFeatureFromString("PixelFormat", "Mono8") != 0)
            {
                System.out.println("Could not set pixel format to Mono8");
                return;
            }
        }
        
        // Disable acquisition start trigger if available
        if (pylon.PylonDeviceFeatureIsAvailable("EnumEntry_TriggerSelector_AcquisitionStart" ))
        {
            if(pylon.PylonDeviceFeatureFromString("TriggerSelector", "AcquisitionStart") != 0)
            {
                System.out.println("Could not disable acquisition start trigger");
                return;
            }

            if(pylon.PylonDeviceFeatureFromString("TriggerMode", "Off") != 0)
            {
                System.out.println("Could not disable acquisition start trigger");
                return;
            }
        }

        // Disable frame burst start trigger if available
        if (pylon.PylonDeviceFeatureIsAvailable("EnumEntry_TriggerSelector_FrameBurstStart" ))
        {
            if(pylon.PylonDeviceFeatureFromString("TriggerSelector", "FrameBurstStart") != 0)
            {
                System.out.println("Could not disable frame burst start trigger");
                return;
            }

            if(pylon.PylonDeviceFeatureFromString("TriggerMode", "Off") != 0)
            {
                System.out.println("Could not disable frame burst start trigger");
                return;
            }
        }

        // Disable frame start trigger if available
        if (pylon.PylonDeviceFeatureIsAvailable("EnumEntry_TriggerSelector_FrameBurstStart" ))
        {
            if(pylon.PylonDeviceFeatureFromString("TriggerSelector", "FrameStart") != 0)
            {
                System.out.println("Could not disable frame start trigger");
                return;
            }

            if(pylon.PylonDeviceFeatureFromString("TriggerMode", "Off") != 0)
            {
                System.out.println("Could not disable frame start trigger");
                return;
            }
        }

        if(pylon.PylonDeviceGetStreamGrabber(0) != 0){
            System.out.println("Stream grabber retrieval failed");
            return;
        }

        if(pylon.PylonStreamGrabberOpen() != 0){
            System.out.println("Stream grabber open failed");
            return;
        }

        if(pylon.PylonStreamGrabberGetPayloadSize() != 0){
            System.out.println("Payload size retrieval failed");
            return;
        }

        if(pylon.PylonStreamGrabberClose() != 0){
            System.out.println("Stream grabber close failed");
            return;
        }

        if(pylon.InitializeImageBuffer() != 0){
            System.out.println("Image buffer initialization failed");
            return;
        }

        if(pylon.PylonDeviceGrabSingleFrame(0, 1000) != 2){
            System.out.println("Frame grab failed");
            return;
        }

        // Wait for user input
        Scanner scan = new Scanner(System.in);
        scan.nextLine();
    }
}