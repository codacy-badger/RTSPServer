include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} rtsp_src)

add_style_check_target(rtsp_cpplint "${rtsp_src}")

add_definitions(-DALLOW_SERVER_PORT_REUSE)

add_library(rtsp STATIC ${rtsp_src})