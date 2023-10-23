cmake_minimum_required(VERSION 3.5.0)

project(TKFilter C CXX)

include(common.cmake)

get_filename_component(tk_proto "${CMAKE_SOURCE_DIR}/idl/TKFilter.proto" ABSOLUTE)
get_filename_component(tk_proto_path "${tk_proto}" PATH)

set(tk_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/TKFilter.pb.cc")
set(tk_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/TKFilter.pb.h")
set(tk_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/TKFilter.grpc.pb.cc")
set(tk_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/TKFilter.grpc.pb.h")
add_custom_command(
      OUTPUT "${tk_proto_srcs}" "${tk_proto_hdrs}" "${tk_grpc_srcs}" "${tk_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${tk_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${tk_proto}"
      DEPENDS "${tk_proto}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary)

option(TEST "start testing" OFF)
if (TEST)
  message("-- Note: Start testing, terminal include test results")
  add_definitions(-DTEST)
endif(TEST)

ADD_SUBDIRECTORY(test)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(src/tk_filter crypto_srcs)
aux_source_directory(src/conj_filter crypto_srcs)
aux_source_directory(src/melmm crypto_srcs)
aux_source_directory(src/util crypto_srcs)

aux_source_directory(src/rpc/server crypto_srcs)
aux_source_directory(src/rpc/client crypto_srcs)

add_library(tk_grpc_proto
  ${tk_grpc_srcs}
  ${tk_grpc_hdrs}
  ${tk_proto_srcs}
  ${tk_proto_hdrs}
  ${crypto_srcs})
target_link_libraries(tk_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

foreach(_target client server)
  add_executable(${_target} "./src/rpc/${_target}_main.cc")
  target_link_libraries(${_target}
    tk_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    -lcryptopp -O3)
endforeach()