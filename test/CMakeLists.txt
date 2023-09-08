add_executable(test_thread thread.cc)
target_link_libraries(test_thread yf_shared)

add_executable(test_log log.cc)
target_link_libraries(test_log yf_shared)

add_executable(test_config config.cc)
target_link_libraries(test_config yf_shared)

add_executable(test_buffer buffer.cc)
target_link_libraries(test_buffer yf_shared)

add_executable(test_address address.cc)
target_link_libraries(test_address yf_shared)

add_executable(test test.cc)