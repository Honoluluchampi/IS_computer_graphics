#include <drag_manager.hpp>

// lib
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>

namespace iscg {

bool DragManager::isDragging_ = false;

DragManager::DragManager(GLFWwindow* window) : window_(window)
{
  // set mouse button call-back
  glfwSetMouseButtonCallback(window_, mouseButtonCallback);
}

void DragManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  // imgui
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
  
  // iscg
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    isDragging_ = true;
  else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    isDragging_ = false;
  }
}

void DragManager::updateActor(float dt)
{
  if (!isDragging_) return ;

  double xpos, ypos;
  glfwGetCursorPos(window_, &xpos, &ypos);
  std::cout << xpos << " " << ypos << '\n';
}

void DragManager::calcCursorProjectionIntersect()
{
  if (!isDragging_) return ;

  // if cursor is dragging something
  for (const auto& kv : dragCompMap_) {
    const auto& comp = kv.second;

  }
}

} // namespace iscg