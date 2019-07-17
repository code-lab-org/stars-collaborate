# Create the makefiles for the collaborate library
cmake_minimum_required(VERSION 2.8)
set(LIB_NAME osse_collaborate)
file(GLOB SRCS *.cpp)
file(GLOB INCS ../include/collaborate/*.h)
add_library(${LIB_NAME} SHARED ${SRCS} ${INCS})
include_directories(
  "${PROJECT_SOURCE_DIR}/libs/spdlog/include/"
  "${PROJECT_SOURCE_DIR}/libs/netcdf/include/"
  "${PROJECT_SOURCE_DIR}/libs/sgp4/include/"
  "${PROJECT_SOURCE_DIR}/libs/collaborate/include/"
  )
target_link_libraries(
  ${LIB_NAME}
  "${PROJECT_BINARY_DIR}/libs/netcdf/src/libosse_netcdf.${LIB_SUFFIX}"
  "${PROJECT_BINARY_DIR}/libs/sgp4/src/libosse_sgp4.${LIB_SUFFIX}"
  stdc++fs
  )
install(TARGETS ${LIB_NAME} LIBRARY DESTINATION lib)
install(FILES ${INCS} DESTINATION include/collaborate/)
