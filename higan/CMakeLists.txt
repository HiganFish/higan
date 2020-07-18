SET(SRCS_FILES
        multiplexing/MultiplexBase.cpp
        multiplexing/MultiplexEpoll.cpp
        Channel.cpp
        utils/Logger.cpp
        EventLoop.cpp
        Acceptor.cpp
        Socket.cpp
        InetAddress.cpp
)

SET(HEADERS_FILES
        multiplexing/MultiplexBase.h
        multiplexing/MultiplexEpoll.h
        Channel.h
        utils/Logger.h
        EventLoop.h
        Acceptor.h
        Socket.h
        InetAddress.cpp
)

add_library(higan_net ${SRCS_FILES} ${HEADERS_FILES})

include_directories(${CMAKE_SOURCE_DIR})