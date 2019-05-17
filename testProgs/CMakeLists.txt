include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/live555/liveMedia/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/live555/groupsock/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/live555/BasicUsageEnvironment/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/live555/UsageEnvironment/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} test_src)

add_style_check_target(test_cpplint "${test_src}")

add_executable(clientApp testClient.cpp)
add_executable(serverApp testServer.cpp)

target_link_libraries(clientApp skeleton client utils fmt avformat avcodec avutil
        boost_thread-mt boost_filesystem-mt boost_system-mt boost_chrono-mt)

target_link_libraries(serverApp skeleton server utils fmt
        liveMedia BasicUsageEnvironment UsageEnvironment groupsock
        boost_thread-mt boost_filesystem-mt boost_system-mt boost_chrono-mt)