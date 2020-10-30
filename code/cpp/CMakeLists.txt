cmake_minimum_required(VERSION 3.17)
project(vtk-cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(cppUtils/utils)
add_subdirectory(cppUtils)
list(APPEND EXTRA_LIBS cppUtils)

#add_subdirectory(mhd_stl)

#add_subdirectory(vtk-examples/practice/_001_HelloWorld/GeometricObjects)
#add_subdirectory(vtk-examples/practice/_002_SimpleOperations/SimpleOperations)

#add_subdirectory(vtk-examples/vtk-tutorial)
#add_subdirectory(vtk-examples/cpp-examples/hello-world)
#add_subdirectory(vtk-examples/cpp-examples/simple-operations)
add_subdirectory(vtk-examples/cpp-examples/input-and-output/3d-file-formats)

