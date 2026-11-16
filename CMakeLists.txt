cmake_minimum_required(VERSION 3.31.6)
project(scimitar_se_remapper C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB MAIN
"src/*"
)
add_executable(scimitar_remapd ${MAIN})

find_package(PkgConfig REQUIRED)

pkg_search_module(LIBEVDEV REQUIRED libevdev)
include_directories(${LIBEVDEV_INCLUDE_DIRS})

target_link_libraries(scimitar_remapd PRIVATE ${LIBEVDEV_LIBRARIES})

add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Building in debug mode")

add_custom_target(release
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Building in release mode")
