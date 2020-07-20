add_executable(EventLoopTest EventLoopTest.cpp)
target_link_libraries(EventLoopTest higan_net)

add_executable(TcpServerTest TcpServerTest.cpp)
target_link_libraries(TcpServerTest higan_net)

add_executable(BufferTest BufferTest.cpp)
target_link_libraries(BufferTest higan_net)

include_directories(${CMAKE_SOURCE_DIR})