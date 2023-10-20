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



# main source for logger library
list(APPEND src src/logger.h)       ## <- old header

list(APPEND src src/gnklog.cpp src/gnklog.h)
list(APPEND src src/Log.cpp src/Log.h)
list(APPEND src src/LogCore.cpp src/LogCore.h)
list(APPEND src src/LogConsoleSink.cpp src/LogConsoleSink.h)
list(APPEND src src/LogEvent.cpp src/LogEvent.h)
list(APPEND src src/LogEventFifoUnix.cpp src/LogEventFifoUnix.h)
list(APPEND src src/LogEventPipeUnix.cpp src/LogEventPipeUnix.h)
list(APPEND src src/LogInstance.h)
list(APPEND src src/LogInternal.h)
list(APPEND src src/LogManager.cpp src/LogManager.h)

# unit test source code
list(APPEND src_utest src/utests/test_gnklog.cpp)

# library/artifact declaration
add_library(gnklog_utest SHARED ${src} ${src_utest})
add_library(gnklog STATIC ${src})

target_compile_options(gnklog PUBLIC -Wall -Wextra -Wshadow)
target_include_directories(gnklog PUBLIC src)
target_link_libraries(gnklog options)

# unit test
target_compile_options(gnklog_utest PUBLIC -Wall -Wextra -Wshadow -Wno-unused-parameter)
target_include_directories(gnklog_utest PUBLIC src)
target_link_libraries(gnklog_utest options)
