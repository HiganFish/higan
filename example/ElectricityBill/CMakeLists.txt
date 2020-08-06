SET(SRC_FILES
        main.cpp
        ElectricityBill.cpp
)

SET(HEADER_FILES
        ElectricityBill.h
)

add_executable(ElectricityBill ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(ElectricityBill higan_http)

include_directories(${CMAKE_SOURCE_DIR})