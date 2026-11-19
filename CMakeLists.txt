# CMakeList.txt : CMake project for LogicSimulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 17)
project ("LogicSimulator")

# ԱՆՊԱՅՄԱՆ պետք է ներառել այս մոդուլը մինչև FetchContent հրամաններն օգտագործելը
include(FetchContent)

# Install GLFW using FetchContent
FetchContent_Declare(
	glfw
	URL https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip
)
FetchContent_MakeAvailable(glfw)

# Install Dear ImGui using FetchContent
FetchContent_Declare(
	imgui
	URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.tar.gz
)
FetchContent_MakeAvailable(imgui)

# Setting the include directories for GLFW and ImGui
set(IMGUI_DIR ${imgui_SOURCE_DIR})

add_library(imgui_target STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Սահմանում ենք ինքլուդների ուղիները, որպեսզի ֆայլերը տեսնեն իրար
target_include_directories(imgui_target PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# ImGui-ի բեքենդը կախված է GLFW-ից, ուստի կապում ենք glfw-ն
target_link_libraries(imgui_target PUBLIC glfw)

# Add source to this project's executable.
add_executable (LogicSimulator   "src/Gate.h" "src/Simulator.h" "src/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LogicSimulator PROPERTY CXX_STANDARD 20)
endif()

# Կապում ենք մեր ծրագիրը imgui_target-ի հետ
target_link_libraries(LogicSimulator PRIVATE imgui_target)

if(WIN32)
    target_link_libraries(LogicSimulator PRIVATE opengl32)
endif()