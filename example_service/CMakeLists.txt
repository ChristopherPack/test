set(EXAMPLE_SERVICE_SOURCES
    example_service.cpp
)
list(TRANSFORM EXAMPLE_SERVICE_SOURCES PREPEND "${CMAKE_CURRENT_LIST_DIR}/")
add_executable(example_service)
target_sources(example_service PRIVATE ${EXAMPLE_SERVICE_SOURCES})
target_include_directories(example_service PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(example_service PRIVATE pht_lib)

add_custom_command(TARGET example_service POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/pht/$<CONFIG>/pht_lib.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/pht_lib.dll)