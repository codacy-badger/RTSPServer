include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} utils_src)

add_library(utils STATIC ${utils_src})

add_style_check_target(utils_cpplint "${utils_src}")