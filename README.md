# JNI wrapper for PylonC
This project is a very rudimentary demonstration of how to use JNI to access PylonC SDK and grab a single frame. This is the preparation to later access Basler industrial cameras in OpenPnP software directly via PylonC SDK to enable very low latency imaging to detect part orientations.

## How to Build
1. Make sure you set the correct JAVA_INC, PYLON_INC and PYLON_RUN variables in make.cmd
2. Pylon SDK has to be installed as this wrapper links the pre-compiled Pylon runtime
3. Of course g++ compiler and Java JDK must be installed

I only built this project on Windows, there you should be able to just run make.cmd

## Limitations
The wrapper doesn't expose any internal structures of PylonC like the camera / device handle, streamgrabber handle etc. as I don't need this for later integration in OpenPnP. This means right now you can only access one camera at a time and have no direct control over the different device handlers. Linking a pre-compiled dll also isn't that great.