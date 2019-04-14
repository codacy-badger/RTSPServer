include_directories(${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../live555/liveMedia/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../live555/groupsock/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../live555/BasicUsageEnvironment/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../live555/UsageEnvironment/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} server_src)

add_style_check_target(server_cpplint "${server_src}")

add_definitions(-DALLOW_SERVER_PORT_REUSE)

add_library(server STATIC ${server_src})