add_library(ServerLib TCPServer.cpp)

target_include_directories(
    ServerLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}    
)

target_link_libraries(
    ServerLib 
    PUBLIC 
        HandlersLib
)
