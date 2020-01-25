cmake_minimum_required(VERSION 3.15)

project(mcm_file_transmitter)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "../SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

include_directories("src/header_files/" "src/header_files/client/" "src/header_files/server/")

add_executable(mcm_file_transmitter
src/source_files/server/wServerThread.cpp
src/source_files/server/wClientThread.cpp
src/source_files/client/wreceiver.cpp
src/source_files/client/wclient.cpp
src/source_files/wmain.cpp
)

target_link_libraries(mcm_file_transmitter sfml-graphics)