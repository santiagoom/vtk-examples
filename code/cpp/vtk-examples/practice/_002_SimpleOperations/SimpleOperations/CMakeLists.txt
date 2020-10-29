cmake_minimum_required(VERSION 3.17)
project(_002_SimpleOperations)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_PREFIX_PATH "D:\\lib\\VTK-8.2.0\\VTK8_2-Install-VS2017-x64-Release\\lib\\cmake\\vtk-8.2")

# Find VTK
find_package(VTK REQUIRED)

include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS})

file(GLOB ALL_FILES *.cxx)
foreach (SOURCE_FILE ${ALL_FILES})
    string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
    add_executable(${EXAMPLE} ${EXAMPLE}.cxx)
    target_link_libraries(${EXAMPLE} ${VTK_LIBRARIES})
endforeach ()

#add_executable(DistanceBetweenPoints DistanceBetweenPoints.cxx)
#target_link_libraries(DistanceBetweenPoints ${VTK_LIBRARIES})

if (MSVC)
    add_custom_command(TARGET DistanceBetweenPoints
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${VTK_INSTALL_PREFIX}/bin/"
            $<TARGET_FILE_DIR:DistanceBetweenPoints>
            )
endif (MSVC)
#
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