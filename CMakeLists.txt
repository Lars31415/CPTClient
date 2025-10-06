cmake_minimum_required(VERSION 3.5)

project(cpt_test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS "-Xlint:deprecation ")
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cpt_test
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        RainbowGradient.cpp
        RainbowGradient.hpp
        records/cptrecord.h records/cptrecord.cpp
        records/cpttest.h records/cpttest.cpp
        cpt_test.qrc
        cpt_test.rc
        click_label.cpp
        click_label.h
        ipdialog.h ipdialog.cpp ipdialog.ui
        ../qt-toast/src/Toast.cpp ../qt-toast/src/Toast.h
        temperaturegradient.h temperaturegradient.cpp
        gradientdialog.h gradientdialog.cpp gradientdialog.ui
        testgradient.h testgradient.cpp


    )
# Define target properties for Android with Qt 6 as:
   set_property(TARGET cpt_test APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(cpt_test SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(cpt_test
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(cpt_test PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(cpt_test PUBLIC
${PROJECT_SOURCE_DIR}
../qt-toast/src/
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.cpt_test)
endif()
set_target_properties(cpt_test PROPERTIES
    ${BUNDLE_ID_OPTION}
    # MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    # MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    # MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS cpt_test
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cpt_test)
endif()
