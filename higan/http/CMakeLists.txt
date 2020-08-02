SET(SRC_FILES
        HttpServer.cpp
        HttpContext.cpp
        HttpRequest.cpp
        HttpResponse.cpp
        FileContext.cpp
)

SET(HEADER_FILES
        HttpServer.h
        HttpContext.h
        HttpRequest.h
        HttpResponse.h
        FileContext.h
)

add_library(higan_http ${SRC_FILES} ${HEADER_FILES})
target_link_libraries(higan_http higan_net)

add_executable(HttpServerTest test/HttpServerTest.cpp)
target_link_libraries(HttpServerTest higan_http)

add_executable(HttpContextTest test/HttpContextTest.cpp)
target_link_libraries(HttpContextTest higan_http)

add_executable(CodecTest test/CodecTest.cpp)
target_link_libraries(CodecTest higan_http)
