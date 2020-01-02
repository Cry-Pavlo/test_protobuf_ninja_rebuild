cmake_minimum_required(VERSION 3.16)

set(THIS_PROJECT TestNinjaGen)
project(${THIS_PROJECT} LANGUAGES CXX)


add_executable(FakeProtoGen generator.cpp)
set_property(TARGET FakeProtoGen PROPERTY CXX_STANDARD 17)
set(FAKEPROTOBUF_PROTOC_EXECUTABLE FakeProtoGen)

function(FAKEPROTOBUF_GENERATE_CPP OUTPUT_PATH SRCS HDRS)
	if(NOT ARGN)
		message(SEND_ERROR "Error: FAKEPROTOBUF_GENERATE_CPP() called without any proto files")
		return()
	endif()

	set(${SRCS})
	set(${HDRS})

	foreach(FIL ${ARGN})
		get_filename_component(ABS_INPUT_PROTO_FILE ${FIL} ABSOLUTE)
		get_filename_component(FIL_WE ${FIL} NAME_WE)

		set(GEN_PB_CC "${OUTPUT_PATH}/${FIL_WE}.pb.cc")
		set(GEN_PB_H  "${OUTPUT_PATH}/${FIL_WE}.pb.h")

		list(APPEND ${SRCS} ${GEN_PB_CC})
		list(APPEND ${HDRS} ${GEN_PB_H})

		message(STATUS "${FAKEPROTOBUF_PROTOC_EXECUTABLE} --cpp_out ${OUTPUT_PATH} ${ABS_INPUT_PROTO_FILE}")

		add_custom_command(
			OUTPUT ${GEN_PB_H} ${GEN_PB_CC}
			COMMAND  ${FAKEPROTOBUF_PROTOC_EXECUTABLE}
			ARGS --cpp_out ${OUTPUT_PATH} ${ABS_INPUT_PROTO_FILE}
			DEPENDS ${ABS_INPUT_PROTO_FILE} ${FAKEPROTOBUF_PROTOC_EXECUTABLE}
			COMMENT "Running C++ fake protocol buffer compiler on ${FIL}"
			VERBATIM )
	endforeach()

	set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED true)
	set(${SRCS} ${${SRCS}} PARENT_SCOPE)
	set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

function(process_fakeprotobuf FAKEPROTOBUF_GEN_PATH)
	set(FAKEPROTOBUF_INPUTS)
	foreach(SOURCEFILE ${SOURCES})
		if (${SOURCEFILE} MATCHES ".*\\.\\fakeproto$")
			set(FAKEPROTOBUF_INPUTS ${FAKEPROTOBUF_INPUTS} ${SOURCEFILE})
		endif()
	endforeach()
	message(STATUS "FAKEPROTOBUF_INPUTS = ${FAKEPROTOBUF_INPUTS}")
	set(FAKEPROTOBUF_GEN_CPP_SRC)
	set(FAKEPROTOBUF_GEN_H_SRC)
	message(STATUS "FAKEPROTOBUF_GEN_PATH = ${FAKEPROTOBUF_GEN_PATH}")
	FAKEPROTOBUF_GENERATE_CPP(${FAKEPROTOBUF_GEN_PATH} FAKEPROTOBUF_GEN_CPP_SRC FAKEPROTOBUF_GEN_H_SRC ${FAKEPROTOBUF_INPUTS})
	target_sources(${THIS_PROJECT} PRIVATE ${FAKEPROTOBUF_GEN_CPP_SRC} ${FAKEPROTOBUF_GEN_H_SRC})
	source_group("GeneratedFiles" FILES ${FAKEPROTOBUF_GEN_CPP_SRC} ${FAKEPROTOBUF_GEN_H_SRC})
endfunction()




add_executable(${THIS_PROJECT})

set(SOURCES
	main.cpp
	a.fakeproto 
	b.fakeproto
)
target_sources(${THIS_PROJECT} PRIVATE ${SOURCES})
source_group("src" FILES ${SOURCES})

set(FAKEPROTOBUF_GEN_PATH "${CMAKE_CURRENT_BINARY_DIR}/GeneratedFiles")
target_include_directories(${THIS_PROJECT} PRIVATE ${FAKEPROTOBUF_GEN_PATH})
process_fakeprotobuf(${FAKEPROTOBUF_GEN_PATH})





