@echo off
"C:\\Users\\AOZ\\AppData\\Local\\Android\\Sdk\\cmake\\3.18.1\\bin\\cmake.exe" ^
  "-HC:\\Users\\AOZ\\Desktop\\Our-Ultimate-Robot\\Object Detection\\Multi-Model\\RobotVision-Android\\cv\\libcxx_helper" ^
  "-DCMAKE_SYSTEM_NAME=Android" ^
  "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON" ^
  "-DCMAKE_SYSTEM_VERSION=21" ^
  "-DANDROID_PLATFORM=android-21" ^
  "-DANDROID_ABI=x86" ^
  "-DCMAKE_ANDROID_ARCH_ABI=x86" ^
  "-DANDROID_NDK=C:\\Users\\AOZ\\AppData\\Local\\Android\\Sdk\\ndk\\23.1.7779620" ^
  "-DCMAKE_ANDROID_NDK=C:\\Users\\AOZ\\AppData\\Local\\Android\\Sdk\\ndk\\23.1.7779620" ^
  "-DCMAKE_TOOLCHAIN_FILE=C:\\Users\\AOZ\\AppData\\Local\\Android\\Sdk\\ndk\\23.1.7779620\\build\\cmake\\android.toolchain.cmake" ^
  "-DCMAKE_MAKE_PROGRAM=C:\\Users\\AOZ\\AppData\\Local\\Android\\Sdk\\cmake\\3.18.1\\bin\\ninja.exe" ^
  "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=C:\\Users\\AOZ\\Desktop\\Our-Ultimate-Robot\\Object Detection\\Multi-Model\\RobotVision-Android\\cv\\build\\intermediates\\cxx\\Debug\\24166s2p\\obj\\x86" ^
  "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=C:\\Users\\AOZ\\Desktop\\Our-Ultimate-Robot\\Object Detection\\Multi-Model\\RobotVision-Android\\cv\\build\\intermediates\\cxx\\Debug\\24166s2p\\obj\\x86" ^
  "-DCMAKE_BUILD_TYPE=Debug" ^
  "-BC:\\Users\\AOZ\\Desktop\\Our-Ultimate-Robot\\Object Detection\\Multi-Model\\RobotVision-Android\\cv\\.cxx\\Debug\\24166s2p\\x86" ^
  -GNinja ^
  "-DANDROID_STL=c++_shared"
