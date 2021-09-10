set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT "MinimalQml")
project(${PROJECT})

find_package(Qt5 COMPONENTS Core Widgets Qml Quick REQUIRED)

if (Qt5_FOUND)
    add_executable(${PROJECT}
      main.cpp
      Launcher.h
      qml.qrc
    )

    target_compile_definitions(${PROJECT}
      PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
    target_link_libraries(${PROJECT}
        PRIVATE Qt5::Core Qt5::Quick Qt5::Qml Qt5::Widgets)

    #target_link_libraries(${PROJECT}
    #    PUBLIC FWI_PreProcess FWI_Process FWI_PostProcess)
    install(TARGETS ${PROJECT} DESTINATION bin)
endif()
