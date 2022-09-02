project(unit_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()
add_executable(unit_tests vector_test.cpp)
target_link_libraries(unit_tests PUBLIC gtest gtest_main)
target_link_libraries(unit_tests PUBLIC blockadeRunnerLib) # Why tf is gtest throwing a fit over libraries
gtest_discover_tests(unit_tests)