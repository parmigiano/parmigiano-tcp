cmake_minimum_required(VERSION 4.1.0)

set(PROJECT_NAME ServerMain)
set(EXECUTABLE_NAME ServerMain)

project(${PROJECT_NAME}) # default LANGUAGE C CXX

set(GENERATED_PROTO_DIR "${CMAKE_BINARY_DIR}/generatedProto")
file(MAKE_DIRECTORY ${GENERATED_PROTO_DIR})

include(FetchContent)

#set(PostgreSQL_INCLUDE_DIR "C:/Program Files/PostgreSQL/18/include")
#set(PostgreSQL_LIBRARY "C:/Program Files/PostgreSQL/18/lib/libpq.lib")

find_package(PostgreSQL REQUIRED)

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

FetchContent_Declare(
    libpqxx
    GIT_REPOSITORY https://github.com/jtv/libpqxx.git
    GIT_TAG 7.10.1
)

set(PQXX_LIBRARIES pqxx_static)

FetchContent_MakeAvailable(protobuf boost libpqxx)

include(${protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)

file(GLOB SOURCES_CPP
	./include/*.h
	./src/*.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES_CPP})

protobuf_generate(
    TARGET ${EXECUTABLE_NAME}
    LANGUAGE cpp
	PROTOS
		${CMAKE_CURRENT_SOURCE_DIR}/protoStructs/ClientRequestStruct.proto
		${CMAKE_CURRENT_SOURCE_DIR}/protoStructs/ResponseStruct.proto
    PROTOC_OUT_DIR ${GENERATED_PROTO_DIR}
    IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/protoStructs
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE # replace #include "../include/ConnectionManager.h" to #include "ConnectionManager.h"
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${GENERATED_PROTO_DIR}
    #${PostgreSQL_INCLUDE_DIR}
)

target_link_libraries(${EXECUTABLE_NAME} # link protobuf and boost with project
	PRIVATE 
	protobuf::libprotobuf
	Boost::asio 
	Boost::system
    pqxx
    #${PostgreSQL_LIBRARY}
) 

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

add_definitions(-D_WIN32_WINNT=0x0601)

add_custom_target(copy_exe ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/executable
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${EXECUTABLE_NAME}>
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/$<TARGET_FILE_NAME:${EXECUTABLE_NAME}>
    DEPENDS ${EXECUTABLE_NAME}
    COMMENT "Copying executable to folder"
)

add_custom_target(dist
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_SOURCE_DIR}/executable
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/executable
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${EXECUTABLE_NAME}>
        ${CMAKE_CURRENT_SOURCE_DIR}/executable/
    COMMENT "Creating clean distribution"
)

add_dependencies(dist ${EXECUTABLE_NAME})