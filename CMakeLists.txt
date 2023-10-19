cmake_minimum_required(VERSION 3.26)
project(gnklog)

set(CMAKE_CXX_STANDARD 17)

# main source for logger library
list(APPEND src src/logger.h)       ## <- old header

list(APPEND src src/gnklog.cpp src/gnklog.h)
list(APPEND src src/logcore.h)
list(APPEND src src/logwriter.cpp src/logwriter.h)

# unit test source code
list(APPEND src_utest src/utests/test_gnklog.cpp)

# library/artifact declaration
add_library(gnklog_utest SHARED ${src} ${src_utest})
add_library(gnklog STATIC ${src})

target_compile_options(gnklog PUBLIC -Wall -Wextra -Wshadow -fPIC)
target_include_directories(gnklog PUBLIC src)

# unit test
target_compile_options(gnklog_utest PUBLIC -Wall -Wextra -Wshadow -fPIC)
target_include_directories(gnklog_utest PUBLIC src)

