cmake_minimum_required (VERSION 3.10)
project ("LoopieEditor" VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE  SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

set(OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "${SRC_DIR}")

target_link_libraries(${PROJECT_NAME} PUBLIC loopie-core)

file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${OUTPUT_PATH})

set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vendor")
get_filename_component(VENDOR_DIR "${VENDOR_DIR}" ABSOLUTE)

set(FMOD_DIR "${VENDOR_DIR}/fmod")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(FMOD_ARCH_DIR "x64")
else()
    set(FMOD_ARCH_DIR "x86")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copiando DLLs de FMOD para el Editor..."
    
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FMOD_DIR}/api/core/lib/${FMOD_ARCH_DIR}/fmod.dll"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FMOD_DIR}/api/core/lib/${FMOD_ARCH_DIR}/fmodL.dll"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FMOD_DIR}/api/studio/lib/${FMOD_ARCH_DIR}/fmodstudio.dll"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FMOD_DIR}/api/studio/lib/${FMOD_ARCH_DIR}/fmodstudioL.dll"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)