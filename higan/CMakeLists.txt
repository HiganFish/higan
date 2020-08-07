SET(SRCS_FILES
        multiplexing/MultiplexBase.cpp
        multiplexing/MultiplexEpoll.cpp
        Channel.cpp
        EventLoop.cpp
        Acceptor.cpp
        Socket.cpp
        InetAddress.cpp
        TcpServer.cpp
        TcpConnection.cpp
        EventLoopThread.cpp
        EventLoopThreadPool.cpp
        )

SET(HEADERS_FILES
        multiplexing/MultiplexBase.h
        multiplexing/MultiplexEpoll.h
        Channel.h
        EventLoop.h
        Acceptor.h
        Socket.h
        InetAddress.h
        TcpServer.h
        TcpConnection.h
        Callback.h
        EventLoopThread.h
        EventLoopThreadPool.h
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

add_executable(EventLoopThreadPoolTest test/EventLoopThreadPoolTest.cpp)
target_link_libraries(EventLoopThreadPoolTest higan_net)