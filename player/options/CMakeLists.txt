project(xibo-options)

add_executable(${PROJECT_NAME}
    main.cpp
    MainWindowController.cpp
    MainWindowController.hpp
    Resources.hpp
    Bypass.cpp
    Bypass.hpp
)

# !!!cagri!!! -------------------------
add_library(OPTIONS_BYPASS
    Bypass.cpp
    Bypass.hpp
)
# -------------------------------------

target_link_libraries(${PROJECT_NAME}
    cms
    config
    system
    ${GTKMM_LINK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTKMM_INCLUDE_DIRS}
)
