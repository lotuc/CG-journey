set(glfw_NAME "glfw")

find_path(
  glfw_INCLUDE_DIR

  NAMES "GLFW/glfw3.h"

  PATHS
  "/usr/include"
  "/usr/local/include"

  NO_DEFAULT_PATH
)

find_library(
  glfw_LIBRARY

  NAMES glfw

  PATHS
  "/usr/local/lib"
  "/usr/lib"

  NO_DEFAULT_PATH
)

if(glfw_INCLUDE_DIR AND glfw_LIBRARY)
  set(glfw_FOUND TRUE)
  set(glfw_INCLUDE_DIRS ${glfw_INCLUDE_DIR})
  set(glfw_LIBRARIES ${glfw_LIBRARY})
else()
  set(glfw_FOUND FALSE)
  message(STATUS "Could not find glfw.  Make sure it's installed and the paths are correct.")
endif()

mark_as_advanced(glfw_INCLUDE_DIR glfw_LIBRARY)
