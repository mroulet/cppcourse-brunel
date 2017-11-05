cmake_minimum_required (VERSION 2.6)
project (neuron)

set(CMAKE_CXX_FLAGS "-W -Wall -pedantic -std=c++11 -O3")

enable_testing()
add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR} include ${gtest_SOURCE_DIR})

add_executable (neuron src/network.cpp src/neuron.cpp src/test_multipleNeurons.cpp)
add_executable (neuron_unittest src/neuron.cpp src/network.cpp gtest/neuron_unittest.cpp)

target_link_libraries(neuron_unittest gtest gtest_main)
add_test(neuron_unittest neuron_unittest)

###### Doxygen generation ######

# We first check if Doxygen is present.
find_package(Doxygen)
if(DOXYGEN_FOUND)

    # We then check if the Doxygen.in configuration file exists
    IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
        message(FATAL_ERROR "Doxygen configuration file is needed to build the documentation:
         - ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in not found.
         - Generate one in the project folder with doxygen -g Doxyfile.in")
    ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)

    # We configure the doxyfile for make.
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen
    )
ELSE(DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
ENDIF(DOXYGEN_FOUND)

