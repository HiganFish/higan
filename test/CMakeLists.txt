add_executable(EventLoopTest EventLoopTest.cpp)
target_link_libraries(EventLoopTest higan_net)

add_executable(TcpServerTest TcpServerTest.cpp)
target_link_libraries(TcpServerTest higan_net)

add_executable(BufferTest BufferTest.cpp)
target_link_libraries(BufferTest higan_net)

add_executable(HttpContextTest HttpContextTest.cpp)
target_link_libraries(HttpContextTest higan_http)

add_executable(HttpServerTest HttpServerTest.cpp)
target_link_libraries(HttpServerTest higan_http)


add_executable(CodecTest utils/CodecTest.cpp)
target_link_libraries(CodecTest higan_http)

include_directories(${CMAKE_SOURCE_DIR})