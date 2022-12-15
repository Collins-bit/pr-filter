cmake_minimum_required(VERSION 3.5.0)

project(PrFilter C CXX)

include(common.cmake)

get_filename_component(pr_proto "${CMAKE_SOURCE_DIR}/idl/PrFilter.proto" ABSOLUTE)
get_filename_component(pr_proto_path "${pr_proto}" PATH)

set(pr_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/PrFilter.pb.cc")
set(pr_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/PrFilter.pb.h")
set(pr_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/PrFilter.grpc.pb.cc")
set(pr_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/PrFilter.grpc.pb.h")
add_custom_command(
      OUTPUT "${pr_proto_srcs}" "${pr_proto_hdrs}" "${pr_grpc_srcs}" "${pr_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${pr_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${pr_proto}"
      DEPENDS "${pr_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(src/algorithm crypto_srcs)
aux_source_directory(src/util crypto_srcs)

add_library(pr_grpc_proto
  ${pr_grpc_srcs}
  ${pr_grpc_hdrs}
  ${pr_proto_srcs}
  ${pr_proto_hdrs}
  ${crypto_srcs})
target_link_libraries(pr_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

foreach(_target client server)
  add_executable(${_target} "./src/rpc/${_target}.cc")
  target_link_libraries(${_target}
    pr_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()