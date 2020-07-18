add_executable(EventLoopTest EventLoopTest.cpp)
target_link_libraries(EventLoopTest higan_net)

add_executable(AcceptorTest AcceptorTest.cpp)
target_link_libraries(AcceptorTest higan_net)

add_executable(TcpServerTest TcpServerTest.cpp)
target_link_libraries(TcpServerTest higan_net)

include_directories(${CMAKE_SOURCE_DIR})