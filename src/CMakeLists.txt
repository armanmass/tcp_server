add_executable(
	RunTCPServer
	main.cpp
)

target_link_libraries(
	RunTCPServer
	PRIVATE
		TCPServerLib	
)