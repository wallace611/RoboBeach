cmake_minimum_required(VERSION 3.12)

project("RoboBeach"
    VERSION 0.1.0
    LANGUAGES CXX C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # General
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic-errors")
    # Debug mode
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -D_GLIBC_DEBUG")
# LLVM Clang
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # General
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic-errors")
    # Debug mode
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
# Microsoft Visual C++
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # General
    if (CMAKE_C_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
    endif()
endif()
#[[ End of Compiler Flags ]]

# Print out compiler settings
message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
message(STATUS "C Debug Flags: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C Release Flags: ${CMAKE_C_FLAGS_RELEASE}")

# Find external libraries
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(cglm REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/engine)
include_directories(${CMAKE_SOURCE_DIR}/code)

set(Examples
    "main"
)

foreach(ex IN LISTS Examples)
    message(STATUS "Configuring ${ex}...")

    add_executable(${ex})
    target_sources(
        ${ex} PRIVATE "${ex}.c"   
        "engine/Engine.h" 
        "engine/Engine.c"  
         
          
        "engine/input/InputMapper.h" 
        "engine/input/InputMapper.c" 
        "engine/object/Object.h" 
        "engine/object/Object.c"
        "engine/utils/Shapes.h" 
        "engine/scene/World.h" 
        "engine/scene/World.c"   
        "engine/utils/ObjContainer.h"   
        "engine/utils/ObjContainer.c"    
        "engine/object/Camera.h" 
        "engine/object/Camera.c" 
        "engine/object/component/Component.h"
        "engine/object/component/Component.c"
        "engine/ui/DebugText.h"
        "engine/ui/DebugText.c" 
        "engine/utils/String.h"
        "engine/utils/String.c" 
        "engine/utils/Shapes.c"

        "code/object/Triangle.h" 
        "code/object/Triangle.c" 
        "engine/object/utils.h" 
        "engine/object/utils.c" 
        "engine/object/CollisionShape.h" 
        "engine/object/CollisionShape.c" 
        "code/object/robot/Robot.h" 
        "code/object/robot/Robot.c" 
        "code/object/robot/components/BotBody.h" 
        "code/object/robot/components/BotBody.c" 
        "code/object/robot/components/head/BotHead.h" 
        "code/object/robot/components/head/BotHead.c" 
        "engine/object/component/Connector.h" 
        "engine/object/component/Connector.c" 
        "code/object/robot/components/arm/BotArm.h" 
        "code/object/robot/components/arm/BotArm.c"  
        "code/object/Floor.h" "code/object/Floor.c" 
        "engine/object/component/Animation.h" 
        "engine/object/component/Animation.c"   
        "code/object/Rock.h" 
        "code/object/Rock.c" 
        "code/Test.c" 
        "code/RoboBeach.c"
        "code/RoboBeach.h" 
        "code/Test.h"
     "code/object/Umbrella.h" "code/object/Umbrella.c" "code/object/Fan.h" "code/object/Fan.c")
    target_link_libraries(${ex} PRIVATE GLUT::GLUT)

    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_libraries(${ex} PRIVATE
            OpenGL::OpenGL
            OpenGL::GLU
            m # Required by math.h
            X11 # Required by freeglut static linkage
        )
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_link_libraries(${ex} PRIVATE
            OpenGL32
        )
    endif()
endforeach()