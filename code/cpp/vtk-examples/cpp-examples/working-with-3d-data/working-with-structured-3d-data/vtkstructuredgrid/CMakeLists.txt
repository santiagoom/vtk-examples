cmake_minimum_required(VERSION 3.17)
project(cpp-examples)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_PREFIX_PATH "D:\\lib\\VTK-8.2.0\\VTK8_2-Install-VS2017-x64-Release\\lib\\cmake\\vtk-8.2")

## Find VTK
find_package(VTK REQUIRED)

include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS})

# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)
foreach (SOURCE_FILE ${ALL_FILES})
    string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
    add_executable(${EXAMPLE} ${EXAMPLE}.cxx)
    target_link_libraries(${EXAMPLE} ${VTK_LIBRARIES})
endforeach ()

#file(GLOB ALL_FILES *.cpp)
#foreach (SOURCE_FILE ${ALL_FILES})
#    string(REPLACE ".cpp" "" TMP ${SOURCE_FILE})
#    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
#    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
#    target_link_libraries(${EXAMPLE} ${VTK_LIBRARIES})
#endforeach ()

set(TARGET_NAME SGrid)

if (MSVC)
    add_custom_command(TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${VTK_INSTALL_PREFIX}/bin/"
            $<TARGET_FILE_DIR:${TARGET_NAME}>
            )
endif (MSVC)
