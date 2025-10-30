cmake_minimum_required(VERSION 4.1.0)

set(PROJECT_NAME ServerMain)
set(EXECUTABLE_NAME ServerMain)
set(PROTO_FILES ./protoStructs)

project(${PROJECT_NAME}) # default LANGUAGE C CXX

include(FetchContent)

# get and build protocol buffers
FetchContent_Declare(
  protobuf
  GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
  GIT_TAG "v32.0" 
)

set(protobuf_BUILD_TESTS OFF) # disable tests
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")

# get and build boost.asio
FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG "boost-1.89.0"
)

set(BOOST_INCLUDE_LIBRARIES system asio)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_MakeAvailable(protobuf boost)

include(${protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)

file(GLOB SOURCES
	./include/*.h
	./src/*.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_sources(${EXECUTABLE_NAME} PRIVATE 
	${PROTO_FILES}/ClientRequestStruct.proto
	${PROTO_FILES}/ResponseStruct.proto
)

protobuf_generate(TARGET ${EXECUTABLE_NAME} 
	PROTOS
	${PROTO_FILES}/ClientRequestStruct.proto
	${PROTO_FILES}/ResponseStruct.proto
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE # replace #include "../include/ConnectionManager.h" to #include "ConnectionManager.h"
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${EXECUTABLE_NAME} # link protobuf and boost with project
	PRIVATE 
	protobuf::libprotobuf
	Boost::asio 
	Boost::system
) 

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

add_definitions(-D_WIN32_WINNT=0x0601)