include(${PROJECT_SOURCE_DIR}/cmake/modules/Find_glfw.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/modules/Find_glad.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/modules/Find_stb.cmake)

if (NOT glfw_FOUND)
  message(FATAL_ERROR "glfw not found!")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(
  hello_window hello_window.cpp ${glad_SOURCES})
target_include_directories(
  hello_window PUBLIC ${glad_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS})
target_link_libraries(
  hello_window ${glfw_LIBRARIES})


add_executable(
  hello_triangle hello_triangle.cpp ${glad_SOURCES})
target_include_directories(
  hello_triangle PUBLIC ${glad_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS})
target_link_libraries(
  hello_triangle ${glfw_LIBRARIES})


add_executable(
  hello_shader hello_shader.cpp ${glad_SOURCES})
target_include_directories(
  hello_shader PUBLIC ${glad_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS})
target_link_libraries(
  hello_shader ${glfw_LIBRARIES})


add_executable(
  hello_texture hello_texture.cpp ${glad_SOURCES})
target_include_directories(
  hello_texture PUBLIC ${stb_INCLUDE_DIRS} ${glad_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS})
target_link_libraries(
  hello_texture ${glfw_LIBRARIES})
