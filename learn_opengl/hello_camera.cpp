#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "camera.h"
#include "shader.h"

#include "data0.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int load_texture(const char *texture_file, GLenum format, bool flip) {
  int width, height, nrChannels;

  if (flip) {
    stbi_set_flip_vertically_on_load(flip);
  }

  unsigned char *data = stbi_load(texture_file, &width, &height, &nrChannels, 0);

  unsigned int texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  return texture;
}

bool keys[1024];

GLboolean first_mouse = true;
GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;
GLfloat last_x = SCR_WIDTH / 2.0f, last_y = SCR_HEIGHT / 2.0f;

Camera camera;

void setup_vbo(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first Triangle
      1, 2, 3  // second Triangle
  };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int main() {

  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

  glfwSetScrollCallback(window, scroll_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  Shader shader("learn_opengl/shaders/3.6.shader.vs", "learn_opengl/shaders/3.6.shader.fs");

  unsigned int VBO, VAO, EBO;
  setup_vbo(VBO, VAO, EBO);

  shader.use();

  glActiveTexture(GL_TEXTURE0); // default behavior
  unsigned int texture1 = load_texture("learn_opengl/textures/container.jpg", GL_RGB, false);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glActiveTexture(GL_TEXTURE1);
  unsigned int texture2 = load_texture("learn_opengl/textures/awesomeface.png", GL_RGBA, true);
  glBindTexture(GL_TEXTURE_2D, texture2);

  shader.set_int("texture1", 0);
  shader.set_int("texture2", 1);

  glEnable(GL_DEPTH_TEST);

  last_frame = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    GLfloat current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);

    shader.set_mat4("projection", glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 1000.0f));
    shader.set_mat4("view", camera.get_view());

    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cube_positions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0, 0.3f, 0.5f));
      shader.set_mat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(window, true);

  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if (keys[GLFW_KEY_W])
    camera.on_keyboard_move(FORWARD, delta_time);
  if (keys[GLFW_KEY_S])
    camera.on_keyboard_move(BACKWARD, delta_time);
  if (keys[GLFW_KEY_A])
    camera.on_keyboard_move(LEFT, delta_time);
  if (keys[GLFW_KEY_D])
    camera.on_keyboard_move(RIGHT, delta_time);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (first_mouse) {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }

  float xoffset = xpos - last_x;
  float yoffset = ypos - last_y;
  last_x = xpos;
  last_y = ypos;

  camera.on_mouse_move(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) { camera.on_mouse_scroll(yoffset); }
