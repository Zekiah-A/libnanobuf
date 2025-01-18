# Add test executables
add_executable(test_read_boundary_str test_read_boundary_str.c)
add_executable(test_read_cstr test_read_cstr.c)
add_executable(test_read_empty_str test_read_empty_str.c)
add_executable(test_read_medium_str test_read_medium_str.c)
add_executable(test_read_primitives test_read_primitives.c)
add_executable(test_read_short_str test_read_short_str.c)
add_executable(test_read_utf8_str test_read_utf8_str.c)

add_executable(test_write_primitives test_write_primitives.c)

# Link the test executables with the nanobuf library (static or shared)
target_link_libraries(test_read_boundary_str nanobuf)
target_link_libraries(test_read_cstr nanobuf)
target_link_libraries(test_read_empty_str nanobuf)
target_link_libraries(test_read_medium_str nanobuf)
target_link_libraries(test_read_primitives nanobuf)
target_link_libraries(test_read_short_str nanobuf)
target_link_libraries(test_read_utf8_str nanobuf)

target_link_libraries(test_write_primitives nanobuf)

# Add tests to CTest
add_test(NAME test_read_boundary_str COMMAND test_read_boundary_str)
add_test(NAME test_read_cstr COMMAND test_read_cstr)
add_test(NAME test_read_empty_str COMMAND test_read_empty_str)
add_test(NAME test_read_medium_str COMMAND test_read_medium_str)
add_test(NAME test_read_primitives COMMAND test_read_primitives)
add_test(NAME test_read_short_str COMMAND test_read_short_str)
add_test(NAME test_read_utf8_str COMMAND test_read_utf8_str)

add_test(NAME test_write_primitives COMMAND test_write_primitives)
