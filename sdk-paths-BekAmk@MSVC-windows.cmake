
#set(__depdir "${CMAKE_CURRENT_LIST_DIR}/../opentrack-depends")
#
function(setq name value)
    set("${name}" "${__depdir}/${value}" CACHE INTERNAL "" FORCE)
endfunction()

set(opentrack_install-debug-info TRUE CACHE INTERNAL "" FORCE)
set(opentrack_maintainer-mode TRUE CACHE INTERNAL "" FORCE)

set(Qt6_ROOT_DIR "C:/Qt/6.2.0/msvc2019_64")

#setq(EIGEN3_INCLUDE_DIR "eigen")
#setq(OpenCV_DIR "opencv/build")
#setq(SDL2_DIR "SDL2-win32")
#setq(SDK_ARUCO_LIBPATH "aruco/build/src/aruco.lib")
#setq(SDK_FSUIPC "fsuipc")
#setq(SDK_HYDRA "SixenseSDK")
#setq(SDK_KINECT20 "Kinect-v2.0")
#setq(SDK_LIBUSB "libusb-msvc-x86")
#setq(SDK_PS3EYEDRIVER "PS3EYEDriver")
#setq(SDK_REALSENSE "RSSDK-R3")
#setq(SDK_RIFT_140 "ovr_sdk_win_1.43.0/LibOVR")
#setq(SDK_VALVE_STEAMVR "steamvr")
#setq(SDK_VJOYSTICK "vjoystick")

set(PREFIX "prefix64")

#set(Qt6_DIR "${Qt6_ROOT_DIR}/lib/cmake/Qt6" CACHE PATH "" FORCE)
#set(Qt6Core_DIR "${Qt6_DIR}Core" CACHE PATH "" FORCE)
#set(Qt6Gui_DIR "${Qt6_DIR}Gui" CACHE PATH "" FORCE)

#if(CMAKE_GENERATOR STREQUAL "NMake Makefiles")
#    message("USING JOM")
#    set(CMAKE_MAKE_PROGRAM "C:/Programs/jom_1_1_3/jom" CACHE STRING "" FORCE)
#endif()


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")


