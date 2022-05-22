# define executable targets
add_executable(JetContextUnitTest "./JetContextUnitTest.cpp")
add_executable(InputVariableUnitTest "./InputVariableUnitTest.cpp")

# link required libraries and includes.
# "JetToolHelpers" is available because of compilation order
target_link_libraries(JetContextUnitTest JetToolHelpers)
target_include_directories(JetContextUnitTest PUBLIC ".")

target_link_libraries(InputVariableUnitTest JetToolHelpers)
target_include_directories(InputVariableUnitTest PUBLIC ".")

# declare them as CMake tests. (will be ran via ctest --verbose)
add_test(JetContextUnitTest JetContextUnitTest)
add_test(InputVariableUnitTest InputVariableUnitTest)