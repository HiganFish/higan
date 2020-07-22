SET(SRC_FILES
        HttpServer.cpp
        HttpContext.cpp
        HttpRequest.cpp
)

SET(HEADER_FILES
        HttpServer.h
        HttpContext.h
        HttpRequest.h
)

add_library(higan_http ${SRC_FILES} ${HEADER_FILES})
target_link_libraries(higan_http higan_net)