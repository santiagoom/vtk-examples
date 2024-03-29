cmake_minimum_required(VERSION 3.17)
project(cpp-examples)

set(CMAKE_CXX_STANDARD 14)


if (MSVC)
    list(APPEND CMAKE_PREFIX_PATH "D:\\gitrepoe\\lang-tutorial\\02-utils\\cppUtils")
else ()
    list(APPEND CMAKE_PREFIX_PATH "/Users/tianm/repos/lang-tutorial/02-utils/cppUtils")
endif (MSVC)

find_package(cppUtils REQUIRED)

file(GLOB ALL_FILES *.cxx)
foreach (SOURCE_FILE ${ALL_FILES})
    string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
    add_executable(${EXAMPLE} ${EXAMPLE}.cxx)
    target_link_libraries(${EXAMPLE} ${VTK_LIBRARIES} ${cppUtils_LIBS})
endforeach ()

set(TARGET_NAME AnimateVectors)

if (MSVC)
    add_custom_command(TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${VTK_INSTALL_PREFIX}/bin/"
            $<TARGET_FILE_DIR:${TARGET_NAME}>
            )
endif (MSVC)

