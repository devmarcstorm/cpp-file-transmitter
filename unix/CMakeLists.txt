cmake_minimum_required(VERSION 3.15)

project(mcm_file_transmitter)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "../SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

include_directories(
"src/header_files/"
"src/header_files/client/"
"src/header_files/server/"
"src/header_files/state/"
"src/header_files/gui/"
)

add_executable(mcm_file_transmitter
src/source_files/server/serverThread.cpp
src/source_files/server/clientThread.cpp
src/source_files/client/receiver.cpp
src/source_files/client/client.cpp
src/source_files/gui/guiManager.cpp
src/source_files/gui/panel.cpp
src/source_files/gui/button.cpp
src/source_files/gui/textbox.cpp
src/source_files/gui/textArea.cpp
src/source_files/state/stateManager.cpp
src/source_files/state/mainmenu.cpp
src/source_files/state/premenuClient.cpp
src/source_files/state/clientMenu.cpp
src/source_files/state/serverMenu.cpp
src/source_files/display.cpp
src/source_files/settings.cpp
src/source_files/main.cpp
)

target_link_libraries(mcm_file_transmitter sfml-graphics)