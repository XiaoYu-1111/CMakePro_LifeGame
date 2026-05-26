# ==========================================
# 1. 基础环境配置 (必须在 project 之前)
# ==========================================
cmake_minimum_required(VERSION 3.18) # 建议最低 3.18 以支持现代 CUDA 属性

# [核心修复] 针对 MSVC + Ninja 环境下的 NVCC 路径转义 Bug
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
endif()
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded) # 强制 /Z7 嵌入调试信息
set(CMAKE_CUDA_COMPILE_OPTIONS_MSVC_PDB_OUTPUT_PATH "" CACHE STRING "" FORCE) # 彻底禁用 -Fd 参数

# 设置显卡架构 (RTX 4060 对应 89)
set(CMAKE_CUDA_ARCHITECTURES native)# native自动检测

# [关键修改] 显式声明支持 CUDA 语言
project(CMakePro_CUDA_LifeGame_V2 LANGUAGES CXX C CUDA)

set(TARGET_NAME "CMakePro_CUDA_LifeGame_V2")

# ==========================================
# 2. 第三方库路径设置 (保留绝对路径方式)
# ==========================================
set(LIB_ROOT "D:/CPP_lib")
set(OPENGL_ROOT "${LIB_ROOT}/OpenGL")
set(SDL2_ROOT   "${LIB_ROOT}/SDL2/SDL2-devel-2.30.9-VC/SDL2-2.30.9")
set(CUDAToolkit_ROOT "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.8")

# ==========================================
# 3. 收集源文件
# ==========================================
# ImGui & ImPlot (Docking 分支)
file(GLOB IMGUIDocking_SRCS
    "ImGui_docking/*.cpp"
    "ImPlot/*.cpp"
)

# CUDA 核函数文件
file(GLOB CUDA_USER_SOURCES "CUDA_Kernels/Cuda_Check.cu")
file(GLOB CUDA_USER_HEADERS "CUDA_Kernels/Cuda_Check.cuh")

# 项目主源文件
set(PROJECT_SRCS "Main_cpp/CMakePro_cuda.cpp")

# STB Image (仅头文件)
set(STB_SRCS "stb_image/stb_image_write.h")

# ==========================================
# 4. 创建可执行文件
# ==========================================
add_executable(${TARGET_NAME}
    ${IMGUIDocking_SRCS}
    ${STB_SRCS}
    ${PROJECT_SRCS} 
    ${CUDA_USER_SOURCES}
    ${CUDA_USER_HEADERS}
    "Main_header/Common.h"
)

# ==========================================
# 5. 编译选项与标准设置
# ==========================================
set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CUDA_STANDARD 17
    MSVC_DEBUG_INFORMATION_FORMAT Embedded
)

#解决cuda链接出错问题
if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:/FS>
    )
endif()

# 针对不同语言的特定编译选项
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:
        --use_fast_math
        -Xcompiler=/wd4819
        -Xcompiler=/utf-8
        -Xcompiler=/Z7
    >
)

# ==========================================
# 6. 资源文件同步 (resources_gpu_v32)
# ==========================================
set(RESOURCE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources_LifeGame_V2")
set(RESOURCE_DEST_DIR "${CMAKE_BINARY_DIR}/resources_LifeGame_V2")

file(GLOB_RECURSE RESOURCE_FILES "${RESOURCE_SRC_DIR}/*")

add_custom_target(copy_resources_LifeGame_V2
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${RESOURCE_SRC_DIR} ${RESOURCE_DEST_DIR}
    DEPENDS ${RESOURCE_FILES}
    COMMENT "Syncing resource files..."
)
add_dependencies(${TARGET_NAME} copy_resources_LifeGame_V2)

# ==========================================
# 7. 配置头文件路径 (Include)
# ==========================================
target_include_directories(${TARGET_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Main_header"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGui_docking"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImPlot"
    "${CMAKE_CURRENT_SOURCE_DIR}/stb_image"
    "${CMAKE_CURRENT_SOURCE_DIR}/CUDA_Kernels"
    
    # 第三方库
    "${OPENGL_ROOT}/glm-master/glm-master"
    "${OPENGL_ROOT}/glew-2.1.0-win32/glew-2.1.0/include"
    "${OPENGL_ROOT}/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/include"
    "${SDL2_ROOT}/include"
    "${CUDAToolkit_ROOT}/include"
)

# ==========================================
# 8. 配置库路径 (Link Directories)
# ==========================================
target_link_directories(${TARGET_NAME} PRIVATE
    "${OPENGL_ROOT}/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/lib-vc2022"
    "${OPENGL_ROOT}/glew-2.1.0-win32/glew-2.1.0/lib/Release/x64"
    "${SDL2_ROOT}/lib/x64"
    "${CUDAToolkit_ROOT}/lib/x64"
)

# ==========================================
# 9. 链接库与定义
# ==========================================
target_compile_definitions(${TARGET_NAME} PRIVATE GLEW_STATIC)

target_link_libraries(${TARGET_NAME} PRIVATE
    # OpenGL
    glfw3.lib
    glew32s.lib
    opengl32.lib
    user32.lib
    gdi32.lib
    shell32.lib
    
    # SDL2
    SDL2.lib
    SDL2_image.lib
    SDL2_mixer.lib

    # CUDA (显式路径下的库名)
    cudart.lib
    cuda.lib
)

# ==========================================
# 10. IDE 视图美化 (可选)
# ==========================================
source_group("Source Files\\CUDA" FILES ${CUDA_USER_SOURCES} ${CUDA_USER_HEADERS})
source_group("Source Files\\ImGui" FILES ${IMGUIDocking_SRCS})