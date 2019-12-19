cmake_minimum_required(VERSION 3.16)

set(THIS_PROJECT TestNinjaGen)
project(${THIS_PROJECT} LANGUAGES CXX)

set(Protobuf_USE_STATIC_LIBS TRUE)
set(CMAKE_PREFIX_PATH protobuf)
find_package(Protobuf REQUIRED)

add_executable(${THIS_PROJECT})

set(SOURCES
	main.cpp
)
target_sources(${THIS_PROJECT} PRIVATE ${SOURCES})
source_group("src" FILES ${SOURCES})


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS a.proto b.proto PROTOC_OUT_DIR GenProto)

target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/GenProto)
target_sources(${THIS_PROJECT} PRIVATE ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(${THIS_PROJECT} PRIVATE protobuf::libprotobuf)





