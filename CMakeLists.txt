cmake_minimum_required(VERSION 3.0.0)
project(wxCapture VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} wxcapture.cpp convertmattowxbmp.cpp convertmattowxbmp.h)

find_package(wxWidgets)
# sudo apt install libwxbase*-dev libwxgtk*-dev

set(wxWidgets_CONFIGURATION mswu)
if (WIN32)
    set(CMAKE_CXX_FLAGS "-mwindows")
endif (WIN32)
include(${wxWidgets_USE_FILE})


#set(OpenCV_DIR "/usr/lib/opencv")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${OpenCV_LIBS} -lstdc++fs)