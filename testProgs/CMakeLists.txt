aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} test_src)

add_style_check_target(test_cpplint "${test_src}")

add_executable(clientApp testClient.cpp)

target_link_libraries(clientApp skeleton client utils fmt avformat avcodec avutil
        boost_thread-mt boost_filesystem-mt boost_system-mt boost_chrono-mt)