find_package(Boost 1.55 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
aux_source_directory(. CLIENT_SRCS)

add_executable(client ${CLIENT_SRCS})

target_link_libraries(client helloworld ${Boost_LIBRARIES} ${PROTOBUF_LIBRARY})
