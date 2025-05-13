cmake_minimum_required(VERSION 3.16)

project(newAOE VERSION 0.1 LANGUAGES CXX)

# 启用Qt自动处理系统
set(CMAKE_AUTOUIC ON)   # 自动处理Qt的UI文件
set(CMAKE_AUTOMOC ON)   # 自动处理Qt的元对象编译器(MOC)
set(CMAKE_AUTORCC ON)   # 自动处理Qt的资源文件

# 设置C++标准版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置Qt的安装路径
set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64")

# 查找 Qt 模块
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

# 自动处理 .ui 文件
set(PROJECT_UI
    MainWidget.ui
    GameWidget.ui
    SelectWidget.ui
    Option.ui
    AboutDialog.ui
)
# 处理UI文件并生成对应的头文件
qt_wrap_ui(PROJECT_SOURCES ${PROJECT_UI})

# 源文件和头文件
set(PROJECT_SOURCES
main.cpp               # 程序入口
MainWidget.cpp         # 主窗口部件
GameWidget.cpp         # 游戏显示部件
Coordinate.cpp         # 坐标相关类
GlobalVariate.cpp      # 全局变量
Map.cpp                # 地图类
Block.cpp              # 地图块类
MoveObject.cpp         # 可移动对象基类
Building.cpp           # 建筑基类
Resource.cpp           # 资源基类
Human.cpp              # 人类单位基类
Animal.cpp             # 动物类
StaticRes.cpp          # 静态资源类
Player.cpp             # 玩家类
Core.cpp               # 核心游戏逻辑
Farmer.cpp             # 农民类
ActWidget.cpp          # 动作部件
SelectWidget.cpp       # 选择部件
Development.cpp        # 发展(科技树)类
Army.cpp               # 军队单位类
Bloodhaver.cpp         # 拥有血量的对象基类
Missile.cpp            # 投射物类
Core_CondiFunc.cpp     # 核心条件函数
Core_List.cpp          # 核心对象列表管理
AI.cpp                 # AI基类
Building_Resource.cpp  # 资源建筑类
UsrAI.cpp              # 用户AI
EnemyAI.cpp            # 敌人AI
ViewWidget.cpp         # 视图部件
Option.cpp             # 选项部件
AboutDialog.cpp        # 关于对话框
${PROJECT_UI}          # UI文件
)

set(PROJECT_HEADERS
    MainWidget.h
    GameWidget.h
    config.h    # 配置常量定义
    Coordinate.h
    GlobalVariate.h
    Map.h
    Block.h
    MoveObject.h
    Building.h
    Resource.h
    Human.h
    Animal.h
    StaticRes.h
    Player.h
    Core.h
    Farmer.h
    ActWidget.h
    SelectWidget.h
    Development.h
    Army.h
    Bloodhaver.h
    Missile.h
    Core_CondiFunc.h
    Core_List.h
    AI.h
    Building_Resource.h
    UsrAI.h
    EnemyAI.h
    ViewWidget.h
    Option.h
    digitalConfig.h        # 数字化配置
    AboutDialog.h
)

# 主程序目标
add_executable(newAOE
    ${PROJECT_SOURCES}
    res.rcc    # 资源文件
)

# 添加编译器定义
target_compile_definitions(newAOE PRIVATE QT_DEPRECATED_WARNINGS)

# 设置 include 路径，确保编译器能找到 config.h
target_include_directories(newAOE PRIVATE ${CMAKE_SOURCE_DIR})

target_include_directories(newAOE PRIVATE ${CMAKE_SOURCE_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})

# 链接 Qt 库
target_link_libraries(newAOE
    PRIVATE Qt5::Core     # 核心功能
            Qt5::Gui      # GUI功能
            Qt5::Widgets
            Qt5::Multimedia  # 添加多媒体模块
        )

link_directories(${Qt5Core_LIBRARY_DIRS} ${Qt5Gui_LIBRARY_DIRS} ${Qt5Widgets_LIBRARY_DIRS})
