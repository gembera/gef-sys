cmake_minimum_required(VERSION 3.21)
project(gef-sys)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-DENABLE_MEM_PROFILE")
    add_definitions("-DENABLE_MEM_RECORD")
endif()

aux_source_directory(src DIR_SRC_GEF_SYS)

add_library(gef-sys STATIC ${DIR_SRC_GEF_SYS})

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(pkgs/gef-core)
    target_include_directories(gef-sys PRIVATE
        pkgs/gef-core/inc
    )
else()
    target_include_directories(gef-sys PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../gef-core/inc
    )
endif()

target_include_directories(gef-sys PUBLIC
    inc
)

if(PROJECT_IS_TOP_LEVEL)
    message(STATUS "${PROJECT_NAME} enables testing.")
    enable_testing()
    add_subdirectory(tests)
endif()