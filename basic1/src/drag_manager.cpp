#include <drag_manager.hpp>

// lib
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>

namespace iscg {

bool DragManager::isDragging_ = false;
bool DragManager::isBinded_ = false;

DragManager::DragManager(GLFWwindow* window, hnll::HgeCamera& camera) 
  : window_(window), camera_(camera)
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
    isBinded_ = false;
  }
}

void DragManager::updateActor(float dt)
{
  if (!isDragging_) return ;

  calcCursorProjectionIntersect();
}

glm::vec2 DragManager::calcRawClickPoint()
{
  int windowX, windowY;
  glfwGetWindowSize(window_, &windowX, &windowY);
  double clickX, clickY;
  glfwGetCursorPos(window_, &clickX, &clickY);

  // center origin
  clickX -= windowX / 2.f;
  clickX /= windowX / 2.f;
  clickY -= windowY / 2.f;
  clickY /= windowY / 2.f;

  return {clickX, clickY};
}

// takes click point, drag comp pos, projected radius of drag comp
float onTheLineJudge(glm::vec2& a, glm::vec2& b, float buffer)
{
  if (glm::distance(a, b) > buffer) return -1;
  
  // the larger the buffer is, the closer the comp is to the camera
  return buffer;
}

void DragManager::calcCursorProjectionIntersect()
{
  glm::vec2 clickPoint = calcRawClickPoint();
  isChanged_ = false;

  if (!isBinded_) {
    float maxPriority = -1;
    // if cursor is dragging something
    for (const auto& kv : dragCompMap_) {
      const auto& comp = kv.second;
      // calc projected (virtual) sphere
      glm::vec3 cameraOriginatedComp = 
        camera_.viewerComponent()->getProjection() * 
        camera_.viewerComponent()->getView() * 
        glm::vec4(comp->getTransform().translation_m, 1.0f);

      glm::vec2 compPos = {cameraOriginatedComp.x / cameraOriginatedComp.z, cameraOriginatedComp.y / cameraOriginatedComp.z};
      // TODO : calc proper buffer coefficient
      float buffer = comp->getRadius() / cameraOriginatedComp.z * 2;
      
      // relaxation
      auto priority = onTheLineJudge(clickPoint, compPos, buffer);
      if (priority == -1) continue;
      if (maxPriority == -1 || priority > maxPriority) {
        maxPriority = priority;
        bindedCompId_ = comp->getCompId();
        cameraOriginatedCompZ_ = cameraOriginatedComp.z;
      }
    }
    if (maxPriority != -1) isBinded_ = true;
  }
  if (isBinded_) {
    // TODO : calc new position correctly
    auto view = camera_.viewerComponent();
    auto z = dragCompMap_[bindedCompId_]->getTransform().translation_m.z;
    glm::vec3  cameraCompPoint = calcWorldClickPoint(clickPoint);
    auto projVec = cameraCompPoint - camera_.getTransform().translation_m;
    projVec *= cameraOriginatedCompZ_ / hnll::ViewerComponent::getNearDistance();
    auto newPos = camera_.getTransform().translation_m + projVec;

    if (newPos != dragCompMap_[bindedCompId_]->getTransform().translation_m) {
      dragCompMap_[bindedCompId_]->getTransform().translation_m = 
        camera_.getTransform().translation_m + projVec;
      isChanged_ = true;
    }
  }
}

glm::vec3 DragManager::calcWorldClickPoint(const glm::vec2& clickPoint)
{
  auto near = hnll::ViewerComponent::getNearDistance();
  auto worldWidth = near * std::tan(hnll::ViewerComponent::getFovy() / 2);
  int w, h; glfwGetWindowSize(window_, &w, &h);
  auto worldHeight = worldWidth * ((float)h / (float)w);

  return camera_.getTransform().mat4() * glm::vec4(clickPoint.x * worldWidth, clickPoint.y * worldHeight, near, 1);
}

} // namespace iscg