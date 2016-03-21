cmake_minimum_required(VERSION 2.8.4)

set(Project_Name "CGS")
set(OGUI_VERSION_MAJOR 0)
set(OGUI_VERSION_MINOR 1)

# Build-specific locations. These will later be as dynamically determined as possible.
set(System_Include_Path "/c/msys2/mingw64/include")
set(System_Library_Path "/c/msys2/mingw64/lib")
set(Required_Platform_CXX_Flags "-mwin32") #-mwin32 is required on Windows with some MSYS (esp. MSYS2)

project(${Project_Name})

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/inc" # Internal includes
	"${System_Include_Path}"
)
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

link_directories(${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_LIBRARIES} ${System_Library_Path})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Required_Platform_CXX_Flags} -std=c++11 -Wall -Wno-unused-but-set-variable -Wno-unused-variable -Wshadow -Wno-deprecated-declarations -D__STDC_LIMIT_MACROS -ffast-math -g -o0") #-Wfatal-errors
add_library(${Project_Name} SHARED ${SOURCE_FILES})
target_link_libraries(${Project_Name} glew32 glu32 opengl32 SDL2main SDL2 freeimage)