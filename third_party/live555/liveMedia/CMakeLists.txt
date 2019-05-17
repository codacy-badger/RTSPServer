include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../UsageEnvironment/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../groupsock/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} liveMedia_src)

add_library(liveMedia ${lib_style} ${liveMedia_src})