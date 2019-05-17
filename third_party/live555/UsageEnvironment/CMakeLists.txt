include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../groupsock/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} UsageEnvironment_src)

add_library(UsageEnvironment ${lib_style} ${UsageEnvironment_src})