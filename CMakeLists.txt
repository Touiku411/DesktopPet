cmake_minimum_required(VERSION 3.16)
project(DesktopPet VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 尋找 Qt6 套件
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# 啟用 Qt 的 Meta-Object Compiler, UI compiler 支援
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 添加執行檔
add_executable(DesktopPet main.cpp assets.qrc)

# 連結 Qt 函式庫
target_link_libraries(DesktopPet PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
