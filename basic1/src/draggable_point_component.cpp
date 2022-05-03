#include <draggable_point_component.hpp>

// std
#include <iostream>

namespace hnll {

bool DraggableComponent::isDragging_ = false;

DraggableComponent::DraggableComponent(GLFWwindow* window) : HgeComponent(), window_(window)
{
  // set mouse button call-back
  glfwSetMouseButtonCallback(window_, mouseButtonCallback);
}

void DraggableComponent::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    isDragging_ = true;
  else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    isDragging_ = false;
  }
}

void DraggableComponent::updateComponent(float dt)
{
  if (intersectsWithClickProjection())
      moveToClickProjection();
}

bool DraggableComponent::intersectsWithClickProjection()
{
  // mouse is not dragging
  if (!isDragging_) return false;

  // get cursor position 
  double xpos, ypos;
  glfwGetCursorPos(window_, &xpos, &ypos);
  std::cout << "x: " << xpos << " y: " << ypos << '\n';
  return false;
}

} // namespace hnll