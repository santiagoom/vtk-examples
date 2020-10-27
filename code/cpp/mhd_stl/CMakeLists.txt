cmake_minimum_required(VERSION 3.17)
project(mhd2stl)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_PREFIX_PATH "D:\\lib\\VTK-8.2.0\\VTK8_2-Install-VS2017-x64-Release\\lib\\cmake\\vtk-8.2")

## Find VTK
find_package(VTK REQUIRED)

include_directories(${VTK_INCLUDE_DIRS})

add_executable(mhd2stl mhd2stl.cpp)
target_link_libraries(mhd2stl ${VTK_LIBRARIES})

add_executable(poly2vol poly2vol.cpp)
target_link_libraries(poly2vol ${VTK_LIBRARIES})

#
#message(STATUS "print_all_variables------------------------------------------{")
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#message(STATUS "print_all_variables------------------------------------------}")
#

