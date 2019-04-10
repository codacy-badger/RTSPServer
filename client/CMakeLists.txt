include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} client_src)

add_library(client STATIC ${client_src})

add_style_check_target(client_cpplint "${client_src}")