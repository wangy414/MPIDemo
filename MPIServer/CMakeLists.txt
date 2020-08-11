cmake_minimum_required(VERSION 2.8)


set(PROJECT_ROOT_PATH "./")  

set(CMAKE_C_COMPILER   mpicc)
set(CMAKE_CXX_COMPILER   mpicc)

add_definitions(-std=c++11)
include_directories(
${PROJECT_SOURCE_DIR}/
)



set(src_client 
	${PROJECT_SOURCE_DIR}/mpi_client.cpp
)
link_directories(${PROJECT_SOURCE_DIR})
add_executable(mpiclient ${src_client})
target_link_libraries(mpiclient -lpthread -lc -lm -lirc -lz -lsvml -lstdc++)



set(src_server 
	${PROJECT_SOURCE_DIR}/mpi_server.cpp
)
link_directories(${PROJECT_SOURCE_DIR})
add_executable(mpiserver ${src_server})
target_link_libraries(mpiserver -lpthread -lc -lm -lirc -lz -lsvml -lstdc++)