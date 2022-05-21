add_executable(JetContextUnitTest "./JetContextUnitTest.cpp")
add_executable(InputVariableUnitTest "./InputVariableUnitTest.cpp")

# is available because of compilation order
target_link_libraries(myTest JetToolHelpersLib)
target_include_directories(myTest PUBLIC ".")

target_link_libraries(JetContextUnitTest JetToolHelpersLib)
target_include_directories(JetContextUnitTest PUBLIC ".")

target_link_libraries(InputVariableUnitTest JetToolHelpersLib)
target_include_directories(InputVariableUnitTest PUBLIC ".")

add_test(firstTest myTest)
add_test(JetContextUnitTest JetContextUnitTest)
add_test(InputVariableUnitTest InputVariableUnitTest)