#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
public:
  glm::vec3 pos;
  glm::vec3 front;
  glm::vec3 up;

  GLfloat yaw;
  GLfloat pitch;
  GLfloat fov;

  GLfloat mouse_sensitivity;
  GLfloat movement_speed;

  Camera()
      : pos(glm::vec3(0.0f, 0.0f, 3.0f)),    // pos
        front(glm::vec3(0.0f, 0.0f, -1.0f)), // front
        up(glm::vec3(0.0f, 1.0f, 0.f)),      // up
        yaw(-90.0), pitch(0),                // Euler angles
        fov(45.0),                           // fov
        mouse_sensitivity(0.1),              // mouse move sensitivity
        movement_speed(10)                   // keyboard movement speed

  {
    on_euler_angle_change();
  }

  glm::mat4 get_view() { return glm::lookAt(pos, pos + front, up); }

  void on_mouse_move(GLfloat xoffset, GLfloat yoffset, GLboolean constraint_pitch = true) {
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constraint_pitch) {
      if (pitch > 89.0f)
        pitch = 89.0f;
      if (pitch < -89.0f)
        pitch = -89.0f;
    }

    on_euler_angle_change();
  }

  void on_mouse_scroll(GLfloat yoffset) {
    if (fov >= 1.0f && fov <= 45.0f)
      fov -= yoffset;
    if (fov <= 1.0f)
      fov = 1.0f;
    if (fov >= 45.0f)
      fov = 45.0f;
  }

  void on_keyboard_move(CameraMovement direction, float delta_time) {
    GLfloat camera_speed = movement_speed * delta_time;
    switch (direction) {
    case FORWARD:
      pos += camera_speed * front;
      break;
    case BACKWARD:
      pos -= camera_speed * front;
      break;
    case LEFT:
      pos -= glm::normalize(glm::cross(front, up)) * camera_speed;
      break;
    case RIGHT:
      pos += glm::normalize(glm::cross(front, up)) * camera_speed;
      break;
    }
  }

private:
  void on_euler_angle_change() {
    glm::vec3 _front;
    _front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    _front.y = sin(glm::radians(pitch));
    _front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(_front);
  }
};

#endif // CAMERA_H
