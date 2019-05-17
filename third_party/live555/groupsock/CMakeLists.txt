include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../UsageEnvironment/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} groupsock_src)

add_library(groupsock ${lib_style} ${groupsock_src})