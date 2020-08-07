SET(SRCS_FILES
        Mutex.cpp
        Condition.cpp
        Thread.cpp
        ThreadPool.cpp
        Timer.cpp
        TimeStamp.cpp
        LogFile.cpp
        Logger.cpp
        File.cpp
        System.cpp
        Codec.cpp
        Buffer.cpp
)

SET(HEADERS_FILES
        Mutex.h
        Condition.h
        Thread.h
        ThreadPool.h
        Timer.h
        TimeStamp.h
        LogFile.h
        Logger.h
        File.h
        copyable.h
        noncopyable.h
        System.h
        Codec.h
        Buffer.h
)

add_library(higan_base ${SRCS_FILES} ${HEADERS_FILES})
target_link_libraries(higan_base pthread)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(ThreadTest test/ThreadPoolTest.cpp)
target_link_libraries(ThreadTest higan_net)

add_executable(TimerTest test/TimerTest.cpp)
target_link_libraries(TimerTest higan_net)

add_executable(LoggerTest test/LoggerTest.cpp)
target_link_libraries(LoggerTest higan_net)

add_executable(BufferTest test/BufferTest.cpp)
target_link_libraries(BufferTest higan_net)