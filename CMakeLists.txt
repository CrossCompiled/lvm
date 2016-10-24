cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

IF(DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
ELSE()
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
ENDIF()

set(LECTURE LittleVirtualMachine)

project(${LECTURE} VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

find_package(Boost REQUIRED)

# Libraries
add_library(${LECTURE} src/LittleCompiler/CodeGenerator.cpp)
target_include_directories(${LECTURE} PUBLIC includes)
target_link_libraries(${LECTURE} ${Boost_LIBRARIES})
target_include_directories(${LECTURE} PUBLIC ${Boost_INCLUDE_DIRS})
set_target_properties(${LECTURE} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES)
IF(DEFINED STATIC_STACK)
    target_compile_definitions(${LECTURE} PUBLIC STATIC_STACK=1)
ENDIF()

#LittleApplication
add_executable(LittleApplication src/LittleApplication/main.cpp)
target_link_libraries(LittleApplication ${LECTURE} ncurses++w formw menuw panelw ncursesw)

set_target_properties(LittleApplication PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)

# LittleCompiler
add_executable(LittleCompiler src/LittleCompiler/main.cpp)
target_link_libraries(LittleCompiler ${LECTURE})

set_target_properties(LittleCompiler PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES)

# LittleInterpreter
add_executable(LittleInterpreter src/LittleInterpreter/main.cpp)
target_link_libraries(LittleInterpreter ${LECTURE})
IF(DEFINED STATIC_STACK)
target_compile_definitions(LittleInterpreter PUBLIC STATIC_STACK=${STATIC_STACK})
ENDIF()
IF(DEFINED STATIC_PROGRAM)
    target_compile_definitions(LittleInterpreter PUBLIC STATIC_PROGRAM=${STATIC_PROGRAM})
ENDIF()
IF(DEFINED STATIC_MEMORY)
    target_compile_definitions(LittleInterpreter PUBLIC STATIC_MEMORY=${STATIC_MEMORY})
ENDIF()
set_target_properties(LittleInterpreter PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES)


#########
# TESTS
#########
enable_testing()
find_package(GTest REQUIRED)

set(LTESTS
        test/LittleCompiler/test_exercise02_binaru.cpp
        test/LittleInterpeter/test_exercise03_Typelist.cpp
        test/LittleCompiler/test_compiler_states.cpp
        test/LittleCompiler/test_codegenerator.cpp
        )

foreach(testfile ${LTESTS})
    get_filename_component(name ${testfile} NAME_WE)
    add_executable(${name} "${testfile}")
    target_link_libraries(${name} ${LECTURE})
    target_link_libraries(${name} ${GMOCK_MAIN_LIBRARY})
    set_target_properties(${name} PROPERTIES
            CXX_STANDARD 14
            CXX_STANDARD_REQUIRED YES)
    if(MSVC)
        set_target_properties(${name} PROPERTIES
                COMPILE_FLAGS "-MTd")
    else()
        set_target_properties(${name} PROPERTIES
                COMPILE_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage"
                LINK_FLAGS "-fprofile-arcs -ftest-coverage")
    endif()
    add_test(${name} ${name})
endforeach()
