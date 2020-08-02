SET(SRCS_FILES
        Mutex.cpp
        Condition.cpp
        Thread.cpp
)

SET(HEADERS_FILES
        Mutex.h
        Condition.h
        Thread.h
)

add_library(higan_base ${SRCS_FILES} ${HEADERS_FILES})
target_link_libraries(higan_base pthread)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(ThreadTest test/ThreadTest.cpp)
target_link_libraries(ThreadTest higan_base)