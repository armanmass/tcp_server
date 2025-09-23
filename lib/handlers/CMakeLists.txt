add_library(
    HandlersLib 
    STATIC
        EpollHandler.cpp
)

target_include_directories(
    HandlersLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)