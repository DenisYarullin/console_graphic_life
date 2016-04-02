project(tests)
cmake_minimum_required(VERSION 3.2)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Add test cpp file
set(test1_source main.cpp)
add_executable(test1 ${test1_source})

# Link test executable against gtest & gtest_main
target_link_libraries(test1 ${GTEST_BOTH_LIBRARIES} pthread)
#target_link_libraries(runUnitTests ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG}

add_test(NAME Test1 COMMAND test1)


#find_package (Boost COMPONENTS unit_test_framework REQUIRED)
#make test ARGS=”-V”
