cmake_minimum_required (VERSION 3.10)

project ("Loopie" VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

set(WWISE_DIR "${VENDOR_DIR}/wwise/SDK" CACHE PATH "SDK Wwise in Loopie/vendor")

if(NOT EXISTS "${WWISE_DIR}/include")
    message(FATAL_ERROR
        "SDK Not found: ${WWISE_DIR}\n"
        "Expected: ${WWISE_DIR}/include"
    )
endif()

#VS2022
set(WWISE_TOOLSET_DIR "x64_vc170")

set(WWISE_LIB_DIR_DEBUG    "${WWISE_DIR}/${WWISE_TOOLSET_DIR}/Debug/lib")
set(WWISE_LIB_DIR_RELEASE  "${WWISE_DIR}/${WWISE_TOOLSET_DIR}/Release/lib")

#Libs
set(WWISE_AKSOUNDENGINE_LIB_DEBUG   "${WWISE_LIB_DIR_DEBUG}/AkSoundEngine.lib")
set(WWISE_AKMEMORYMGR_LIB_DEBUG     "${WWISE_LIB_DIR_DEBUG}/AkMemoryMgr.lib")
set(WWISE_AKSTREAMMGR_LIB_DEBUG     "${WWISE_LIB_DIR_DEBUG}/AkStreamMgr.lib")

set(WWISE_AKSOUNDENGINE_LIB_RELEASE "${WWISE_LIB_DIR_RELEASE}/AkSoundEngine.lib")
set(WWISE_AKMEMORYMGR_LIB_RELEASE   "${WWISE_LIB_DIR_RELEASE}/AkMemoryMgr.lib")
set(WWISE_AKSTREAMMGR_LIB_RELEASE   "${WWISE_LIB_DIR_RELEASE}/AkStreamMgr.lib")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h" "${VENDOR_DIR}/*.cpp" "${VENDOR_DIR}/*.h")

list(FILTER SOURCES EXCLUDE REGEX "/wwise/")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

find_package(spdlog REQUIRED)
find_package(glad REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(SDL3 REQUIRED)
find_package(nfd REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(assimp REQUIRED)
find_package(lz4 REQUIRED)
find_package(imguizmo REQUIRED)
find_package(imgui REQUIRED)
find_package(DevIL REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(loopie-core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC "${SRC_DIR}" "${VENDOR_DIR}" "${WWISE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PUBLIC
        spdlog::spdlog
        glad::glad
        OpenGL::GL
        glm::glm
        SDL3::SDL3
        nfd::nfd
        nlohmann_json::nlohmann_json
        assimp::assimp
        lz4::lz4
        imguizmo::imguizmo
        imgui::imgui
        DevIL::IL
        DevIL::ILU

    #Wwise
    $<$<CONFIG:Debug>:${WWISE_AKSOUNDENGINE_LIB_DEBUG}>
    $<$<CONFIG:Debug>:${WWISE_AKMEMORYMGR_LIB_DEBUG}>
    $<$<CONFIG:Debug>:${WWISE_AKSTREAMMGR_LIB_DEBUG}>

    $<$<CONFIG:Release>:${WWISE_AKSOUNDENGINE_LIB_RELEASE}>
    $<$<CONFIG:Release>:${WWISE_AKMEMORYMGR_LIB_RELEASE}>
    $<$<CONFIG:Release>:${WWISE_AKSTREAMMGR_LIB_RELEASE}>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS GLFW_INCLUDE_NONE)


