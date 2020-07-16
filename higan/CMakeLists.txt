SET(SRCS_FILES
    multiplexing/MultiplexBase.cpp
    multiplexing/MultiplexEpoll.cpp
    Channel.cpp
    utils/Logger.cpp
    EventLoop.cpp
)

SET(HEADERS_FILES
    multiplexing/MultiplexBase.h
    multiplexing/MultiplexEpoll.h
    Channel.h
    utils/Logger.h
    EventLoop.h
)

add_library(higan_net ${SRCS_FILES} ${HEADERS_FILES})

include_directories(${CMAKE_SOURCE_DIR})