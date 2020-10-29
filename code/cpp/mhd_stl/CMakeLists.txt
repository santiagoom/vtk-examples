cmake_minimum_required(VERSION 3.17)
project(mhd2stl)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_PREFIX_PATH "D:\\lib\\VTK-8.2.0\\VTK8_2-Install-VS2017-x64-Release\\lib\\cmake\\vtk-8.2")

set(TARGET_TISH poly2vol)

## Find VTK
find_package(VTK REQUIRED)

include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS})

add_executable(TARGET_TISH ${TARGET_TISH}.cpp)
target_link_libraries(TARGET_TISH ${VTK_LIBRARIES})

if (MSVC)
  add_custom_command(TARGET TARGET_TISH
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${VTK_INSTALL_PREFIX}/bin/"
          $<TARGET_FILE_DIR:TARGET_TISH>
          )
endif (MSVC)


#
#message(STATUS "print_all_variables------------------------------------------{")
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#message(STATUS "print_all_variables------------------------------------------}")
#
