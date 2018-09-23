cmake_minimum_required(VERSION 3.4.1)



add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp
             src/main/cpp/PeopleFFmpeg.cpp
             src/main/cpp/JavaCallHelper.cpp
             src/main/cpp/AudioChannel.cpp
             src/main/cpp/VideoChannel.cpp
             )

#此ffmpeg库文件支持最低的Android版本是21(即Android5.0)
include_directories(src/main/cpp/include/ffmpeg_header)

#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${ANDROID_ABI}")
set(ffmpeg_lib_path ${CMAKE_SOURCE_DIR}/src/main/cpp/libs)
#message("ffmpeg_lib_path==> " ${ffmpeg_lib_path}/${ANDROID_ABI}/libavformat.a)
# 添加第三方动态库(avcodec)
add_library(  avcodec
             STATIC
             IMPORTED)
# 设置第三方动态库属性（avcodec 存储位置）
set_target_properties(  avcodec
                      PROPERTIES IMPORTED_LOCATION
                      ${ffmpeg_lib_path}/${ANDROID_ABI}/libavcodec.a)

# 添加第三方动态库(avfilter)
add_library(  avfilter
             STATIC
             IMPORTED)
# 设置第三方动态库属性（avfilter 存储位置）
set_target_properties(  avfilter
                      PROPERTIES IMPORTED_LOCATION
                      ${ffmpeg_lib_path}/${ANDROID_ABI}/libavfilter.a)


# 添加第三方动态库(avformat)
add_library(  avformat
             STATIC
             IMPORTED)
# 设置第三方动态库属性（avformat 存储位置）
set_target_properties(  avformat
                      PROPERTIES IMPORTED_LOCATION
                      #${ffmpeg_lib_path}/${ANDROID_ABI}/libavformat.a
                      C:/work/study_dn/dn_video_player/app/src/main/cpp/libs/armeabi-v7a/libavformat.a
                      )

# 添加第三方动态库(avutil)
add_library(  avutil
             STATIC
             IMPORTED)
# 设置第三方动态库属性（avutil 存储位置）
set_target_properties(  avutil
                      PROPERTIES IMPORTED_LOCATION
                      ${ffmpeg_lib_path}/${ANDROID_ABI}/libavutil.a)


# 添加第三方动态库(swresample)
add_library(  swresample
             STATIC
             IMPORTED)
# 设置第三方动态库属性（swresample 存储位置）
set_target_properties(  swresample
                      PROPERTIES IMPORTED_LOCATION
                      ${ffmpeg_lib_path}/${ANDROID_ABI}/libswresample.a)


# 添加第三方动态库(swscale)
add_library(  swscale
             STATIC
             IMPORTED)
# 设置第三方动态库属性（swscale 存储位置）
set_target_properties(  swscale
                      PROPERTIES IMPORTED_LOCATION
                      ${ffmpeg_lib_path}/${ANDROID_ABI}/libswscale.a)
#ffmpeg静态库调用的顺序也不能错，否则调用的时候会出错
target_link_libraries(native-lib
                    avformat
                    avcodec
                    avfilter
                    avutil
                    swresample
                    swscale
                    log
                    z
                    android#链接Android的libandroid.so库
                    )