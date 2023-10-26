include_directories(${CMAKE_SOURCE_DIR}/include)

set(TEST_BUILD_NAME tk_filter_test)
add_executable(${TEST_BUILD_NAME} tk_filter_test.cpp)
TARGET_LINK_LIBRARIES(${TEST_BUILD_NAME} tk_grpc_proto -lcryptopp)

set(TEST_BUILD_NAME mlemm_test)
add_executable(${TEST_BUILD_NAME} mlemm_test.cpp)
TARGET_LINK_LIBRARIES(${TEST_BUILD_NAME} tk_grpc_proto -lcryptopp)

set(TEST_BUILD_NAME conj_filter_test)
add_executable(${TEST_BUILD_NAME} conj_filter_test.cpp)
TARGET_LINK_LIBRARIES(${TEST_BUILD_NAME} tk_grpc_proto -lcryptopp)

set(TEST_BUILD_NAME tk_filter_perf)
add_executable(${TEST_BUILD_NAME} tk_filter_perf.cpp  test_utils.cpp)
TARGET_LINK_LIBRARIES(${TEST_BUILD_NAME} tk_grpc_proto -lcryptopp)

set(TEST_BUILD_NAME conj_filter_perf)
add_executable(${TEST_BUILD_NAME} conj_filter_perf.cpp  test_utils.cpp)
TARGET_LINK_LIBRARIES(${TEST_BUILD_NAME} tk_grpc_proto -lcryptopp)