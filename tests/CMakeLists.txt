

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc ${CMAKE_CURRENT_BINARY_DIR})

create_test_sourcelist(tests
    all_test.c
    test_sys.c
    test_filestream.c
    test_thread.c
)

add_executable(all-gef-sys-tests ${tests})
target_link_libraries(all-gef-sys-tests gef-sys gef-core)

set(all-gef-sys-tests-to-run ${tests})
remove(all-gef-sys-tests-to-run all_test.c)

foreach(test ${all-gef-sys-tests-to-run})
    get_filename_component(tname ${test} NAME_WE)
    add_test(NAME ${tname} COMMAND all-gef-sys-tests ${tname})
endforeach()