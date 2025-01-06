# Find the nanobuf library (this assumes that the nanobuf library is already built or installed)
add_executable(test_read read.c)
add_executable(test_write write.c)

# Link the test executables with the nanobuf library (static or shared)
target_link_libraries(test_read nanobuf)
target_link_libraries(test_write nanobuf)

# Add tests to CTest
add_test(NAME test_read COMMAND test_read)
add_test(NAME test_write COMMAND test_write)
