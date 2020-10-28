cmake_minimum_required(VERSION 3.17)
project(_002_SimpleOperations)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_PREFIX_PATH "D:\\lib\\VTK-8.2.0\\VTK8_2-Install-VS2017-x64-Release\\lib\\cmake\\vtk-8.2")

#set(TARGET_TISH DistanceBetweenPoints)
#
### Find VTK
#find_package(VTK REQUIRED)
#
#include(${VTK_USE_FILE})
#include_directories(${VTK_INCLUDE_DIRS})
#
#add_executable(TARGET_TISH ${TARGET_TISH}.cxx)
#target_link_libraries(TARGET_TISH ${VTK_LIBRARIES})
#
#if (MSVC)
#  add_custom_command(TARGET TARGET_TISH
#          POST_BUILD
#          COMMAND ${CMAKE_COMMAND} -E copy_directory
#          "${VTK_INSTALL_PREFIX}/bin/"
#          $<TARGET_FILE_DIR:TARGET_TISH>
#          )
#endif (MSVC)

project (${WIKI}SimpleOperations)

if(NOT WikiExamples_BINARY_DIR)
  set(VTK_LIBRARIES "")
  find_package(VTK COMPONENTS
          vtkCommonCore
          vtkCommonDataModel
          vtkCommonMath
          vtkCommonTransforms
          QUIET
          )
  if (VTK_VERSION VERSION_LESS "8.90.0")
    include(${VTK_USE_FILE})
  endif()
endif()

set(KIT_LIBS ${VTK_LIBRARIES})

#
# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)
foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} ${EXECUTABLE_FLAG} ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${KIT_LIBS})
  if (VTK_VERSION VERSION_GREATER "8.8")
    vtk_module_autoinit(
            TARGETS ${WIKI}${EXAMPLE}
            MODULES ${VTK_LIBRARIES}
    )
  endif()
endforeach()

if (BUILD_TESTING)

  # Testing
  set(KIT SimpleOperations)

  set (EXCLUDE_TEST
          FloatingPointExceptions
          )

  include(${WikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)
  # set_tests_properties(
  #     SimpleOperations-FloatingPointExceptions
  #     PROPERTIES WILL_FAIL TRUE
  #     )
endif()


#
#message(STATUS "print_all_variables------------------------------------------{")
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#message(STATUS "print_all_variables------------------------------------------}")
#
