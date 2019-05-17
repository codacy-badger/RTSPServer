include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../groupsock/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../UsageEnvironment/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} BasicUsageEnvironment_src)

add_library(BasicUsageEnvironment ${lib_style} ${BasicUsageEnvironment_src})