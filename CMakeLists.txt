cmake_minimum_required(VERSION 3.26)
project(gnklog)


set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(LOG_HAVE_NEWLINE "Append newline to strings" ON)


# dummy library to hold baseline compile options for multiple targets..
add_library(options INTERFACE)

if(LOG_HAVE_NEWLINE)
    target_compile_definitions(options INTERFACE LOG_HAVE_NEWLINE)
endif()

#
# Compile the bundled version of fmt
#
list(APPEND fmt_src src/ext/fmt/format.cc)
add_library(fmt STATIC ${fmt_src})
target_compile_options(fmt PUBLIC -fPIC)
target_include_directories(fmt PUBLIC src/ext)



# main source for logger library
list(APPEND src src/logger.h)       ## <- old header

list(APPEND src src/gnklog.cpp src/gnklog.h)
list(APPEND src src/Log.cpp src/Log.h)
list(APPEND src src/LogCache.cpp src/LogCache.h)
list(APPEND src src/LogCore.cpp src/LogCore.h)
list(APPEND src src/LogConsoleSink.cpp src/LogConsoleSink.h)
list(APPEND src src/LogEvent.cpp src/LogEvent.h)
list(APPEND src src/LogEventFifoUnix.cpp src/LogEventFifoUnix.h)
list(APPEND src src/LogEventPipeUnix.cpp src/LogEventPipeUnix.h)
list(APPEND src src/LogInstance.h)
list(APPEND src src/LogInternal.h)
list(APPEND src src/LogManager.cpp src/LogManager.h)
list(APPEND src src/LogSink.cpp src/LogSink.h)

# unit test source code
list(APPEND src_utest src/utests/test_gnklog.cpp)
list(APPEND src_utest src/utests/test_logcache.cpp)
list(APPEND src_utest src/utests/test_logger.cpp)

# library/artifact declaration
add_library(gnklog_utest SHARED ${src} ${src_utest})
add_library(gnklog STATIC ${src})

list(APPEND include_dirs src src/ext)
if (APPLE)
    list(APPEND include_dirs /usr/local/include)
endif()



target_compile_options(gnklog PUBLIC -Wall -Wextra -Wshadow)
target_include_directories(gnklog PUBLIC ${include_dirs})
target_link_libraries(gnklog options fmt)

# unit test
target_compile_options(gnklog_utest PUBLIC -Wall -Wextra -Wshadow -Wno-unused-parameter)
target_include_directories(gnklog_utest PUBLIC ${include_dirs})
target_link_libraries(gnklog_utest options fmt)
