add_subdirectory(handlers)
add_subdirectory(tcp_server)

add_library(TCPServerLib INTERFACE)

target_link_libraries(
    TCPServerLib
    INTERFACE
        HandlersLib
        ServerLib
)
