SET(SRCS_FILES
        multiplexing/MultiplexBase.cpp
        multiplexing/MultiplexEpoll.cpp
        Channel.cpp
        utils/Logger.cpp
        utils/File.cpp
        utils/FileCache.cpp
        EventLoop.cpp
        Acceptor.cpp
        Socket.cpp
        InetAddress.cpp
        TcpServer.cpp
        TcpConnection.cpp
        Buffer.cpp
        utils/System.cpp
)

SET(HEADERS_FILES
        multiplexing/MultiplexBase.h
        multiplexing/MultiplexEpoll.h
        Channel.h
        utils/Logger.h
        utils/File.h
        utils/FileCache.h
        utils/copyable.h
        utils/noncopyable.h
        EventLoop.h
        Acceptor.h
        Socket.h
        InetAddress.h
        TcpServer.h
        TcpConnection.h
        Buffer.h
        utils/System.h
        Callback.h
)

add_library(higan_net ${SRCS_FILES} ${HEADERS_FILES})

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(http)