include_directories(../GameComponent)

SET(LOGIN_SERVER_SRCS
        LoginServer/LoginGateServer.cpp
        LoginServer/LoginNetworkPackage.cpp
        LoginServer/UserManager.cpp
)

SET(LOGIN_SERVER_HEADERS
        LoginServer/LoginNetworkPackage.h
        LoginServer/UserManager.h
)

SET(CODEC_SERVER
    CodecServer.cpp
    CodecServer.h
    NetworkPackageBase.cpp
    NetworkPackageBase.h
)
add_library(higan_game ${CODEC_SERVER})
target_link_libraries(higan_game higan_net)

add_executable(GameLoginServer ${LOGIN_SERVER_SRCS} ${LOGIN_SERVER_HEADERS})
target_link_libraries(GameLoginServer higan_game)