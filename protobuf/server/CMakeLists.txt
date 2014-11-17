find_package(Boost 1.55 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
aux_source_directory(. SERVER_SRCS)
add_executable(server ${SERVER_SRCS})

target_link_libraries(server helloworld ${Boost_LIBRARIES} ${PROTOBUF_LIBRARY})
