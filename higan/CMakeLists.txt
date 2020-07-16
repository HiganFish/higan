SET(SRCS_FILES
    multiplexing/MultiplexBase.cpp
    multiplexing/MultiplexEpoll.cpp
    Channel.cpp
    utils/Logger.cpp
)

SET(HEADERS_FILES
    multiplexing/MultiplexBase.h
    multiplexing/MultiplexEpoll.h
    Channel.h
    utils/Logger.h
)

add_library(mongo_net ${SRCS_FILES} ${HEADERS_FILES})

include_directories(../)