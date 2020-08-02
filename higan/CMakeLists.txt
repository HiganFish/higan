SET(SRCS_FILES
        multiplexing/MultiplexBase.cpp
        multiplexing/MultiplexEpoll.cpp
        Channel.cpp
        utils/Logger.cpp
        utils/File.cpp
        EventLoop.cpp
        Acceptor.cpp
        Socket.cpp
        InetAddress.cpp
        TcpServer.cpp
        TcpConnection.cpp
        Buffer.cpp
        utils/System.cpp
        utils/Codec.cpp
        )

SET(HEADERS_FILES
        multiplexing/MultiplexBase.h
        multiplexing/MultiplexEpoll.h
        Channel.h
        utils/Logger.h
        utils/File.h
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
        utils/Codec.h
        )

add_library(higan_net ${SRCS_FILES} ${HEADERS_FILES})
target_link_libraries(higan_net higan_base)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(base)
add_subdirectory(http)


add_executable(EventLoopTest test/EventLoopTest.cpp)
target_link_libraries(EventLoopTest higan_net)

add_executable(TcpServerTest test/TcpServerTest.cpp)
target_link_libraries(TcpServerTest higan_net)

add_executable(BufferTest test/BufferTest.cpp)
target_link_libraries(BufferTest higan_net)

