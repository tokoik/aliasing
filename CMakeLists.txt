cmake_minimum_required(VERSION 3.22)
project(aliasing LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/utf-8")
endif()

# ソースファイル
set(SOURCES
    main.c
    menu.c
    quaternion.c
)

# ヘッダファイル
set(HEADERS
    aliasing.h
    menu.h
)

# リソースファイル
set(RESOURCE_FILES
)

# ソリューションエクスプローラー用のグループ分け
source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Resource Files" FILES ${RESOURCE_FILES})

# 外部ライブラリをプロジェクト内の libs/ フォルダへ自動ダウンロードする設定
set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

if(APPLE)

    # macOS の設定
    set(ICON_FILE "opengl.icns")

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${RESOURCE_FILES} ${ICON_FILE})

    find_library(GLUT_LIBRARY GLUT)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GLUT_LIBRARY} ${OPENGL_LIBRARY})

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_ICON_FILE "${ICON_FILE}"
        MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2004-2026 Kohe Tokoi. All rights reserved."
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.yourcompany.${PROJECT_NAME}"
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    set_source_files_properties(${RESOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY XCODE_GENERATE_SCHEME ON)

elseif(WIN32)

    # Windows (Visual Studio) の設定
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCE_FILES})

    include(FetchContent)
    FetchContent_Declare(
        freeglut
        GIT_REPOSITORY https://github.com/FreeGLUTProject/freeglut.git
        GIT_TAG master
    )

    set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(freeglut)

    find_package(OpenGL REQUIRED)

    target_include_directories(${PROJECT_NAME} PRIVATE "${freeglut_SOURCE_DIR}/include")
    target_link_libraries(${PROJECT_NAME} PRIVATE freeglut_static OpenGL::GL OpenGL::GLU)
    target_compile_definitions(${PROJECT_NAME} PRIVATE FREEGLUT_STATIC)

    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

else()

    # Ubuntu Linux (Makefile) の設定
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCE_FILES})

    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        OpenGL::GL
        OpenGL::GLU
        GLUT::GLUT
        m
    )

endif()
