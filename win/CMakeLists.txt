cmake_minimum_required(VERSION 3.15)

project(mcm_file_transmitter)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "../SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

include_directories("src/server/hpp/" "src/client/hpp/" "src/")

add_executable(mcm_file_transmitter
src/server/cpp/wServerThread.cpp
src/server/cpp/wClientThread.cpp
src/client/cpp/wreceiver.cpp
src/client/cpp/wclient.cpp
src/wmain.cpp
)

target_link_libraries(mcm_file_transmitter sfml-graphics)