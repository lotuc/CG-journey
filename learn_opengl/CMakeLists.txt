include(${PROJECT_SOURCE_DIR}/cmake/modules/Find_glfw.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/modules/Find_glad.cmake)

if (NOT glfw_FOUND)
  message(FATAL_ERROR "glfw not found!")
endif()

add_executable(
  hello_window hello_window.cpp ${glad_SOURCES})
target_include_directories(
  hello_window PUBLIC ${glad_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS})
target_link_libraries(
  hello_window ${glfw_LIBRARIES})
