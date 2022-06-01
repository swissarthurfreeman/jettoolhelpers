message("Building utils...")

# we have google benchmark locally only.
if(NOT IN_PRODUCTION)
    message("Configuring Benchmarking...")
    find_package(benchmark REQUIRED)

    add_executable(perf_test perf_test.cpp)
    target_link_libraries(perf_test JetToolHelpers benchmark::benchmark)
endif()

# copy test data to build folder to be accessed by the executables.
# configure_file(data/R4_AllComponents.root ${CMAKE_CURRENT_BINARY_DIR}/R4_AllComponents.root COPYONLY)
file(COPY data/R4_AllComponents.root DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(usage_example  usage_example.cpp)
target_link_libraries(usage_example JetToolHelpers)
#target_include_directories(JetContextUnitTest PUBLIC ".")