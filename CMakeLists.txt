cmake_minimum_required(VERSION 3.16)
project(gnklog)


set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)


# I just want a small set of features (field initializers) and perhaps change some stuff to use std:span as well
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(LOG_HAVE_NEWLINE "Append newline to strings" ON)
option(LOG_HAVE_FMT "Set to true to use your version of fmt" OFF)


# dummy library to hold baseline compile options for multiple targets..
add_library(options INTERFACE)

if(LOG_HAVE_NEWLINE)
    target_compile_definitions(options INTERFACE LOG_HAVE_NEWLINE)
endif()

#
# Compile the bundled version of fmt
#
if (NOT LOG_HAVE_FMT)
    message(STATUS "gnklog using bundled fmt")
    list(APPEND fmt_src src/ext/fmt/format.cc)
    # note: name is not fmt because it will clash with other projects importing this library
    add_library(gnk_log_fmt STATIC ${fmt_src})
    target_compile_options(gnk_log_fmt PUBLIC -fPIC)
    target_include_directories(gnk_log_fmt PUBLIC src/ext)
    list(APPEND libdeps gnk_log_fmt)
else()
    #set(LOG_FMT_INCLUDE ${LOG_FMT_DIR}/include)
    message(STATUS "gnklog using external fmt from ${LOG_FMT_DIR}")
    if (NOT LOG_FMT_DIR)
        message("You need to specify your FMT directory with 'LOG_FMT_DIR'")
    endif()

    list(APPEND include_dirs ${LOG_FMT_DIR}/include)
    #target_include_directories(options INTERFACE )

endif()



# main source for logger library
list(APPEND src src/logger.h)       ## <- old header

list(APPEND src src/gnklog.cpp src/gnklog.h)
list(APPEND src src/Log.cpp src/Log.h)
list(APPEND src src/LogCache.cpp src/LogCache.h)
list(APPEND src src/LogCore.cpp src/LogCore.h)
list(APPEND src src/LogConsoleSink.cpp src/LogConsoleSink.h)
list(APPEND src src/LogEvent.cpp src/LogEvent.h)
list(APPEND src src/LogFileSink.cpp src/LogFileSink.h)
list(APPEND src src/LogInstance.h)
list(APPEND src src/LogInternal.h)
list(APPEND src src/LogIPCStreamBase.cpp src/LogIPCStreamBase.h)
list(APPEND src src/LogIPCQueue.cpp src/LogIPCQueue.h)
list(APPEND src src/LogManager.cpp src/LogManager.h)
list(APPEND src src/LogSafeQueue.h)
list(APPEND src src/LogSink.cpp src/LogSink.h)

if (UNIX)
    list(APPEND src src/LogIPCFifoUnix.cpp src/LogIPCFifoUnix.h)
    list(APPEND src src/LogIPCPipeUnix.cpp src/LogIPCPipeUnix.h)
elseif (WIN32)

endif()

# unit test source code
list(APPEND src_utest src/utests/test_gnklog.cpp)
list(APPEND src_utest src/utests/test_logcache.cpp)
list(APPEND src_utest src/utests/test_logger.cpp)
list(APPEND src_utest src/utests/test_logipc.cpp)
list(APPEND src_utest src/utests/test_logmanager.cpp)
list(APPEND src_utest src/utests/test_threading.cpp)

# library/artifact declaration
add_library(gnklog_utest SHARED ${src} ${src_utest})
add_library(gnklog STATIC ${src})

list(APPEND include_dirs src src/ext)
if (APPLE)
    list(APPEND include_dirs /usr/local/include)
endif()


if(UNIX)
    target_compile_options(gnklog PUBLIC -Wall -Wextra -Wshadow -Wpedantic)
    target_compile_options(gnklog_utest PUBLIC -Wall -Wextra -Wshadow -Wpedantic -Wno-unused-parameter)
elseif(WIN32)
    # win32 stuff here...
endif()
target_include_directories(gnklog PUBLIC ${include_dirs})
#target_link_libraries(gnklog options gnk_log_fmt)
target_link_libraries(gnklog options ${libdeps})

# unit test
target_include_directories(gnklog_utest PUBLIC ${include_dirs})
target_compile_definitions(gnklog_utest PUBLIC -DTRUN_USE_V1)
#target_link_libraries(gnklog_utest options gnk_log_fmt)
target_link_libraries(gnklog_utest options ${libdeps})
