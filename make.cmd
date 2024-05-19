SET JAVA_INC="C:\jdk-15.0.1\include"
SET PYLON_INC="C:\Program Files\Basler\pylon 7\Development\include"
SET PYLON_RUN="C:\Program Files\Basler\pylon 7\Runtime\x64"

javac -h . PylonCJNI.java
g++ -std=c++11 -I%JAVA_INC% -I%JAVA_INC%/win32 -I%JAVA_INC%/win32/bridge -I%PYLON_INC% -L%PYLON_RUN% -lPylonC_v7_4 PylonCJNIWrapper.cpp -shared -Wl,--add-stdcall-alias -o pyloncjni.dll