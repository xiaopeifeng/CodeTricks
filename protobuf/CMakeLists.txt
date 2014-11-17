project (google_protobuf)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR})
aux_source_directory(. DIR_SRCS)

add_definitions(-std=c++11)
add_subdirectory(pb)

add_subdirectory(client)

add_subdirectory(server)

add_executable(googleproto ${DIR_SRCS})

target_link_libraries(googleproto helloworld ${PROTOBUF_LIBRARY})
