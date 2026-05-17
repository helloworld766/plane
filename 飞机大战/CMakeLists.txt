cmake_minimum_required(VERSION 3.15)
project(PlaneWar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== EasyX 配置（位于 thirdparty 文件夹） =====
set(EASYX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/easyx")
include_directories("${EASYX_ROOT}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(EASYX_LIB "${EASYX_ROOT}/lib/x64/EasyXw.lib")
else()
    set(EASYX_LIB "${EASYX_ROOT}/lib/x86/EasyXa.lib")
endif()

# ===== 自动收集源文件 =====
# 告诉编译器在 飞机大战/ 文件夹下找头文件
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/飞机大战")

# 自动收集所有 .cpp 和 .h 文件（递归子文件夹）
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/飞机大战/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/飞机大战/*.h"
)

# 如果还有 .rc 资源脚本（比如自定义图标），也可以一并收集
file(GLOB_RECURSE RC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/飞机大战/*.rc"
)
list(APPEND SOURCES ${RC_FILES})

# ===== 生成可执行文件 =====
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

# ===== 链接库 =====
target_link_libraries(${PROJECT_NAME} PRIVATE
    "${EASYX_LIB}"
    winmm
    gdi32
    user32
)

# ===== 资源文件复制 =====
set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/飞机大战")

# 收集需要复制的资源文件（图片、音频等，不要 .cpp .h .rc）
file(GLOB_RECURSE RESOURCE_FILES
    "${RESOURCE_DIR}/*.png"
    "${RESOURCE_DIR}/*.jpg"
    "${RESOURCE_DIR}/*.jpeg"
    "${RESOURCE_DIR}/*.bmp"
    "${RESOURCE_DIR}/*.gif"
    "${RESOURCE_DIR}/*.wav"
    "${RESOURCE_DIR}/*.mp3"
)

if(RESOURCE_FILES)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${RESOURCE_FILES}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "复制资源文件到输出目录"
    )
endif()